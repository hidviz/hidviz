#ifndef HIDVIZ_ITEM_HH
#define HIDVIZ_ITEM_HH

#include <vector>
#include <memory>

namespace libhidx {
namespace hid {

    class Item {
    public:
        Item(Item* parent = nullptr) : m_parent{parent}{}
        void appendChild(Item* child);
        Item* child(int row);
        size_t childCount() const;
        Item* parentItem();
        size_t row() const;

        bool m_collection = false;
        bool m_control = false;

    private:
        std::vector<std::unique_ptr<Item>> m_children;
        Item* m_parent;
    };
}
}


#endif //HIDVIZ_ITEM_HH
