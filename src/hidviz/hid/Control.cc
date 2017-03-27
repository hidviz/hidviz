#include "Control.hh"

#include "libhidx/hid/Control.hh"
#include "libhidx/Usages.hh"

#include <QVBoxLayout>
#include <QLabel>

namespace hidviz{
namespace hid {
    Control::Control(libhidx::hid::Control* control) : Item{}, m_control{control} {
        const char *types[] = {
            "Input", "Output", "Feature", "Padding"
        };

        auto reportType = m_control->m_reportType;
        if(!m_control->m_usages.size()){
            reportType = 3;
        }

        m_name->setText(types[reportType]);

        auto valuesLayout = new QVBoxLayout{};

        for(auto usage: m_control->m_usages){
            auto valueLayout = new QHBoxLayout{};
            auto usageText = getHidUsageText(usage);
            valueLayout->addWidget(new QLabel{QString::fromStdString(usageText)});
            auto value = new QLabel;
            m_valueLabels.push_back(value);
            valueLayout->addWidget(value);
            valuesLayout->addLayout(valueLayout);
        }

        m_mainLayout->addLayout(valuesLayout);
    }

    void Control::updateData() {
        for(auto label: m_valueLabels){
            label->setText(QString::number(m_control->getData()));
        }
    }
}
}
