#include "Item.hh"

#include "ui_Item.h"

namespace hidviz {
namespace hid {

    Item::Item() : QWidget{}, ui{new Ui::Item} {

        ui->setupUi(this);
        connect(ui->visibleCheckbox, &QCheckBox::stateChanged, [this](int newState){

            if(newState == Qt::Checked){
                ui->content->show();
            } else if(newState == Qt::Unchecked){
                ui->content->hide();
            }
        });
    }

    void Item::setContent(QWidget* widget) {
        delete ui->content;
        ui->mainLayout->addWidget(widget, 1, 1);
        ui->content = widget;
    }

    void Item::setName(const QString &name) {
        ui->name->setText(name);
    }

    void Item::setUsage(const QString &usage) {
        ui->usage->setText(usage);
    }
}
}
