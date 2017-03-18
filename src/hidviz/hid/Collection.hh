#ifndef HIDVIZ_COLLECTION_HH
#define HIDVIZ_COLLECTION_HH

#include <QWidget>

namespace libhidx {
namespace hid {
    class Item;
}
}

namespace hidviz {
namespace hid {
    class Collection : public QWidget {
    public:
        Collection(libhidx::hid::Item* item);
    private:
        libhidx::hid::Item* m_item;
    };
}
}


#endif //HIDVIZ_COLLECTION_HH
