#ifndef HIDVIZ_WINDOW_HH
#define HIDVIZ_WINDOW_HH

#include <QWidget>

#include <string>

namespace hidviz {

    class Window : public QWidget {
    Q_OBJECT

    public:
        Window();

    public slots:

        void openDeviceSelector();

        void selectDevice(std::string);

    };
}


#endif //HIDVIZ_WINDOW_HH
