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
            auto usageText = getHidUsageText(usage);
            valuesLayout->addWidget(new QLabel{QString::fromStdString(usageText)});
        }

        m_mainLayout->addLayout(valuesLayout);
    }
}
}
