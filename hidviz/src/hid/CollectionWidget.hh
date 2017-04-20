#ifndef HIDVIZ_COLLECTION_HH
#define HIDVIZ_COLLECTION_HH

#include "ItemWidget.hh"

namespace libhidx {
namespace hid {
    class Collection;
}
}

namespace hidviz {
namespace hid {
    class CollectionWidget : public ItemWidget {
    public:
        explicit CollectionWidget(libhidx::hid::Collection* collection, QWidget* parent = nullptr);
    private:
        libhidx::hid::Collection* m_collection;
    };
}
}


#endif //HIDVIZ_COLLECTION_HH
