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
        QLabel* deviceName;
        QTreeView* content;

    public slots:

        void openDeviceSelector();

        void selectDevice(const libhidx::Interface&);

    };
}


#endif //HIDVIZ_WINDOW_HH
