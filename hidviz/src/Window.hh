#ifndef HIDVIZ_WINDOW_HH
#define HIDVIZ_WINDOW_HH

#include <libhidx/Interface.hh>

#include <QWidget>


namespace Ui {
    class Window;
}

namespace hidviz {

    class DeviceView;

    class Window : public QWidget {
    Q_OBJECT

    public:
        Window();
        ~Window();

    protected:
        void closeEvent(QCloseEvent* event) override;

    private:
        Ui::Window* ui;
        DeviceView *m_deviceView;
        libhidx::Interface* m_selectedInterface;

    signals:
        void dataRead();

    public slots:
        void openDeviceSelector();
        void selectDevice(libhidx::Interface&);
        void updateData();
        void clearModel();

    };
}


#endif //HIDVIZ_WINDOW_HH
