#include "libhidx/hid/Usage.hh"

#include "libhidx/Usages.hh"

#include <stdexcept>
#include <limits>

namespace libhidx{
namespace hid {
    Usage::Usage(unsigned usageId) : m_id{usageId} {
        m_name = getHidUsageText(usageId);

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
}
}
