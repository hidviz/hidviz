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

    /**
     * Widget containing information about device structure and state.
     *
     * This widget can contain more than one ItemWidget, new widgets are added by calling addItem method.
     */

    class DeviceView : public QWidget {
    Q_OBJECT
    public:
        /**
         * Constructs DeviceView instance.
         * @param interface Interface to be shown in DeviceView
         * @param parent Parent widget
         */
        explicit DeviceView(libhidx::Interface& interface, QWidget* parent = nullptr);

        /// Destructs DeviceView instance.
        ~DeviceView() override;

        /// Updates data in DeviceView.
        void updateData();

    private:
        /**
         * Adds item to DeviceView (usable for more top level collections)
         * @param item Libhidx representation of item
         * @param parent Parent widget
         */
        void addItem(libhidx::hid::Item* item, hid::ItemWidget* parent = nullptr);

        /// Tells interface to send the output data.
        void sendData();

        /// Selected interface for this view.
        libhidx::Interface& m_interface;

        /// Main layout of this view.
        QGridLayout* m_layout;

        /// Vector holding all top level item widgets
        std::vector<hid::ItemWidget*> m_items;

    signals:
        /// Signal emitted when the data has been read.
        void dataRead();
    };
}


#endif //HIDVIZ_DEVICEVIEW_HH
