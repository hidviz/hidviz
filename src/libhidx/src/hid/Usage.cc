#include "libhidx/hid/Usage.hh"

#include "libhidx/Usages.hh"
#include "libhidx/hid/Control.hh"

#include <stdexcept>
#include <limits>
#include <cmath>

namespace libhidx{
namespace hid {
    Usage::Usage(unsigned usageId, Control* control) : m_id{usageId}, m_control{control} {
        m_name = getHidUsageText(usageId);

    }
    void Usage::setLogicalValue(uint32_t rawLogicalValue) {
        int64_t logicalValue = 0;

        if(m_control->getLogicalMinimum() >= 0 && m_control->getLogicalMaximum() >= 0){
            logicalValue = rawLogicalValue;
        } else {
            auto size = m_control->getSize();
            logicalValue = convertLogicalValue(rawLogicalValue, size);
        }

        m_logicalValue = logicalValue;
        m_physicalValue = logicalToPhysical(logicalValue);
    }

    bool Usage::setLogicalValueFromUser(const std::string& dataString) {
        unsigned data;
        try {
            auto tmp = std::stoul(dataString);
            if(std::numeric_limits<unsigned>::max() < tmp){
                throw std::overflow_error{""};
            }
            data = static_cast<unsigned>(tmp);
        } catch(const std::exception& ex) {
            return false;
        }

        //TODO: check range
        m_logicalValue = data;

        return true;

    }

    double Usage::logicalToPhysical(int64_t logical) {
        auto logMin = m_control->getLogicalMinimum();
        auto logMax = m_control->getLogicalMaximum();
        auto phyMin = m_control->getPhysicalMinimum();
        auto phyMax = m_control->getPhysicalMaximum();

        if(phyMin == 0 && phyMax == 0){
            phyMin = logMin;
            phyMax = logMax;
        }

        auto unitExp = m_control->getUnitExponent();

        auto logicalRange = static_cast<double>(logMax - logMin);
        auto physicalRange = static_cast<double>(phyMax - phyMin);

        auto resolution = logicalRange / physicalRange * std::pow(10., unitExp);

        return logical * resolution;
    }

    int64_t Usage::convertLogicalValue(uint32_t value, unsigned int size) {
        uint32_t one32 = 1;
        auto highestBitMask = one32 << (size - 1);
        auto sign = value & highestBitMask;
        if(sign){
            for(unsigned i = size; i < sizeof(value) * 8; ++i){
                value |= one32 << i;
            }
            auto signedValue = static_cast<int32_t>(value);
            return signedValue;
        }

        return static_cast<int64_t>(value);
    }

}
}
