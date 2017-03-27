#include "libhidx/hid/Control.hh"

namespace libhidx {
namespace hid {
    Item* Control::clone(Item* parent, Item* dst) const {
        if(!dst){
            dst = new Control{};
        }
        auto it = static_cast<Control*>(Item::clone(parent, dst));
        it->m_offset = m_offset;
        it->m_usages = m_usages;
        it->m_flags = m_flags;
        it->m_reportType = m_reportType;
        it->m_reportSize = m_reportSize;
        it->m_reportCount = m_reportCount;
        it->m_logicalMinimum = m_logicalMinimum;
        it->m_logicalMaximum = m_logicalMaximum;
        it->m_physicalMinimum = m_physicalMinimum;
        it->m_physicalMaximum = m_physicalMaximum;
        it->m_unitExponent = m_unitExponent;
        it->m_unit = m_unit;

        return it;
    }

    void Control::update(const std::vector<unsigned char>& vector) {
        std::lock_guard<std::mutex> lock{m_dataMutex};
        auto length = m_reportCount * m_reportSize;
        m_data = 0;

        for(unsigned i = 0; i < length; ++i){
            auto originPos = m_offset + i;
            auto bytePos = originPos / 8;
            auto bitPos = originPos % 8;

            unsigned char byte = vector[bytePos];
            byte >>= bitPos;
            byte &= 1;
            m_data |= byte << i;
        }
    }

    uint32_t Control::getData() {
        std::lock_guard<std::mutex> lock{m_dataMutex};

        return m_data;
    }
}
}
