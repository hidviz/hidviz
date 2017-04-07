#ifndef HIDVIZ_USAGE_H
#define HIDVIZ_USAGE_H

#include <cstdint>
#include <string>

namespace libhidx {
namespace hid {

    class Control;

    class Usage {
    public:
        Usage(unsigned usageId, Control* control);

        auto getId() const { return m_id;}
        const auto &getName() const { return m_name; }

        auto getLogicalValue() const { return m_logicalValue; }
        void setLogicalValue(uint32_t logicalValue);
        bool setLogicalValueFromUser(const std::string& dataString);

        auto getPhysicalValue() const {return m_physicalValue;}


    private:
        unsigned m_id = 0;
        std::string m_name;
        Control* m_control = nullptr;

        uint32_t m_logicalValue = 0;
        double m_physicalValue = 0;

        double logicalToPhysical(uint32_t);
        //TODO uint32_t physicalToLogical(double);
    };
}
}


#endif //HIDVIZ_USAGE_H
