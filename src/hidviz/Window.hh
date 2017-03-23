#ifndef HIDVIZ_WINDOW_HH
#define HIDVIZ_WINDOW_HH

#include "libhidx/Interface.hh"

#include <QWidget>

#include <string>

class QLabel;
class QTreeView;

namespace hidviz {

    class Window : public QWidget {
    Q_OBJECT

    public:
        Window();

    protected:
        void closeEvent(QCloseEvent* event) override;

    private:
        QTreeView* content = nullptr;
        QLabel* deviceName = nullptr;
        libhidx::Interface* m_selectedInterface = nullptr;

    public slots:
        void openDeviceSelector();
        void selectDevice(libhidx::Interface&);

    };
}


#endif //HIDVIZ_WINDOW_HH
