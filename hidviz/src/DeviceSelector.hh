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

#ifndef HIDVIZ_DEVICESELECTOR_HH
#define HIDVIZ_DEVICESELECTOR_HH

#include <QDialog>

namespace libhidx {
    class LibHidx;
    class Interface;
}

namespace Ui {
    class DeviceSelector;
}

namespace hidviz {
    /**
     * Widget for selecting interface to be analyzed.
     *
     * @todo Possibly rename this widget to InterfaceSelector.
     * @todo Convert me to exec style widget.
     *
     * Do not get confused with this class. It actually selects an interface
     * to be analyzed. Name is just kept for backward compatibility.
     */
    class DeviceSelector : public QDialog {
    Q_OBJECT
    public:
        /**
         * Constructs DeviceSelector instance.
         * @param lib Instance of Libhidx
         */
        explicit DeviceSelector(libhidx::LibHidx& lib);

        /// Destructs DeviceSelector instance.
        ~DeviceSelector() override;
    private:
        /// Ui instance.
        Ui::DeviceSelector* ui;

        /// Libhidx instance.
        libhidx::LibHidx& m_lib;

        /// Inits list widget containing all interfaces.
        void initListWidget() const;

    signals:
        /**
         * Signal emitted when interface was selected by user.
         * @param interface Selected interface
         */
        void deviceSelected(libhidx::Interface& interface);

        /// Signal emitted when all open Interface should be closed and the device list will be reloaded.
        void listCleared();

    private slots:
        /// Device is selected by user.
        void selectDevice();

        /// User demands reloading the device list.
        void reloadDevices();
    };
}


#endif //HIDVIZ_DEVICESELECTOR_HH
