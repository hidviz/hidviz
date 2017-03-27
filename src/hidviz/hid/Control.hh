#ifndef HIDVIZ_CONTROL_HH
#define HIDVIZ_CONTROL_HH

#include "Item.hh"

namespace libhidx {
namespace hid {
    class Control;
}
}

namespace hidviz {
namespace hid {
    class Control : public Item {
    public:
        Control(libhidx::hid::Control*);
        void updateData();
    private:
        libhidx::hid::Control* m_control;
        std::vector<QLabel*> m_valueLabels;

    };
}
}


#endif //HIDVIZ_CONTROL_HH
