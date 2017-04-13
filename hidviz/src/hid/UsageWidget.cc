#include "UsageWidget.hh"

#include "ui_UsageWidget.h"

#include <libhidx/hid/Usage.hh>
#include <libhidx/hid/Control.hh>

#include <QLineEdit>

namespace hidviz {
namespace hid {
    UsageWidget::UsageWidget(libhidx::hid::Usage& usage, QWidget* parent) : QFrame{parent}, ui{new Ui::UsageWidget}, m_usage{usage}, m_control{usage.getControl()} {
        ui->setupUi(this);

        ui->name->setText(QString::fromStdString(usage.getName()));

        auto reportType = m_control.getReportType();

        if(reportType == libhidx::hid::Control::Type::INPUT){
            m_label = new QLabel();
            ui->contentLayout->addWidget(m_label);
        } else if(reportType == libhidx::hid::Control::Type::OUTPUT){
            m_lineEdit = new QLineEdit{};
            ui->contentLayout->addWidget(m_lineEdit);
            connect(m_lineEdit, &QLineEdit::editingFinished, [this, usage]{
                auto text = m_lineEdit->text().toStdString();
                m_usage.setLogicalValueFromUser(text);
                emit dataUpdated();
            });
        }

        updateData();
    }

    UsageWidget::~UsageWidget() {
        delete ui;
    }

    void UsageWidget::updateData() {
        if(m_control.getReportType() != libhidx::hid::Control::Type::INPUT){
            return;
        }

        auto value = m_usage.getPhysicalValue();
        m_label->setText(QString::number(value));
    }
}
}

