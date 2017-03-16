#ifndef HIDVIZ_DEVICESELECTIONLISTITEM_HH
#define HIDVIZ_DEVICESELECTIONLISTITEM_HH

#include <QListWidgetItem>
#include "libhidx/Interface.hh"

namespace hidviz {

    class DeviceSelectionListItem : public QListWidgetItem {
    public:
        DeviceSelectionListItem(const QString &text, const libhidx::Interface& interface);
        const auto& getInterface(){return m_interface;}

    private:
        const libhidx::Interface& m_interface;

    };
}



#endif //HIDVIZ_DEVICESELECTIONLISTITEM_HH
