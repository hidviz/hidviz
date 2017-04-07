#include "libhidx/Parser.hh"

#include "libhidx/hid/Collection.hh"
#include "libhidx/hid/Control.hh"

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

    Parser::Parser(uint8_t* start, std::size_t size)
        : m_start{start}, m_size{size} {}


    hid::Item* Parser::parse() {
        auto start = m_start;
        uint8_t* end;

        m_collectionStack.emplace_back(new hid::Collection{});

        static std::array<void (Parser::*)(), 4> dispatchFunctions {
            &Parser::parseMainItem,
            &Parser::parseGlobalItem,
            &Parser::parseLocalItem,
            &Parser::parseReservedItem,
        };

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
                return m_collectionStack.front();
            }
        }
        throw ParserError{"Unexpected parser error."};

    }

    uint8_t* Parser::fetchItem(uint8_t* start, uint8_t* end) {
        uint8_t b;
        m_currentItem = Item{};

        if ((end - start) <= 0)
            return nullptr;

        b = *start++;

        m_currentItem.type = (uint8_t) ((b >> 2) & 3);
        m_currentItem.tag = (uint8_t) ((b >> 4) & 15);

        if (m_currentItem.tag == Item::TAG::LONG) {

            m_currentItem.isLongFormat = true;

            if ((end - start) < 2)
                return nullptr;

            m_currentItem.size = *start++;
            m_currentItem.tag = *start++;

            if ((end - start) < m_currentItem.size)
                return nullptr;

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
                if ((end - start) < 1)
                    return nullptr;
                m_currentItem.data.u8 = *start++;
                return start;

            case 2:
                if ((end - start) < 2)
                    return nullptr;
                m_currentItem.data.u16 = get_unaligned_le16(start);
                start = (uint8_t*) ((uint16_t*) start + 1);
                return start;

            case 3:
                m_currentItem.size++;
                if ((end - start) < 4)
                    return nullptr;
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

    }

    void Parser::closeCollection() {
        if(m_collectionStack.size() <= 1){
            throw ParserError{"Collection stack underrun."};
        }
        m_collectionStack.pop_back();
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

        if (!m_local.usagesStack.size()){ /* Ignore padding fields */
            return;
        }

        auto usages = std::max(
            static_cast<unsigned>(m_local.usagesStack.size()),
            m_global.reportCount
        );

        for (unsigned i = 0; i < usages; i++) {
            unsigned j = i;
            /* Duplicate the last usage we parsed if we have excess values */
            if (i >= m_local.usagesStack.size()){
                j = static_cast<unsigned>(m_local.usagesStack.size()) - 1;
            }

            field->m_usages.emplace_back(new hid::Usage{m_local.usagesStack[j], field});
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
    }

    void Parser::parseGlobalItem() {
        const auto sdata = m_currentItem.sdata();
        const auto udata = m_currentItem.udata();
        switch (m_currentItem.tag) {
            case Item::TAG::GLOBAL::PUSH:
                m_globalStack.emplace_back(m_global);
                return;

            case Item::TAG::GLOBAL::POP:
                if(m_globalStack.empty()){
                    throw ParserError{"Empty global stack and popping."};
                }
                m_global = m_globalStack.back();
                m_globalStack.pop_back();
                return;

            case Item::TAG::GLOBAL::USAGE_PAGE:
                m_global.usagePage = udata;
                return;

            case Item::TAG::GLOBAL::LOGICAL_MINIMUM:
                m_global.logicalMinimum = sdata;
                return;

            case Item::TAG::GLOBAL::LOGICAL_MAXIMUM:
                if (m_global.logicalMinimum < 0)
                    m_global.logicalMaximum = sdata;
                else
                    m_global.logicalMaximum = udata;
                return;

            case Item::TAG::GLOBAL::PHYSICAL_MINIMUM:
                m_global.physicalMinimum = sdata;
                return;

            case Item::TAG::GLOBAL::PHYSICAL_MAXIMUM:
                if (m_global.physicalMinimum < 0)
                    m_global.physicalMaximum = sdata;
                else
                    m_global.physicalMaximum = udata;
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
                return;

            case Item::TAG::GLOBAL::UNIT:
                m_global.unit = udata;
                return;

            case Item::TAG::GLOBAL::REPORT_SIZE:
                m_global.reportSize = udata;
                if (m_global.reportSize > 128) {
                    throw ParserError{"Invalid report size."};
                }
                return;

            case Item::TAG::GLOBAL::REPORT_COUNT:
                m_global.reportCount = udata;
                return;

            case Item::TAG::GLOBAL::REPORT_ID:
                m_global.reportId = udata;
                if (m_global.reportId == 0) {
                    throw ParserError{"Invalid report id."};
                }
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
                return;

            case Item::TAG::LOCAL::USAGE:

                if (m_local.delimiterBranch > 1) {
    //TODO                dbg_hid("alternative usage ignored\n");
                    return;
                }

                if (m_currentItem.size <= 2)
                    udata = (m_global.usagePage << 16) + udata;

                m_local.usagesStack.emplace_back(udata);
                return;

            case Item::TAG::LOCAL::USAGE_MINIMUM:

                if (m_local.delimiterBranch > 1) {
    //TODO                dbg_hid("alternative usage ignored\n");
                    return;
                }

                if (m_currentItem.size <= 2)
                    udata = (m_global.usagePage << 16) + udata;

                m_local.usageMinimum = udata;
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

                return;

            default:
                throw ParserError{"Unknown local item tag"};
        }
    }

    void Parser::parseReservedItem() {
        throw ParserError{"Unknown reserved item tag."};
    }
}
