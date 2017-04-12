#include "DeviceSelector.hh"

#include "ui_DeviceSelector.h"

#include "DeviceSelectionListItem.hh"

#include <libhidx/LibHidx.hh>
#include <libhidx/LibHidxFactory.hh>
#include <libhidx/InterfaceHandle.hh>

#include <QMessageBox>

namespace hidviz {

    DeviceSelector::DeviceSelector() : ui{new Ui::DeviceSelector{}} {
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
        auto& lib = libhidx::LibHidxFactory::get();
        if(lib.getDevices().empty()){
            lib.loadDevices();
        }

        const auto& devices = lib.getDevices();
        for (const auto& device: devices) {
            const auto& interfaces = device->getInterfaces();
            for(auto& interface: interfaces){
                if(!interface->isHid()){
                    continue;
                }

                const auto& strings = device->getStrings();
                auto interfaceNum = interface->getNumber();
                auto str = strings.manufacturer + " " + strings.product;

                str += " (interface " + std::to_string(interfaceNum) + ")";

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
            QMessageBox::critical(this, "Connection error", ex.what());
            ui->selectButton->setEnabled(true);
            return;
        }

        close();
        emit deviceSelected(selectedItem->getInterface());
    }

    void DeviceSelector::reloadDevices() {
        emit listCleared();
        ui->deviceList->clear();
        auto& lib = libhidx::LibHidxFactory::get();
        lib.reloadDevices();
        initListWidget();
    }

}
