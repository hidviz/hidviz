#include "DeviceView.hh"

#include "hid/Control.hh"
#include "hid/Collection.hh"

#include "libhidx/hid/Control.hh"
#include "libhidx/hid/Collection.hh"

#include <QGridLayout>
#include <QLabel>

namespace hidviz {

    DeviceView::DeviceView(libhidx::hid::Item *model, QWidget *parent) : QWidget{parent}, m_model{model} {
        m_layout = new QGridLayout{this};

        std::function<unsigned(libhidx::hid::Item *)> it;

        it = [&it](libhidx::hid::Item *item) {
            unsigned max = 0;
            for (unsigned i = 0; i < item->childCount(); ++i) {
                auto child = item->child(0);
                max = std::max(max, it(child) + 1);
            }

            return max;
        };

        m_depth = it(model);
        for(unsigned i = 0; i < model->childCount(); ++i){
            auto child = model->child(i);
            addItem(child, 0);
        }

        auto w = new QWidget{};
        w->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

        m_layout->addWidget(w, m_layout->rowCount(), 0);
    }

    void DeviceView::addItem(libhidx::hid::Item *item, unsigned currentDepth) {
        auto nextRow = m_layout->rowCount();

        auto label = new QLabel{"here"};
        label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        m_layout->addWidget(label, nextRow, currentDepth);

        hid::Item* itemWidget = nullptr;
        if (item->m_control) {
            itemWidget = new hid::Control{static_cast<libhidx::hid::Control*>(item)};
            m_layout->addWidget(itemWidget, nextRow, m_depth);
        } else if(item->m_collection) {
            itemWidget = new hid::Collection{static_cast<libhidx::hid::Collection *>(item)};
            m_layout->addWidget(itemWidget, nextRow, m_depth);
        }

        if(itemWidget){
            m_items.emplace_back(itemWidget);
        }

        for (unsigned i = 0; i < item->childCount(); ++i) {
            auto child = item->child(i);
            addItem(child, currentDepth + 1);
        }
    }

    void DeviceView::updateData() {
        for(auto item: m_items){
            auto control = dynamic_cast<hid::Control*>(item);
            if(control) {
                control->updateData();
            }
        }
    }

}
