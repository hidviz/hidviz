#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#include <functional>
#include <memory>

namespace libhidx {
namespace hid {
    class Item;
}
}

namespace hidviz {

    class TreeModel : public QAbstractItemModel {
    Q_OBJECT

    public:
        explicit TreeModel(libhidx::hid::Item*);

        QVariant data(const QModelIndex& index, int role) const override;

        Qt::ItemFlags flags(const QModelIndex& index) const override;

        QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const override;

        QModelIndex index(int row, int column,
                          const QModelIndex& parent = QModelIndex()) const override;

        QModelIndex parent(const QModelIndex& index) const override;

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;

        int
        columnCount(const QModelIndex& parent = QModelIndex()) const override;

        void forEach(std::function<void(const QModelIndex&)>, QModelIndex parent = QModelIndex{});



    private:
        libhidx::hid::Item* m_rootItem;
    };

}

#endif // TREEMODEL_H
