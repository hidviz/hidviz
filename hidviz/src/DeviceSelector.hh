#ifndef HIDVIZ_DEVICESELECTOR_HH
#define HIDVIZ_DEVICESELECTOR_HH

#include <QDialog>

namespace libhidx {
    class LibHidx;
    class Interface;
}

namespace Ui {
    class DeviceSelector;
}

namespace hidviz {

    class DeviceSelector : public QDialog {
    Q_OBJECT
    public:
        explicit DeviceSelector(libhidx::LibHidx& lib);
        ~DeviceSelector() override;
    private:
        Ui::DeviceSelector* ui;
        libhidx::LibHidx& m_lib;

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
