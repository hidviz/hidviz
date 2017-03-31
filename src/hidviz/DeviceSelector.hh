#ifndef HIDVIZ_DEVICESELECTOR_HH
#define HIDVIZ_DEVICESELECTOR_HH

#include "libhidx/Interface.hh"

#include <QDialog>

namespace Ui {
    class DeviceSelector;
}

namespace hidviz {

    class DeviceSelector : public QDialog {
    Q_OBJECT
    public:
        DeviceSelector();
    private:
        Ui::DeviceSelector* ui;

    signals:
        void deviceSelected(libhidx::Interface&);
        void listCleared();

    public slots:

        void selectDevice();
        void initListWidget() const;
        void reloadDevices();
    };
}


#endif //HIDVIZ_DEVICESELECTOR_HH
