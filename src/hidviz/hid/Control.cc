#include "Control.hh"

#include "libhidx/hid/Control.hh"

#include <QVBoxLayout>
#include <QLabel>
#include <FlowLayout.hh>

namespace hidviz{
namespace hid {
    Control::Control(libhidx::hid::Control* control) : Item{}, m_control{control} {
        initGui();
    }

    void Control::initGui() {
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

    void Control::initDetailInfo() {
        auto valuesLayout = new FlowLayout{};
        const auto& usages = m_control->getUsages();
        for(const auto& usage: usages){
            QWidget *valueLayoutWidget = getWidgetForUsage(usage);
            valuesLayout->addWidget(valueLayoutWidget);
        }

        auto w = new QWidget;
        w->setLayout(valuesLayout);
        setContent(w);
    }

    QWidget* Control::getWidgetForUsage(const libhidx::hid::Usage& usage) {
        auto valueLayoutWidget = new QWidget{};
        auto valueLayout = new QVBoxLayout{};
        valueLayout->addWidget(new QLabel{QString::fromStdString(usage.getName())});
        auto value = new QLabel;
        m_valueLabels.push_back(value);
        valueLayout->addWidget(value);
        valueLayoutWidget->setLayout(valueLayout);
        return valueLayoutWidget;
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
