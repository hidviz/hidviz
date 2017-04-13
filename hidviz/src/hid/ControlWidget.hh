#ifndef HIDVIZ_CONTROL_HH
#define HIDVIZ_CONTROL_HH

#include "ItemWidget.hh"

namespace libhidx {
namespace hid {
    class Control;
    class Usage;
}
}

namespace hidviz {
namespace hid {
    class UsageWidget;
    class ControlWidget : public ItemWidget {
    Q_OBJECT
    public:
        ControlWidget(libhidx::hid::Control*);
        void updateData();
    private:
        libhidx::hid::Control* m_control;
        QList<UsageWidget*> m_usageWidgets;

        void initGui();
        void initDetailInfo();

    signals:
        void dataUpdated();


    };
}
}


#endif //HIDVIZ_CONTROL_HH
