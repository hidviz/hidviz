#ifndef HIDVIZ_WINDOW_HH
#define HIDVIZ_WINDOW_HH

#include "libhidx/Interface.hh"

#include <QWidget>

#include <string>

class QLabel;
class QTreeView;

namespace hidviz {

    class TreeModel;

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
        TreeModel* m_model;

    signals:
        void dataRead();

    public slots:
        void openDeviceSelector();
        void selectDevice(libhidx::Interface&);
        void updateData();

    };
}


#endif //HIDVIZ_WINDOW_HH
