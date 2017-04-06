#include "Item.hh"

#include "ui_Item.h"

namespace hidviz {
namespace hid {

    Item::Item() : QWidget{}, ui{new Ui::Item} {

        ui->setupUi(this);
        connect(ui->visibleCheckbox, &QCheckBox::stateChanged, [this](int newState){
            if(newState == Qt::Checked){
                for(auto widget: m_children) {
                    widget->show();
                }
            } else if(newState == Qt::Unchecked){
                for(auto widget: m_children) {
                    widget->hide();
                }
            }
        });
    }

    void Item::appendWidget(QWidget *widget) {
        auto nextRow = ui->mainLayout->rowCount();
        ui->mainLayout->addWidget(widget, nextRow, 1);
        m_children.append(widget);
    }

    void Item::setName(const QString &name) {
        ui->name->setText(name);
    }

    void Item::setUsage(const QString &usage) {
        ui->usage->setText(usage);
    }

    Item::~Item() {
        delete ui;
    }
}
}
