#include "CollectionWidget.hh"

#include <libhidx/hid/Collection.hh>
#include <libhidx/Usages.hh>

#include <QLabel>

namespace hidviz {
namespace hid {

    CollectionWidget::CollectionWidget(libhidx::hid::Collection* collection) : ItemWidget{}, m_collection{collection} {
        const char* types[] = {
            "Physical", "Application", "Logical", "Report", "Named Array",
            "Usage Switch", "Usage modifier"
        };

        auto name = QString {"Collection ("};
        name += types[m_collection->m_type];
        name += ")";

        setName(name);

        auto usageText = getHidUsageText(collection->m_usage);

        setUsage(QString::fromStdString(usageText));
    }
}
}
