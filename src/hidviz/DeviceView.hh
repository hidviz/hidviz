#ifndef HIDVIZ_DEVICEVIEW_HH
#define HIDVIZ_DEVICEVIEW_HH

#include <QWidget>

namespace libhidx {
    class Interface;
namespace hid {
    class Item;
}
}

class QGridLayout;

namespace hidviz {
    namespace hid {
        class Item;
    }
    class DeviceView : public QWidget {
    Q_OBJECT
    public:
        DeviceView(libhidx::Interface& model, QWidget *parent = nullptr);
        void updateData();

    private:
        void addItem(libhidx::hid::Item *item, hid::Item *parent = nullptr);
        void sendData();

        libhidx::Interface& m_interface;
        QGridLayout* m_layout;


        std::vector<hid::Item*> m_items;

    signals:
        void dataRead();
    };
}


#endif //HIDVIZ_DEVICEVIEW_HH
