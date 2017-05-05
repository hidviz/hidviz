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

#include <QtCore/QPropertyAnimation>
#include "ItemWidget.hh"

#include "ui_ItemWidget.h"

namespace hidviz {
namespace hid {

    ItemWidget::ItemWidget(size_t level, QWidget* parent) : QWidget{parent}, ui{new Ui::ItemWidget} {

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
