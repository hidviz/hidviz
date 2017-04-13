#include "ControlWidget.hh"

#include "../FlowLayout.hh"
#include "UsageWidget.hh"

#include <libhidx/hid/Control.hh>

#include <QLabel>
#include <QLineEdit>

namespace hidviz{
namespace hid {
    ControlWidget::ControlWidget(libhidx::hid::Control* control) : ItemWidget{}, m_control{control} {
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

        setName(typeStr);

        initDetailInfo();
    }

    void ControlWidget::initDetailInfo() {
        auto valuesLayout = new FlowLayout{};
        auto& usages = m_control->getUsages();
        for(auto& usage: usages){
            UsageWidget *usageWidget = new UsageWidget{*usage};
            m_usageWidgets.push_back(usageWidget);
            valuesLayout->addWidget(usageWidget);

            connect(usageWidget, &UsageWidget::dataUpdated, [this](){
                emit dataUpdated();
            });
        }

        updateData();

        auto w = new QWidget;
        w->setLayout(valuesLayout);
        appendWidget(w);
    }

    void ControlWidget::updateData() {
        for(auto usageWidget: m_usageWidgets){
            usageWidget->updateData();
        }
    }
}
}
