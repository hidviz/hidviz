#include "Control.hh"

#include "libhidx/hid/Control.hh"

#include <QVBoxLayout>
#include <QLabel>

namespace hidviz{
namespace hid {
    Control::Control(libhidx::hid::Control* control) : Item{}, m_control{control} {
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

        m_name->setText(typeStr);

        auto valuesLayout = new QVBoxLayout{};

        for(const auto& usage: usages){
            auto valueLayout = new QHBoxLayout{};
            valueLayout->addWidget(new QLabel{QString::fromStdString(usage.getName())});
            auto value = new QLabel;
            m_valueLabels.push_back(value);
            valueLayout->addWidget(value);
            valuesLayout->addLayout(valueLayout);
        }

        m_mainLayout->addLayout(valuesLayout);
    }

    void Control::updateData() {
        for(size_t i = 0; i < m_valueLabels.size(); ++i){
            auto label = m_valueLabels[i];
            auto data = m_control->getUsages()[i].getData();
            label->setText(QString::number(data));
        }
    }
}
}
