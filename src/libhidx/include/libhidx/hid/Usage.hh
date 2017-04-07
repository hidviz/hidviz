#ifndef HIDVIZ_USAGE_H
#define HIDVIZ_USAGE_H

#include <cstdint>
#include <string>

namespace libhidx {
namespace hid {

    class Usage {
    public:
        Usage(unsigned usageId);

        auto getId() const { return m_id;}
        const auto &getName() const { return m_name; }

        void setLogicalValue(uint32_t data) { m_logicalValue = data; }
        auto getLogicalValue() const { return m_logicalValue; }

        bool setLogicalValueFromUser(const std::string& dataString);

    private:
        unsigned m_id = 0;
        std::string m_name;
        uint32_t m_logicalValue = 0;
    };
}
}


#endif //HIDVIZ_USAGE_H
