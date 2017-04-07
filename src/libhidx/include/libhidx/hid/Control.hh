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
        Control(Item* parent = nullptr) : Item{parent} {}
        void setData(const std::vector<unsigned char> &rawData);
        uint32_t getData() const;


        enum class Type {
            INPUT = 0,
            OUTPUT  = 1,
            FEATURE = 2
        };

        auto getReportType() const {return m_reportType;}
        const auto& getUsages() const {return m_usages;}
        auto getOffset() const {return m_offset;};
        auto getSize() const {return m_reportSize;}
        auto getCount() const {return m_reportCount;}
        auto getLogicalMinimum() const {return m_logicalMinimum;}
        auto getLogicalMaximum() const {return m_logicalMaximum;}
        auto getPhysicalMinimum() const {return m_physicalMinimum;}
        auto getPhysicalMaximum() const {return m_physicalMaximum;}
        auto getUnitExponent() const {return m_unitExponent;}

    private:
        std::size_t m_offset = 0;
        std::vector<std::unique_ptr<Usage>> m_usages{};
        uint32_t m_flags = 0;
        Type m_reportType;
        unsigned m_reportSize = 0;
        unsigned m_reportCount = 0;
        int32_t m_logicalMinimum = 0;
        int32_t m_logicalMaximum = 0;
        int32_t m_physicalMinimum = 0;
        int32_t m_physicalMaximum = 0;
        int32_t m_unitExponent = 0;
        unsigned m_unit = 0;

        uint32_t extractData(const std::vector<unsigned char> &vector);

        uint32_t extractVariableUsageData(uint32_t data, unsigned index);

        Usage* findUsageByValue(uint32_t value);
    };
}
}


#endif //LIBHIDX_CONTROL__HH
