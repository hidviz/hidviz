#ifndef HIDVIZ_COLLECTION_HH
#define HIDVIZ_COLLECTION_HH

#include "Item.hh"

namespace libhidx {
namespace hid {
    class Collection : public Item {

    public:
        Collection(Item* parent = nullptr) : Item{parent} {}
    };
}
}


#endif //HIDVIZ_COLLECTION_HH
