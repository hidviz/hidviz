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

#include "DeviceSelector.hh"

#include "ui_DeviceSelector.h"

#include "DeviceSelectionListItem.hh"
#include "WindowsDriverDialog.hh"

#include <libhidx/LibHidx.hh>
#include <libhidx/InterfaceHandle.hh>
#include <libhidx/Device.hh>
#include <libhidx/DeviceStrings.hh>

#include <QMessageBox>

namespace hidviz {

    DeviceSelector::DeviceSelector(libhidx::LibHidx& lib) : ui{new Ui::DeviceSelector{}}, m_lib{lib} {
        ui->setupUi(this);
        initListWidget();
        connect(ui->selectButton, &QPushButton::pressed, this,
                &DeviceSelector::selectDevice);

        connect(ui->reloadButton, &QPushButton::pressed, this, &DeviceSelector::reloadDevices);
    }

    DeviceSelector::~DeviceSelector() {
        delete ui;
    }

    void DeviceSelector::initListWidget() const {
        if(m_lib.getDevices().empty()){
            m_lib.loadDevices();
        }

        const auto& devices = m_lib.getDevices();
        for (const auto& device: devices) {
            const auto& interfaces = device->getInterfaces();
            for(auto& interface: interfaces){
                if(!interface->isHid()){
                    continue;
                }

                auto str = interface->getName();

                auto item = new DeviceSelectionListItem{QString::fromStdString(str), *interface};

                ui->deviceList->addItem(item);
            }
        }
    }

    void DeviceSelector::selectDevice() {
        ui->selectButton->setEnabled(false);
        const auto& selectedItems = ui->deviceList->selectedItems();
        if (!selectedItems.size()) {
            ui->selectButton->setEnabled(true);
            return;
        }

        auto selectedItem = static_cast<DeviceSelectionListItem*>(selectedItems.first());

        try {
            selectedItem->getInterface().getHandle();
        } catch(libhidx::ConnectionException& ex){
#ifdef WIN32
            constexpr bool isWindows = true;
#else
            constexpr bool isWindows = false;
#endif
            if(isWindows && ex.code().value() == -12){
                WindowsDriverDialog dialog;
                dialog.exec();
            } else {
                QMessageBox::critical(this, "Connection error", ex.what());
            }
            ui->selectButton->setEnabled(true);
            return;
        }

        close();
        emit deviceSelected(selectedItem->getInterface());
    }

    void DeviceSelector::reloadDevices() {
        emit listCleared();
        ui->deviceList->clear();
        m_lib.reloadDevices();
        initListWidget();
    }

}
