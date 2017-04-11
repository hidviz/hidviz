#include "DeviceView.hh"

#include "hid/Control.hh"
#include "hid/Collection.hh"

#include <libhidx/hid/Control.hh>
#include <libhidx/hid/Collection.hh>
#include <libhidx/Interface.hh>

#include <QGridLayout>

namespace hidviz {

    DeviceView::DeviceView(libhidx::Interface& interface, QWidget *parent) : QWidget{parent}, m_interface{interface} {
        m_layout = new QGridLayout{this};

        const auto& rootItem = m_interface.getHidReportDesc();

        for(unsigned i = 0; i < rootItem->childCount(); ++i){
            auto child = rootItem->child(i);
            addItem(child, nullptr);
        }

        connect(this, &DeviceView::dataRead, this, &DeviceView::updateData);
        interface.setReadingListener([this]{emit dataRead();});
        interface.beginReading();

        auto w = new QWidget{};
        w->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        m_layout->addWidget(w, m_layout->rowCount(), 0);
    }

    void DeviceView::addItem(libhidx::hid::Item *item, hid::Item *parent) {

        hid::Item* itemWidget = nullptr;

        auto control = dynamic_cast<libhidx::hid::Control*>(item);
        auto collection = dynamic_cast<libhidx::hid::Collection*>(item);

        if (control) {
            auto controlWidget = new hid::Control{control};

            connect(controlWidget, &hid::Control::dataUpdated, this, &DeviceView::sendData);
            itemWidget = controlWidget;
        } else if(collection) {
            itemWidget = new hid::Collection{collection};
        }

        if(parent){
            parent->appendWidget(itemWidget);
        } else {
            m_layout->addWidget(itemWidget);
        }

        if(itemWidget){
            m_items.emplace_back(itemWidget);
        }

        for (unsigned i = 0; i < item->childCount(); ++i) {
            auto child = item->child(i);
            addItem(child, itemWidget);
        }
    }

    void DeviceView::updateData() {
        for(auto item: m_items){
            auto control = dynamic_cast<hid::Control*>(item);
            if(control) {
                control->updateData();
            }
        }
    }

    void DeviceView::sendData() {
        m_interface.sendData();
    }

    DeviceView::~DeviceView() {
        m_interface.setReadingListener(nullptr);
        m_interface.stopReading();
    }

}
