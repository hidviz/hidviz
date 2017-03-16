#ifndef HIDVIZ_WINDOW_HH
#define HIDVIZ_WINDOW_HH

#include "libhidx/Interface.hh"

#include <QWidget>

#include <string>

class QLabel;

namespace hidviz {

    class Window : public QWidget {
    Q_OBJECT

    public:
        Window();

    private:
        QLabel* deviceName;

    public slots:

        void openDeviceSelector();

        void selectDevice(const libhidx::Interface&);

    };
}


#endif //HIDVIZ_WINDOW_HH
