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
        CollectionWidget(libhidx::hid::Collection* collection);
    private:
        libhidx::hid::Collection* m_collection;
    };
}
}


#endif //HIDVIZ_COLLECTION_HH
