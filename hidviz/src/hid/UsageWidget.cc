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

#include "UsageWidget.hh"

#include "ui_UsageWidget.h"

#include "../Global.hh"

#include <libhidx/hid/Usage.hh>
#include <libhidx/hid/Control.hh>

#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QSettings>

namespace hidviz {
namespace hid {
    UsageWidget::UsageWidget(libhidx::hid::Usage& usage, QWidget* parent)
        : QFrame{parent}, ui{new Ui::UsageWidget}, m_usage{usage}, m_control{usage.getControl()} {
        ui->setupUi(this);

        if(!m_control.isBinary()) {
            ui->name->setText(QString::fromStdString(usage.getName()));
        } else {
            delete ui->name;
            ui->name = nullptr;
        }

        auto reportType = m_control.getReportType();

        if(reportType == libhidx::hid::Control::Type::INPUT){
            initInput();
        } else if(reportType == libhidx::hid::Control::Type::OUTPUT){
            initOutput();
        }

        auto intValue = static_cast<int>(usage.getPhysicalValue());
        m_clampMinValue = m_clampMaxValue = intValue;

        updateData();
    }

    void UsageWidget::initOutput() {
        if(m_control.isBinary()){
            initBinaryButton(true);
            connect(m_button, &QPushButton::clicked, [this](auto newState){
                m_usage.setLogicalValue(newState);
                emit this->dataUpdated();
            });
            return;
        }
        m_lineEdit = new QLineEdit{};
        ui->contentLayout->addWidget(m_lineEdit);
        connect(m_lineEdit, &QLineEdit::editingFinished, [this]{
            auto text = m_lineEdit->text().toStdString();
            m_usage.setLogicalValueFromUser(text);
            emit dataUpdated();
        });
    }

    void UsageWidget::initInput() {
        if(m_control.isBinary()){
            initBinaryButton(false);
            return;
        }

        m_progressBar = new QProgressBar();
        m_progressBar->setFormat("%v");
        ui->contentLayout->addWidget(m_progressBar);
    }

    void UsageWidget::initBinaryButton(bool enabled) {
        m_button = new QPushButton();
        m_button->setEnabled(enabled);
        m_button->setCheckable(true);
        m_button->setStyleSheet("QPushButton{color: white;background-color: #d9534f;}:checked{background-color: #5cb85c;}");
        m_button->setText(QString::fromStdString(m_usage.getName()));
        ui->contentLayout->addWidget(m_button);
    }

    UsageWidget::~UsageWidget() {
        delete ui;
    }

    void UsageWidget::updateData() {
        if(m_control.getReportType() != libhidx::hid::Control::Type::INPUT){
            return;
        }
        QSettings settings{Global::appName};

        auto value = m_usage.getPhysicalValue();

        if(m_control.isBinary()){
            m_button->setChecked(value != 0);
        } else {
            int intValue = static_cast<int>(value);
            m_clampMinValue = std::min(m_clampMinValue, intValue);
            m_clampMaxValue = std::max(m_clampMaxValue, intValue);

            if(settings.value(Global::Settings::clampValues).toBool()) {
                m_progressBar->setMinimum(m_clampMinValue);
                m_progressBar->setMaximum(m_clampMaxValue);
            } else {
                m_progressBar->setMinimum(m_usage.getControl().getLogicalMinimum());
                m_progressBar->setMaximum(m_usage.getControl().getLogicalMaximum());
            }
            m_progressBar->setValue(intValue);
        }


        if(!m_control.isVariable()){
            auto oldState = isVisible();
            if(!value && settings.value(Global::Settings::hideInactiveUsages).toBool()){
                hide();
            } else {
                show();
            }
            if(oldState != isVisible()){
                // if visibility has changed we need to recalculate layout
                updateGeometry();
            }
        }
    }
}
}

