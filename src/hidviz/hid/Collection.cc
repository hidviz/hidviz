#include "Collection.hh"

#include <QHBoxLayout>
#include <QLabel>
#include <libhidx/hid/Item.hh>

namespace hidviz {
namespace hid {

    Collection::Collection(libhidx::hid::Item* item) : QWidget{}, m_item{item} {
        setAutoFillBackground(true);
        auto layout = new QHBoxLayout{};
        layout->addWidget(new QLabel{"ahoj"});
        layout->addWidget(new QLabel{"ahojda"});
        layout->addWidget(new QLabel{"ahojdacka"});

        if(m_item->m_collection){
            layout->addWidget(new QLabel{"Collection"});
        }
        if(m_item->m_control){
            layout->addWidget(new QLabel{"Control"});
        }

        setLayout(layout);
    }
}
}
