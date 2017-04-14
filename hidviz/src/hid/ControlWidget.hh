#ifndef HIDVIZ_CONTROL_HH
#define HIDVIZ_CONTROL_HH

#include "ItemWidget.hh"

namespace Ui {
    class ControlSubWidget;
}

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
        Ui::ControlSubWidget* ui;

        void initGui();
        void initUsages();
        void initInfoTable();

    signals:
        void dataUpdated();
        void hideInactiveUsagesChanged(bool newState);


    };
}
}


#endif //HIDVIZ_CONTROL_HH
