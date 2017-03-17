#include "TreeModel.hh"

#include "libhidx/hid/Item.hh"

using libhidx::hid::Item;

namespace hidviz {

    TreeModel::TreeModel(Item* root) : m_rootItem{root}{}

    TreeModel::~TreeModel() {
        delete m_rootItem;
    }

    int TreeModel::columnCount(const QModelIndex& parent) const {
        if (parent.isValid())
            return static_cast<Item*>(parent.internalPointer())->columnCount();
        else
            return m_rootItem->columnCount();
    }

    QVariant TreeModel::data(const QModelIndex& index, int role) const {
        if (!index.isValid())
            return QVariant();

        if (role != Qt::DisplayRole)
            return QVariant();

        Item* item = static_cast<Item*>(index.internalPointer());

        return item->data(index.column());
    }

    Qt::ItemFlags TreeModel::flags(const QModelIndex& index) const {
        if (!index.isValid())
            return 0;

        return QAbstractItemModel::flags(index);
    }

    QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
            return m_rootItem->data(section);

        return QVariant();
    }

    QModelIndex TreeModel::index(int row, int column, const QModelIndex& parent)
    const {
        if (!hasIndex(row, column, parent))
            return QModelIndex();

        Item* parentItem;

        if (!parent.isValid())
            parentItem = m_rootItem;
        else
            parentItem = static_cast<Item*>(parent.internalPointer());

        Item* childItem = parentItem->child(row);
        if (childItem)
            return createIndex(row, column, childItem);
        else
            return QModelIndex();
    }

    QModelIndex TreeModel::parent(const QModelIndex& index) const {
        if (!index.isValid())
            return QModelIndex();

        Item* childItem = static_cast<Item*>(index.internalPointer());
        Item* parentItem = childItem->parentItem();

        if (parentItem == m_rootItem)
            return QModelIndex();

        return createIndex(static_cast<int>(parentItem->row()), 0, parentItem);
    }

    int TreeModel::rowCount(const QModelIndex& parent) const {
        Item* parentItem;
        if (parent.column() > 0)
            return 0;

        if (!parent.isValid())
            parentItem = m_rootItem;
        else
            parentItem = static_cast<Item*>(parent.internalPointer());

        return static_cast<int>(parentItem->childCount());
    }


}
