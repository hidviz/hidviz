#include "ControlWidget.hh"

#include "ui_ControlSubWidget.h"
#include "../FlowLayout.hh"
#include "UsageWidget.hh"

#include <libhidx/hid/Control.hh>
#include <libhidx/Usages.hh>

#include <QLabel>
#include <QLineEdit>

namespace hidviz{
namespace hid {
    ControlWidget::ControlWidget(libhidx::hid::Control* control, QWidget* parent) : ItemWidget{control->getLevel(), parent}, m_control{control}, ui{new Ui::ControlSubWidget} {
        initGui();
    }

    void ControlWidget::initGui() {
        const char *types[] = {
            "Input", "Output", "Feature", "Padding"
        };

        auto reportType = m_control->getReportType();
        const auto& usages = m_control->getUsages();

        const char* typeStr;
        if(!usages.size()){
            typeStr = "Padding";
        } else {
            typeStr = types[static_cast<unsigned>(reportType)];
        }

        setUsage("");
        setName(typeStr);

        if(!usages.size()){
            setUsage(QString::number(m_control->getReportCount() * m_control->getReportSize()) + " bits");
            return;
        }

        auto w = new QWidget;
        ui->setupUi(w);
        appendWidget(w);

        initInfoTable();

        initUsages();
    }

    void ControlWidget::initUsages() {
        auto valuesLayout = new FlowLayout{};
        auto& usages = m_control->getUsages();
        for(auto& usage: usages){
            UsageWidget *usageWidget = new UsageWidget{*usage};
            m_usageWidgets.push_back(usageWidget);
            valuesLayout->addWidget(usageWidget);

            connect(usageWidget, &UsageWidget::dataUpdated, this, &ControlWidget::dataUpdated);
            connect(this, &ControlWidget::hideInactiveUsagesChanged, usageWidget, &UsageWidget::updateVisibilitySettings);
        }

        updateData();
        ui->usages->setLayout(valuesLayout);
    }

    void ControlWidget::updateData() {
        for(auto usageWidget: m_usageWidgets){
            usageWidget->updateData();
        }
    }

    void ControlWidget::initInfoTable() {
        ui->logMinValue->setText(QString::number(m_control->getLogicalMinimum()));
        ui->logMaxValue->setText(QString::number(m_control->getLogicalMaximum()));
        ui->phyMinValue->setText(QString::number(m_control->getPhysicalMinimum()));
        ui->phyMaxValue->setText(QString::number(m_control->getPhysicalMaximum()));
        ui->unitValue->setText(/*QString::number(m_control->getUnit())*/ "WIP");
        ui->unitExpValue->setText(QString::number(m_control->getUnitExponent()));
        ui->reportSizeValue->setText(QString::number(m_control->getReportSize()));
        ui->reportCountValue->setText(QString::number(m_control->getReportCount()));
        ui->reportIDValue->setText(QString::number(m_control->getReportId()));
    }
}
}
