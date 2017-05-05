//
// hidviz - Tool for in-depth analysis of USB HID devices communication
// Copyright (C) 2017 Ondrej Budai <ondrej@budai.cz>
//
// This file is part of hidviz.
//
// hidviz is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// hidviz is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with hidviz.  If not, see <http://www.gnu.org/licenses/>.
//

#include "DeviceView.hh"

#include "hid/ControlWidget.hh"
#include "src/hid/CollectionWidget.hh"

#include <libhidx/hid/Control.hh>
#include <libhidx/hid/Collection.hh>
#include <libhidx/Interface.hh>
#include <libhidx/Parser.hh>
#include <libhidx/InterfaceHandle.hh>

#include <QGridLayout>
#include <QMessageBox>

namespace hidviz {

    DeviceView::DeviceView(libhidx::Interface& interface, QWidget *parent) : QWidget{parent}, m_interface{interface} {
        m_layout = new QGridLayout{this};

        libhidx::hid::Item* rootItem = nullptr;

        try {
            rootItem = &m_interface.getParsedHidReportDesc();
        } catch(libhidx::ParserError& e){
            QMessageBox::critical(this, "Parsing HID descriptor failed", e.what());
            return;
        } catch(libhidx::ConnectionException& e) {
            QMessageBox::critical(this, "Retrieving HID descriptor failed", e.what());
            return;
        }

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

    void DeviceView::addItem(libhidx::hid::Item* item, hid::ItemWidget* parent) {

        hid::ItemWidget* itemWidget = nullptr;

        auto control = dynamic_cast<libhidx::hid::Control*>(item);
        auto collection = dynamic_cast<libhidx::hid::Collection*>(item);

        if (control) {
            auto controlWidget = new hid::ControlWidget{control};

            connect(controlWidget, &hid::ControlWidget::dataUpdated, this, &DeviceView::sendData);
            itemWidget = controlWidget;
        } else if(collection) {
            itemWidget = new hid::CollectionWidget{collection};
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
            auto control = dynamic_cast<hid::ControlWidget*>(item);
            if(control) {
                control->updateData();
            }
        }
    }

    void DeviceView::sendData() {
        m_interface.sendData();
    }

    DeviceView::~DeviceView() {
    }

}
