#include "Control.hh"

#include "../FlowLayout.hh"

#include <libhidx/hid/Control.hh>

#include <QLabel>
#include <QLineEdit>

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
        for(auto& usage: usages){
            QWidget *valueLayoutWidget = getWidgetForUsage(usage.get());
            valuesLayout->addWidget(valueLayoutWidget);
        }

        updateData();

        auto w = new QWidget;
        w->setLayout(valuesLayout);
        appendWidget(w);
    }

    QWidget* Control::getWidgetForUsage(libhidx::hid::Usage *usage) {
        auto valueLayoutWidget = new QWidget{};
        auto valueLayout = new QVBoxLayout{};
        valueLayout->addWidget(new QLabel{QString::fromStdString(usage->getName())});

        auto reportType = m_control->getReportType();
        QWidget* value{nullptr};

        if(reportType == libhidx::hid::Control::Type::INPUT){
            auto valueLabel = new QLabel;
            m_valueLabels.push_back(valueLabel);
            value = valueLabel;
        } else if(reportType == libhidx::hid::Control::Type::OUTPUT){
            auto valueEdit = new QLineEdit{};
            connect(valueEdit, &QLineEdit::editingFinished, [valueEdit, usage, this]{
                auto text = valueEdit->text().toStdString();
                usage->setLogicalValueFromUser(text);
                emit dataUpdated();
            });
            value = valueEdit;
        }

        valueLayout->addWidget(value);
        valueLayoutWidget->setLayout(valueLayout);
        return valueLayoutWidget;
    }

    void Control::updateData() {
        if(m_control->getReportType() != libhidx::hid::Control::Type::INPUT){
            return;
        }
        for(size_t i = 0; i < m_valueLabels.size(); ++i){
            auto label = m_valueLabels[i];
            auto data = m_control->getUsages()[i]->getPhysicalValue();
            label->setText(QString::number(data));
        }
    }
}
}
