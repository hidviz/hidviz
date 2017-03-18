#include "Collection.hh"

#include <QHBoxLayout>
#include <QLabel>
#include <libhidx/hid/Item.hh>

namespace hidviz {
namespace hid {

    Collection::Collection(libhidx::hid::Collection* collection) : Item{}, m_collection{collection} {
    }
}
}
