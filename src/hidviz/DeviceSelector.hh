//
// Created by ondrej on 2.3.17.
//

#ifndef HIDVIZ_DEVICESELECTOR_HH
#define HIDVIZ_DEVICESELECTOR_HH


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
        void deviceSelected(std::string);

    public slots:

        void selectDevice();

    };
}


#endif //HIDVIZ_DEVICESELECTOR_HH
