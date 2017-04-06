#ifndef LIBHIDX_COLLECTION_HH
#define LIBHIDX_COLLECTION_HH

#include "Item.hh"

namespace libhidx {
namespace hid {
    class Collection : public Item {

    public:
        Collection(Item* parent = nullptr) : Item{parent} {}

        uint32_t m_type;
        unsigned m_usage;
    };
}
}


#endif //LIBHIDX_COLLECTION_HH
