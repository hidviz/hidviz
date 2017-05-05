//
// hidviz - Tool for in-depth analysis of USB HID devices communication
// Copyright (C) 2017 Ondrej Budai <ondrej@budai.cz>
//
// This file is part of hidviz.
//
// hidviz is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// hidviz is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with hidviz.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef HIDVIZ_DEVICEVIEW_HH
#define HIDVIZ_DEVICEVIEW_HH

#include <QWidget>

namespace libhidx {
    class Interface;
namespace hid {
    class Item;
}
}

class QGridLayout;

namespace hidviz {
    namespace hid {
        class ItemWidget;
    }
    class DeviceView : public QWidget {
    Q_OBJECT
    public:
        explicit DeviceView(libhidx::Interface& model, QWidget *parent = nullptr);
        ~DeviceView() override;
        void updateData();

    private:
        void addItem(libhidx::hid::Item* item, hid::ItemWidget* parent = nullptr);
        void sendData();

        libhidx::Interface& m_interface;
        QGridLayout* m_layout;

        std::vector<hid::ItemWidget*> m_items;

    signals:
        void dataRead();
    };
}


#endif //HIDVIZ_DEVICEVIEW_HH
