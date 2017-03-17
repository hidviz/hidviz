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
        size_t columnCount() const;
        Item* parentItem();
        int data(int column) const;
        size_t row() const;

    private:
        std::vector<std::unique_ptr<Item>> m_children;
        Item* m_parent;
    };
}
}


#endif //HIDVIZ_ITEM_HH
