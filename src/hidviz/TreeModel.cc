#include "TreeModel.hh"

#include "libhidx/hid/Item.hh"

#include <QLabel>

#include <iostream>

using libhidx::hid::Item;

namespace hidviz {

    TreeModel::TreeModel(Item* root) : m_rootItem{root}{}

    int TreeModel::columnCount(const QModelIndex&) const {
        return 2;
    }

    QVariant TreeModel::data(const QModelIndex& index, int role) const {
        if (!index.isValid())
            return QVariant();

        Item* item = static_cast<Item*>(index.internalPointer());
        if (role == Qt::UserRole){
            return QVariant::fromValue(static_cast<void*>(item));
        }

        if (role != Qt::DisplayRole)
            return QVariant();

        return "";
    }

    Qt::ItemFlags TreeModel::flags(const QModelIndex& index) const {
        if (!index.isValid())
            return 0;

        return QAbstractItemModel::flags(index);
    }

    QVariant TreeModel::headerData(int, Qt::Orientation, int) const {
        return "";
    }

    QModelIndex TreeModel::index(int row, int column, const QModelIndex& parent)
    const {
        if (!hasIndex(row, column, parent))
            return QModelIndex();

        Item* parentItem;

        if (!parent.isValid())
            parentItem = m_rootItem.get();
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

        if (parentItem == m_rootItem.get())
            return QModelIndex();

        return createIndex(static_cast<int>(parentItem->row()), 0, parentItem);
    }

    int TreeModel::rowCount(const QModelIndex& parent) const {
        Item* parentItem;
        if (parent.column() > 0) {
            return 0;
        }

        if (!parent.isValid())
            parentItem = m_rootItem.get();
        else
            parentItem = static_cast<Item*>(parent.internalPointer());

        auto rowCount = static_cast<int>(parentItem->childCount());

        return rowCount;
    }

    void TreeModel::forEach(std::function<void(const QModelIndex&)> f,
                            QModelIndex parent) {
        if (!parent.isValid()) {
            parent = index(0, 0, QModelIndex());
            auto numRows = rowCount();
            auto numCols = columnCount();

            for(int i = 0; i < numRows; ++i){
                for(int j = 0; j < numCols; ++j){
                    f(index(i, j));
                }
            }
        } else {
            f(parent);
        }

        auto numRows = rowCount(parent);
        auto numCols = columnCount(parent);

        for(int i = 0; i < numRows; ++i) {
            for(int j = 0; j < numCols; ++j){
                forEach(f,index(i, j, parent));
            }
        }
    }

}
