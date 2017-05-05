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

    class DeviceSelector : public QDialog {
    Q_OBJECT
    public:
        explicit DeviceSelector(libhidx::LibHidx& lib);
        ~DeviceSelector() override;
    private:
        Ui::DeviceSelector* ui;
        libhidx::LibHidx& m_lib;

    signals:
        void deviceSelected(libhidx::Interface&);
        void listCleared();

    public slots:

        void selectDevice();
        void initListWidget() const;
        void reloadDevices();
    };
}


#endif //HIDVIZ_DEVICESELECTOR_HH
