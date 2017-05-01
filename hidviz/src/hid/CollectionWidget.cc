#include "CollectionWidget.hh"

#include <libhidx/hid/Collection.hh>

#include <QLabel>

namespace hidviz {
namespace hid {

    CollectionWidget::CollectionWidget(libhidx::hid::Collection* collection, QWidget* parent) : ItemWidget{collection->getLevel(), parent}, m_collection{collection} {

        auto name = QString {"Collection ("};
        name += QString::fromStdString(m_collection->getTypeStr());
        name += ")";

        setName(name);

        auto usageText = collection->getUsageStr();

        setUsage(QString::fromStdString(usageText));
    }
}
}
