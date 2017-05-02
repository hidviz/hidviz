#ifndef HIDVIZ_WINDOW_HH
#define HIDVIZ_WINDOW_HH

#include <QWidget>

#include <memory>

namespace libhidx {
    class Interface;
    class LibHidx;
}

namespace Ui {
    class Window;
}

namespace hidviz {

    class DeviceView;

    class Window : public QWidget {
    Q_OBJECT

    public:
        Window();
        ~Window() override;

    protected:
        void closeEvent(QCloseEvent* event) override;

    private:
        Ui::Window* ui = nullptr;
        DeviceView *m_deviceView = nullptr;
        libhidx::Interface* m_selectedInterface = nullptr;
        std::unique_ptr<libhidx::LibHidx> m_lib;

    signals:
        void dataRead();

    public slots:
        void openDeviceSelector();
        void selectDevice(libhidx::Interface&);
        void clearModel();
        void updateSettings();

        libhidx::LibHidx* getLibhidx();
        void loadSettings();
    };
}


#endif //HIDVIZ_WINDOW_HH
