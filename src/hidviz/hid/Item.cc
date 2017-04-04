#include "Item.hh"

#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>

namespace hidviz {
namespace hid {

    Item::Item() : QWidget{} {
        m_mainLayout = new QGridLayout{};
        //checkbox
        auto checkbox = new QCheckBox{};
        checkbox->setChecked(true);
        m_mainLayout->addWidget(checkbox, 0, 0);

        //header
        auto header = new QWidget{};
        header->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        auto headerLayout = new QHBoxLayout{};
        header->setLayout(headerLayout);

        m_name = new QLabel{"name"};
        m_name->setStyleSheet("font-weight: bold;");
        headerLayout->addWidget(m_name);

        m_usage = new QLabel{"usage"};
        headerLayout->addWidget(m_name);

        m_mainLayout->addWidget(header, 0, 1);

        m_content = new QWidget{};
        m_mainLayout->addWidget(m_content, 1, 1);

        setLayout(m_mainLayout);
        setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);

        connect(checkbox, &QCheckBox::stateChanged, [this](int newState){
            if(!m_content){
                return;
            }

            if(newState == Qt::Checked){
                m_content->show();
            } else if(newState == Qt::Unchecked){
                m_content->hide();
            }
        });
    }

    void Item::setContent(QWidget* widget) {
        delete m_content;
        m_mainLayout->addWidget(widget, 1, 1);
        m_content = widget;
    }
}
}
