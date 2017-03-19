#include "Collection.hh"

#include "libhidx/hid/Collection.hh"
#include "libhidx/Usages.hh"

#include <QLabel>

namespace hidviz {
namespace hid {

    Collection::Collection(libhidx::hid::Collection* collection) : Item{}, m_collection{collection} {
        const char* types[] = {
            "Physical", "Application", "Logical", "Report", "Named Array",
            "Usage Switch", "Usage modifier"
        };

        auto name = QString {"Collection ("};
        name += types[m_collection->m_type];
        name += ")";

        m_name->setText(name);

        auto usageText = getHidUsageText(collection->m_usage);

        m_usage->setText(QString::fromStdString(usageText));
    }
}
}
