#include "DeviceSelector.hh"

#include "libhidx/LibHidx.hh"
#include "libhidx/LibHidxFactory.hh"

#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>

namespace hidviz {

    DeviceSelector::DeviceSelector() : QDialog{} {
        setModal(true);
        setWindowTitle("Device selector");
        auto layout = new QVBoxLayout{};

        layout->addWidget(new QLabel{"Select device from following list:"});
        listWidget = new QListWidget{};
        layout->addWidget(listWidget);

        auto submitButton = new QPushButton{"Select"};
        connect(submitButton, &QPushButton::pressed, this,
                &DeviceSelector::selectDevice);
        layout->addWidget(submitButton);

        initListWidget();

        setLayout(layout);
    }

    void DeviceSelector::initListWidget() const {
        auto& lib = libhidx::LibHidxFactory::get();
        lib.loadDevices();
        const auto& devices = lib.getDevices();
        for (const auto& device: devices) {
            const auto& interfaces = device.getInterfaces();
            for(const auto& interface: interfaces){
                if(!interface.isHid()){
                    continue;
                }

                const auto& strings = device.getStrings();
                auto interfaceNum = interface.getNumber();
                auto str = strings.manufacturer + " " + strings.product;

                str += " (interface " + std::to_string(interfaceNum) + ")";

                listWidget->addItem(str.c_str());
            }
        }
    }

    void DeviceSelector::selectDevice() {
        const auto& selectedItems = listWidget->selectedItems();
        if (!selectedItems.size()) {
            return;
        }

        auto selectedItem = selectedItems.first()->data(
            Qt::DisplayRole).toString().toStdString();

        emit deviceSelected(selectedItem);
    }
}
