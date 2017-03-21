#ifndef HIDVIZ_DEVICESELECTIONLISTITEM_HH
#define HIDVIZ_DEVICESELECTIONLISTITEM_HH

#include <QListWidgetItem>
#include "libhidx/Interface.hh"

namespace hidviz {

    class DeviceSelectionListItem : public QListWidgetItem {
    public:
        DeviceSelectionListItem(const QString &text, libhidx::Interface& interface);
        auto& getInterface(){return m_interface;}

    private:
        libhidx::Interface& m_interface;

    };
}



#endif //HIDVIZ_DEVICESELECTIONLISTITEM_HH
