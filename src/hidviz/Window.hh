#ifndef HIDVIZ_WINDOW_HH
#define HIDVIZ_WINDOW_HH

#include "libhidx/Interface.hh"

#include <QWidget>

class QLabel;
class QTreeView;

namespace Ui {
    class Window;
}

namespace hidviz {

    class TreeModel;

    class Window : public QWidget {
    Q_OBJECT

    public:
        Window();
        ~Window();

    protected:
        void closeEvent(QCloseEvent* event) override;

    private:
        libhidx::Interface* m_selectedInterface = nullptr;
        TreeModel* m_model;
        Ui::Window* ui;

    signals:
        void dataRead();

    public slots:
        void openDeviceSelector();
        void selectDevice(libhidx::Interface&);
        void updateData();
    };
}


#endif //HIDVIZ_WINDOW_HH
