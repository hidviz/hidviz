#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include "DeviceSelector.hh"

DeviceSelector::DeviceSelector() : QDialog{} {
    setModal(true);
    setWindowTitle("Device selector");
    auto layout = new QVBoxLayout{};

    layout->addWidget(new QLabel{"Select device from following listWidget:"});
    listWidget = new QListWidget{};
    listWidget->addItem("A");
    listWidget->addItem("B");
    listWidget->addItem("C");
    listWidget->addItem("D");
    layout->addWidget(listWidget);

    auto submitButton = new QPushButton{"Select"};
    connect(submitButton, &QPushButton::pressed, this,
            &DeviceSelector::selectDevice);
    layout->addWidget(submitButton);

    setLayout(layout);
}

void DeviceSelector::selectDevice() {
    const auto& selectedItems = listWidget->selectedItems();
    if(!selectedItems.size()){
        return;
    }

    auto selectedItem = selectedItems.first()->data(Qt::DisplayRole).toString().toStdString();

    emit deviceSelected(selectedItem);
}