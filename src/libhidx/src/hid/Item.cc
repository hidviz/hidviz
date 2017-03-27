#include "libhidx/hid/Item.hh"

#include <algorithm>

namespace libhidx {
namespace hid {

    void Item::appendChild(Item* child) {
        m_children.push_back(std::unique_ptr<Item>(child));
    }

    Item* Item::clone(Item* parent, Item* dst) const {
        if(!dst){
            dst = new Item{};
        }

        dst->m_collection = m_collection;
        dst->m_control = m_control;

        for(const auto& child: m_children){
            dst->m_children.emplace_back(child->clone(dst));
        }

        dst->m_parent = parent;

        return dst;
    }

    Item* Item::child(int row) {
        return m_children[row].get();
    }

    size_t Item::childCount() const {
        return m_children.size();
    }

    Item* Item::parentItem() {
        return m_parent;
    }

    size_t Item::row() const {
        const auto& parent = m_parent;
        auto it = std::find_if(
            begin(parent->m_children),
            end(parent->m_children),
            [this](const auto& a){
                return this == a.get();
            }
        );

        return static_cast<size_t>(it - begin(parent->m_children));
    }

    void Item::forEach(std::function<void(Item*)> f) {
        f(this);
        for(const auto& child: m_children){
            child->forEach(f);
        }
    }


}
}
