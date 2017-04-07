#include "libhidx/hid/Control.hh"

#include <algorithm>

namespace libhidx {
namespace hid {

    void Control::setData(const std::vector<unsigned char> &rawData) {
        if(!m_usages.size()){
            return;
        }
        auto data = extractData(rawData);
        //TODO: magic constant!
        if(m_flags & 2){
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

                auto usage = findUsageByValue(usageIndex);
                if(!usage){
                    continue;
                }

                usage->setLogicalValue(1);
            }
        }
    }

    uint32_t Control::extractData(const std::vector<unsigned char> &vector) {
        auto length = m_reportCount * m_reportSize;
        uint32_t data = 0;

        for(unsigned i = 0; i < length; ++i){
            auto originPos = m_offset + i;
            auto bytePos = originPos / 8;
            auto bitPos = originPos % 8;

            unsigned char byte = vector[bytePos];
            byte >>= bitPos;
            byte &= 1;
            data |= byte << i;
        }

        return data;
    }

    uint32_t Control::extractVariableUsageData(uint32_t data, unsigned index) {
        auto i = index * m_reportSize;
        auto mask = (1U << m_reportSize) - 1U;

        return (data >> i) & mask;
    }

    Usage * Control::findUsageByValue(uint32_t value) {
        auto it = std::find_if(begin(m_usages), end(m_usages), [&value](const auto &usage){
            return (usage->getLogicalValue() & 0xff) == value;
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
}
}
