#include "libhidx/hid/Collection.hh"

namespace libhidx {
namespace hid {
    Item* Collection::clone(Item* parent, Item* dst) const{
        if(!dst){
            dst = new Collection{};
        }

        auto it = static_cast<Collection*>(Item::clone(parent, dst));
        it->m_type = m_type;
        it->m_usage = m_usage;

        return it;
    }
}
}
