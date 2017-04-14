#include "UsageWidget.hh"

#include "ui_UsageWidget.h"

#include <libhidx/hid/Usage.hh>
#include <libhidx/hid/Control.hh>

#include <QLineEdit>
#include <QPushButton>

namespace hidviz {
namespace hid {
    UsageWidget::UsageWidget(libhidx::hid::Usage& usage, QWidget* parent) : QFrame{parent}, ui{new Ui::UsageWidget}, m_usage{usage}, m_control{usage.getControl()} {
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

        m_label = new QLabel();
        ui->contentLayout->addWidget(m_label);
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

        auto value = m_usage.getPhysicalValue();

        if(m_control.isBinary()){
            m_button->setChecked(value != 0);
        } else {
            m_label->setText(QString::number(value));
        }


        if(!m_control.isVariable()){
            auto oldState = isVisible();
            if(!value && m_hideInactive){
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

    void UsageWidget::updateVisibilitySettings(bool hideInactive) {
        m_hideInactive = hideInactive;
        updateData();
    }
}
}

