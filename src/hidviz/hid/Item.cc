#include "Item.hh"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

namespace hidviz {
namespace hid {

    Item::Item() : QWidget{} {
        // needed because the parent is QTreeView
        setAutoFillBackground(true);
        m_mainLayout = new QVBoxLayout{};

        m_headerLayout = new QHBoxLayout{};

        m_name = new QLabel{};
        m_headerLayout->addWidget(m_name);

        m_usage = new QLabel{};
        m_headerLayout->addWidget(m_usage);

        m_mainLayout->addLayout(m_headerLayout);

        setLayout(m_mainLayout);

    }
}
}
