#include "DeviceSelector.hh"

#include "libhidx/LibHidx.hh"

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

        auto lib = libhidx::LibHidx{};
        auto devices = lib.enumerateDevices();
        for (const auto& device: devices) {
            auto strings = device.getStrings();
            auto str = strings.manufacturer + " " + strings.product;
            listWidget->addItem(str.c_str());
        }

        setLayout(layout);
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
