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

#include <libhidx/hid/Control.hh>

#include <algorithm>

namespace libhidx {
namespace hid {

    void Control::setData(const std::vector<unsigned char>& rawData, unsigned reportId) {
        if(m_usages.empty()){
            return;
        }

        if(reportId != m_reportId){
            return;
        }

        auto data = extractData(rawData);
        if(isVariable()){
            // variable
            for(unsigned i = 0; i < m_reportCount; ++i){
                uint32_t usageData = extractVariableUsageData(data, i);
                m_usages[i]->setLogicalValue(usageData);
            }
        } else {
            // array
            for(auto& usage: m_usages){
                usage->setLogicalValue(0);
            }

            for(unsigned i = 0; i < m_reportCount; ++i){
                uint32_t usageIndex = extractVariableUsageData(data, i);

                auto usage = findUsageById(usageIndex);
                if(!usage){
                    continue;
                }

                usage->setLogicalValue(1);
            }
        }
    }

    std::vector<bool> Control::extractData(const std::vector<unsigned char>& vector) {
        auto length = m_reportCount * m_reportSize;
        std::vector<bool> data;
        data.resize(length);

        for(unsigned i = 0; i < length; ++i){
            auto originPos = m_offset + i;
            auto bytePos = originPos / 8;
            auto bitPos = originPos % 8;

            unsigned char byte = vector[bytePos];
            byte >>= bitPos;
            data[i] = (byte & 1) == 1;
        }

        return data;
    }

    uint32_t Control::extractVariableUsageData(const std::vector<bool>& data, unsigned index) {
        auto offset = index * m_reportSize;
        uint32_t ret = 0;

        for(auto i = 0U; i < m_reportSize; ++i){
            ret |= data[i + offset] << i;
        }

        return ret;
    }

    Usage* Control::findUsageById(uint32_t id) {
        auto it = std::find_if(begin(m_usages), end(m_usages), [&id](const auto &usage){
            return (usage->getId() & 0xff) == id;
        });

        if(it == end(m_usages)){
            return nullptr;
        }

        return (*it).get();
    }

    uint32_t Control::getData() const {
        uint32_t data = 0;
        unsigned i = 0;
        for(const auto& usage: m_usages){
            auto usageData = usage->getLogicalValue();

            data |= usageData << (m_reportSize * i);
            ++i;
        }

        return data;
    }

    bool Control::isBinary() const {
        return !isVariable() ||(getLogicalMinimum() == 0 && getLogicalMaximum() == 1 && getUnit() == 0 && getUnitExponent() == 0);
    }
}
}
