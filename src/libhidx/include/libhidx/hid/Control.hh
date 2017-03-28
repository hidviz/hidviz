#ifndef LIBHIDX_CONTROL_HH
#define LIBHIDX_CONTROL_HH

#include <mutex>
#include "Item.hh"
#include "Usage.hh"

namespace libhidx {
    class Parser;
namespace hid {
    class Control : public Item {
        friend class libhidx::Parser;

    public:
        Control(Item* parent = nullptr) : Item{parent} {m_control=true;}

        Item* clone(Item* parent = nullptr, Item* dst = nullptr) const override;
        void update(const std::vector<unsigned char>& rawData);


        enum class Type {
            INPUT = 0,
            OUTPUT  = 1,
            FEATURE = 2
        };

        auto getReportType(){return m_reportType;}
        const auto& getUsages(){return m_usages;}

    private:
        std::size_t m_offset;
        std::vector<Usage> m_usages{};
        uint32_t m_flags;
        Type m_reportType;
        unsigned m_reportSize;
        unsigned m_reportCount;
        int32_t m_logicalMinimum;
        int32_t m_logicalMaximum;
        int32_t m_physicalMinimum;
        int32_t m_physicalMaximum;
        int32_t m_unitExponent;
        unsigned m_unit;

        uint32_t extractData(const std::vector<unsigned char> &vector);

        uint32_t extractVariableUsageData(uint32_t data, unsigned index);

        Usage* findUsageByValue(uint32_t value);
    };
}
}


#endif //LIBHIDX_CONTROL__HH
