#ifndef HIDVIZ_DEVICEVIEW_HH
#define HIDVIZ_DEVICEVIEW_HH

#include <QWidget>

namespace libhidx {
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
    public:
        DeviceView(libhidx::hid::Item *model, QWidget *parent = nullptr);
        void updateData();

    private:
        void addItem(libhidx::hid::Item *item, unsigned currentDepth);

        libhidx::hid::Item* m_model;
        unsigned int m_depth;
        QGridLayout *m_layout;


        std::vector<hid::Item*> m_items;
    };
}


#endif //HIDVIZ_DEVICEVIEW_HH
