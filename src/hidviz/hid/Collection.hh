#ifndef HIDVIZ_COLLECTION_HH
#define HIDVIZ_COLLECTION_HH

#include "Control.hh"

namespace libhidx {
namespace hid {
    class Collection;
}
}

namespace hidviz {
namespace hid {
    class Collection : public Item {
    public:
        Collection(libhidx::hid::Collection* collection);
    private:
        libhidx::hid::Collection* m_collection;
    };
}
}


#endif //HIDVIZ_COLLECTION_HH
