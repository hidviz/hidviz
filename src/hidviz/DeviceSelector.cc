#include "DeviceSelector.hh"

#include "ui_DeviceSelector.h"

#include "DeviceSelectionListItem.hh"

#include "libhidx/LibHidx.hh"
#include "libhidx/LibHidxFactory.hh"

namespace hidviz {

    DeviceSelector::DeviceSelector() : ui{new Ui::DeviceSelector{}} {
        ui->setupUi(this);
        initListWidget();
        connect(ui->selectButton, &QPushButton::pressed, this,
                &DeviceSelector::selectDevice);

        connect(ui->reloadButton, &QPushButton::pressed, this, &DeviceSelector::reloadDevices);
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
        const auto& selectedItems = ui->deviceList->selectedItems();
        if (!selectedItems.size()) {
            return;
        }

        auto selectedItem = static_cast<DeviceSelectionListItem*>(selectedItems.first());

        emit deviceSelected(selectedItem->getInterface());
        close();
    }

    void DeviceSelector::reloadDevices() {
        emit listCleared();
        ui->deviceList->clear();
        auto& lib = libhidx::LibHidxFactory::get();
        lib.loadDevices();
        initListWidget();
    }
}
