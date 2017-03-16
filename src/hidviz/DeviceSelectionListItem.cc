#include "DeviceSelectionListItem.hh"

hidviz::DeviceSelectionListItem::DeviceSelectionListItem(const QString& text,
                                                         const libhidx::Interface& interface) : QListWidgetItem{text}, m_interface{interface} {

}
