#include "ItemWidget.hh"

#include "ui_ItemWidget.h"

namespace hidviz {
namespace hid {

    ItemWidget::ItemWidget() : QWidget{}, ui{new Ui::Item} {

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

    void ItemWidget::appendWidget(QWidget *widget) {
        auto nextRow = ui->mainLayout->rowCount();
        ui->mainLayout->addWidget(widget, nextRow, 1);
        m_children.append(widget);
    }

    void ItemWidget::setName(const QString &name) {
        ui->name->setText(name);
    }

    void ItemWidget::setUsage(const QString &usage) {
        ui->usage->setText(usage);
    }

    ItemWidget::~ItemWidget() {
        delete ui;
    }
}
}
