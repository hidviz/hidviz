//
// hidviz - Tool for in-depth analysis of USB HID devices communication
// Copyright (C) 2017 Ondrej Budai <ondrej@budai.cz>
//
// This file is part of hidviz.
//
// hidviz is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// hidviz is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with hidviz.  If not, see <http://www.gnu.org/licenses/>.
//

#include <array>
#include <libhidx/Parser.hh>

#include <libhidx/hid/Collection.hh>
#include <libhidx/hid/Control.hh>
#include <libhidx/Usages.hh>

namespace libhidx {
    static uint16_t get_unaligned_le16(const void* p) {
        return *((uint16_t*) p);
    }

    static uint32_t get_unaligned_le32(const void* p) {
        return *((uint32_t*) p);
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshift-negative-value"
    static int32_t snto32(uint32_t value, unsigned n) {
        switch (n) {
            case 8:
                return ((int8_t) value);
            case 16:
                return ((int16_t) value);
            case 32:
                return ((int32_t) value);
        }
        return value & (1 << (n - 1)) ? value | (-1 << n) : value;
    }
#pragma GCC diagnostic pop

    int32_t hid_snto32(uint32_t value, unsigned n) {
        return snto32(value, n);
    }

    Parser::Parser(const uint8_t* start, std::size_t size)
        : m_start{start}, m_size{size} {}


    void Parser::parse() {
        auto start = m_start;
        const uint8_t* end;

        m_collectionStack.emplace_back(new hid::Collection{});

        static std::array<void (Parser::*)(), 4> dispatchFunctions {{
            &Parser::parseMainItem,
            &Parser::parseGlobalItem,
            &Parser::parseLocalItem,
            &Parser::parseReservedItem,
        }};

        end = m_start + m_size;
        while ((start = fetchItem(start, end)) != nullptr) {

            if (m_currentItem.isLongFormat) {
                throw ParserError{"Long format item found."};
            }

            (this->*dispatchFunctions[m_currentItem.type])();

            if (start == end) {
                if (m_collectionStack.size() != 1) {
                    throw ParserError{"Collections are not balanced."};
                }
                if (m_local.delimiterDepth > 0) {
                    throw ParserError{"Delimiters are not balanced."};
                }
                auto topItem = m_collectionStack.front();
                bool numbered = false;
                topItem->forEach([&numbered](auto item){
                    auto control = dynamic_cast<hid::Control*>(item);
                    if(control){
                        if(control->getReportId()){
                            numbered = true;
                        }
                    }
                });

                topItem->m_numbered = numbered;

                m_parsed = topItem;
                return;
            }
        }
        throw ParserError{"Unexpected parser error."};

    }

    const uint8_t* Parser::fetchItem(const uint8_t* start, const uint8_t* end) {
        uint8_t b;
        m_currentItem = Item{};

        if ((end - start) <= 0) {
            return nullptr;
        }

        b = *start++;

        m_currentItem.type = (uint8_t) ((b >> 2) & 3);
        m_currentItem.tag = (uint8_t) ((b >> 4) & 15);

        if (m_currentItem.tag == Item::TAG::LONG) {

            m_currentItem.isLongFormat = true;

            if ((end - start) < 2) {
                return nullptr;
            }

            m_currentItem.size = *start++;
            m_currentItem.tag = *start++;

            if ((end - start) < m_currentItem.size) {
                return nullptr;
            }

            m_currentItem.data.longdata = start;
            start += m_currentItem.size;
            return start;
        }

        m_currentItem.isLongFormat = false;
        m_currentItem.size = (uint8_t) (b & 3);

        switch (m_currentItem.size) {
            case 0:
                return start;

            case 1:
                if ((end - start) < 1) {
                    return nullptr;
                }
                m_currentItem.data.u8 = *start++;
                return start;

            case 2:
                if ((end - start) < 2) {
                    return nullptr;
                }
                m_currentItem.data.u16 = get_unaligned_le16(start);
                start = (uint8_t*) ((uint16_t*) start + 1);
                return start;

            case 3:
                m_currentItem.size++;
                if ((end - start) < 4) {
                    return nullptr;
                }
                m_currentItem.data.u32 = get_unaligned_le32(start);
                start = (uint8_t*) ((uint32_t*) start + 1);
                return start;
        }

        return nullptr;
    }

    void Parser::parseMainItem() {

        switch (m_currentItem.tag) {
            case Item::TAG::MAIN::BEGIN_COLLECTION:
                openCollection();
                break;
            case Item::TAG::MAIN::END_COLLECTION:
                closeCollection();
                break;
            case Item::TAG::MAIN::INPUT:
                addField(hid::Control::Type::INPUT);
                break;
            case Item::TAG::MAIN::OUTPUT:
                addField(hid::Control::Type::OUTPUT);
                break;
            case Item::TAG::MAIN::FEATURE:
                addField(hid::Control::Type::FEATURE);
                break;
            default:
                throw ParserError{"Unknown main item tag."};
        }
        // reset local environment
        m_local = Local{};
    }

    void Parser::openCollection() {
        hid::Collection* collection = nullptr;
        collection = new hid::Collection{m_collectionStack.back()};
        m_collectionStack.back()->appendChild(collection);
        m_collectionStack.emplace_back(collection);

        collection->m_type = m_currentItem.udata() & 0xff;
        if(m_local.usagesStack.size()) {
            collection->m_usage = m_local.usagesStack.front();
        } else {
            collection->m_usage = 0;
        }

        m_raw += m_currentIndent + "Collection (" + collection->getTypeStr() + ")\n";
        m_currentIndent += INDENT;
    }

    void Parser::closeCollection() {
        if(m_collectionStack.size() <= 1){
            throw ParserError{"Collection stack underrun."};
        }
        m_collectionStack.pop_back();
        m_currentIndent.erase(0, INDENT.length());
        m_raw += m_currentIndent + "End Collection\n";
    }

    void Parser::addField(hid::Control::Type reportType) {
        if(m_collectionStack.size() <= 1){
            throw ParserError{"Field does not belong to collection."};
        }
        auto field = new hid::Control{m_collectionStack.back()};
        m_collectionStack.back()->appendChild(field);

        /* Handle both signed and unsigned cases properly */
        if ((m_global.logicalMinimum < 0 &&
             m_global.logicalMaximum <
             m_global.logicalMinimum) ||
            (m_global.logicalMinimum >= 0 &&
             (uint32_t) m_global.logicalMaximum <
             (uint32_t) m_global.logicalMinimum)) {
            throw ParserError{"Logical range invalid"};
        }

        auto& reportSize = m_reportSizes[std::make_pair(m_global.reportId, reportType)];
        field->m_offset = reportSize;

        reportSize += m_global.reportSize * m_global.reportCount;

        auto usages = std::max(
            static_cast<unsigned>(m_local.usagesStack.size()),
            m_global.reportCount
        );

        if(m_local.usagesStack.size() > 0) {
            for (unsigned i = 0; i < usages; i++) {
                unsigned j = i;
                /* Duplicate the last usage we parsed if we have excess values */
                if (i >= m_local.usagesStack.size()) {
                    j = static_cast<unsigned>(m_local.usagesStack.size()) - 1;
                }

                field->m_usages.emplace_back(new hid::Usage{m_local.usagesStack[j], field});
            }
        }

        field->m_flags = m_currentItem.udata();
        field->m_reportType = reportType;
        field->m_reportSize = m_global.reportSize;
        field->m_reportCount = m_global.reportCount;
        field->m_logicalMinimum = m_global.logicalMinimum;
        field->m_logicalMaximum = m_global.logicalMaximum;
        field->m_physicalMinimum = m_global.physicalMinimum;
        field->m_physicalMaximum = m_global.physicalMaximum;
        field->m_unitExponent = m_global.unitExponent;
        field->m_unit = m_global.unit;
        field->m_reportId = m_global.reportId;

        m_raw += m_currentIndent + "Item\n";
    }

    void Parser::parseGlobalItem() {
        const auto sdata = m_currentItem.sdata();
        const auto udata = m_currentItem.udata();
        switch (m_currentItem.tag) {
            case Item::TAG::GLOBAL::PUSH:
                m_globalStack.emplace_back(m_global);
                m_raw += m_currentIndent + "Push\n";
                return;

            case Item::TAG::GLOBAL::POP:
                if(m_globalStack.empty()){
                    throw ParserError{"Empty global stack and popping."};
                }
                m_global = m_globalStack.back();
                m_globalStack.pop_back();
                m_raw += m_currentIndent + "Pop\n";
                return;

            case Item::TAG::GLOBAL::USAGE_PAGE:
                m_global.usagePage = udata;
                m_raw += m_currentIndent + "Usage Page (" + getHidUsagePageText(udata) + ")\n";
                return;

            case Item::TAG::GLOBAL::LOGICAL_MINIMUM:
                m_global.logicalMinimum = sdata;
                m_raw += m_currentIndent + "Logical minimum (" + std::to_string(sdata) + ")\n";
                return;

            case Item::TAG::GLOBAL::LOGICAL_MAXIMUM:
                if (m_global.logicalMinimum < 0) {
                    m_global.logicalMaximum = sdata;
                } else {
                    m_global.logicalMaximum = udata;
                }
                m_raw += m_currentIndent + "Logical maximum (" + std::to_string(m_global.logicalMaximum) + ")\n";
                return;

            case Item::TAG::GLOBAL::PHYSICAL_MINIMUM:
                m_global.physicalMinimum = sdata;
                m_raw += m_currentIndent + "Physical minimum (" + std::to_string(sdata) + ")\n";
                return;

            case Item::TAG::GLOBAL::PHYSICAL_MAXIMUM:
                if (m_global.physicalMinimum < 0) {
                    m_global.physicalMaximum = sdata;
                } else {
                    m_global.physicalMaximum = udata;
                }
                m_raw += m_currentIndent + "Physical maximum (" + std::to_string(m_global.physicalMaximum) + ")\n";
                return;

            case Item::TAG::GLOBAL::UNIT_EXPONENT:
                /* Many devices provide unit exponent as a two's complement
                 * nibble due to the common misunderstanding of HID
                 * specification 1.11, 6.2.2.7 Global Items. Attempt to handle
                 * both this and the standard encoding. */
                if (!(sdata & 0xfffffff0))
                    m_global.unitExponent = hid_snto32(sdata, 4);
                else
                    m_global.unitExponent = sdata;
                m_raw += m_currentIndent + "Unit exponent (" + std::to_string(m_global.unitExponent) + ")\n";
                return;

            case Item::TAG::GLOBAL::UNIT:
                m_global.unit = udata;
                m_raw += m_currentIndent + "Unit (" + std::to_string(udata) + ")\n";
                return;

            case Item::TAG::GLOBAL::REPORT_SIZE:
                m_global.reportSize = udata;
                if (m_global.reportSize > 128) {
                    throw ParserError{"Invalid report size."};
                }
                m_raw += m_currentIndent + "Report size (" + std::to_string(udata) + ")\n";
                return;

            case Item::TAG::GLOBAL::REPORT_COUNT:
                m_global.reportCount = udata;
                m_raw += m_currentIndent + "Report count (" + std::to_string(udata) + ")\n";
                return;

            case Item::TAG::GLOBAL::REPORT_ID:
                m_global.reportId = udata;
                if (m_global.reportId == 0) {
                    throw ParserError{"Invalid report id."};
                }
                m_raw += m_currentIndent + "Report ID (" + std::to_string(udata) + ")\n";
                return;

            default:
                throw ParserError{"Unknown global tag."};
        }
    }

    void Parser::parseLocalItem() {
        unsigned n;
        auto udata = m_currentItem.udata();

        switch (m_currentItem.tag) {
            case Item::TAG::LOCAL::DELIMITER:
                if (udata) {
                    /*
                     * We treat items before the first delimiter
                     * as global to all usage sets (branch 0).
                     * In the moment we process only these global
                     * items and the first delimiter set.
                     */
                    if (m_local.delimiterDepth != 0) {
                        throw ParserError{"Nested delimiters."};
                    }
                    m_local.delimiterDepth++;
                    m_local.delimiterBranch++;
                } else {
                    if (m_local.delimiterDepth < 1) {
                        throw ParserError{"Bogus close delimiter."};
                    }
                    m_local.delimiterDepth--;
                }
                m_raw += m_currentIndent + "Delimiter\n";
                return;

            case Item::TAG::LOCAL::USAGE:

                if (m_local.delimiterBranch > 1) {
    //TODO                dbg_hid("alternative usage ignored\n");
                    return;
                }

                if (m_currentItem.size <= 2)
                    udata = (m_global.usagePage << 16) + udata;

                m_local.usagesStack.emplace_back(udata);
                m_raw += m_currentIndent + "Usage (" + getHidUsageText(udata) + ")\n";
                return;

            case Item::TAG::LOCAL::USAGE_MINIMUM:

                if (m_local.delimiterBranch > 1) {
    //TODO                dbg_hid("alternative usage ignored\n");
                    return;
                }

                if (m_currentItem.size <= 2)
                    udata = (m_global.usagePage << 16) + udata;

                m_local.usageMinimum = udata;
                m_raw += m_currentIndent + "Usage Minimum (" + getHidUsageText(udata) + ")\n";
                return;

            case Item::TAG::LOCAL::USAGE_MAXIMUM:

                if (m_local.delimiterBranch > 1) {
    //TODO                dbg_hid("alternative usage ignored\n");
                    return;
                }

                if (m_currentItem.size <= 2)
                    udata = (m_global.usagePage << 16) + udata;

                for (n = m_local.usageMinimum; n <= udata; n++){
                    m_local.usagesStack.emplace_back(n);
                }
                m_raw += m_currentIndent + "Usage Maximum (" + getHidUsageText(udata) + ")\n";
                return;

            default:
                throw ParserError{"Unknown local item tag"};
        }
    }

    void Parser::parseReservedItem() {
        throw ParserError{"Unknown reserved item tag."};
    }
}
