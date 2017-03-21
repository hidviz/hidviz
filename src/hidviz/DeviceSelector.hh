#ifndef HIDVIZ_DEVICESELECTOR_HH
#define HIDVIZ_DEVICESELECTOR_HH

#include "libhidx/Interface.hh"

#include <QDialog>

class QListWidget;

namespace hidviz {

    class DeviceSelector : public QDialog {
    Q_OBJECT
    private:
        QListWidget* listWidget;
    public:
        DeviceSelector();

    signals:
        void deviceSelected(libhidx::Interface&);

    public slots:

        void selectDevice();

        void initListWidget() const;
    };
}


#endif //HIDVIZ_DEVICESELECTOR_HH
