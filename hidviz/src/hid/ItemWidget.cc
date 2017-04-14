#include <QtCore/QPropertyAnimation>
#include "ItemWidget.hh"

#include "ui_ItemWidget.h"

namespace hidviz {
namespace hid {

    ItemWidget::ItemWidget(size_t level) : QWidget{}, ui{new Ui::ItemWidget} {

        ui->setupUi(this);
        connect(ui->visibleCheckbox, &QPushButton::toggled, [this](bool newState){
            if(newState){
                for(auto widget: m_children) {
                    widget->show();
                }
            } else {
                for(auto widget: m_children) {
                    widget->hide();
                }
            }
        });

        auto color = 0xffffff - 0x111111 * level;

        ui->header->setStyleSheet("background-color: #" + QString::number(color, 16));
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
