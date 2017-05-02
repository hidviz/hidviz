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
        class ItemWidget;
    }
    class DeviceView : public QWidget {
    Q_OBJECT
    public:
        explicit DeviceView(libhidx::Interface& model, QWidget *parent = nullptr);
        ~DeviceView() override;
        void updateData();

    private:
        void addItem(libhidx::hid::Item* item, hid::ItemWidget* parent = nullptr);
        void sendData();

        libhidx::Interface& m_interface;
        QGridLayout* m_layout;

        std::vector<hid::ItemWidget*> m_items;

    signals:
        void dataRead();
    };
}


#endif //HIDVIZ_DEVICEVIEW_HH
