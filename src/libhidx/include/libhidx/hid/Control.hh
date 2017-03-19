#ifndef LIBHIDX_CONTROL_HH
#define LIBHIDX_CONTROL_HH

#include "Item.hh"

namespace libhidx {
namespace hid {
    class Control : public Item {

    public:
        Control(Item* parent = nullptr) : Item{parent} {m_control=true;}

        std::size_t m_offset;
        std::vector<unsigned> m_usages{};
        uint32_t m_flags;
        uint8_t m_reportType;
        unsigned m_reportSize;
        unsigned m_reportCount;
        int32_t m_logicalMinimum;
        int32_t m_logicalMaximum;
        int32_t m_physicalMinimum;
        int32_t m_physicalMaximum;
        int32_t m_unitExponent;
        unsigned m_unit;

        enum Type {
            INPUT = 0,
            OUTPUT  = 1,
            FEATURE = 2
        };
    };
}
}


#endif //LIBHIDX_CONTROL__HH
