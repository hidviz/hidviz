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

#ifndef HIDVIZ_WINDOW_HH
#define HIDVIZ_WINDOW_HH

#include <QWidget>

#include <memory>

namespace libhidx {
    class Interface;
    class LibHidx;
}

namespace Ui {
    class Window;
}

namespace hidviz {

    class DeviceView;

    class Window : public QWidget {
    Q_OBJECT

    public:
        Window();
        ~Window() override;

    protected:
        void closeEvent(QCloseEvent* event) override;

    private:
        Ui::Window* ui = nullptr;
        DeviceView *m_deviceView = nullptr;
        libhidx::Interface* m_selectedInterface = nullptr;
        std::unique_ptr<libhidx::LibHidx> m_lib;

    signals:
        void dataRead();

    public slots:
        void openDeviceSelector();
        void selectDevice(libhidx::Interface&);
        void clearModel();
        void updateSettings();

        libhidx::LibHidx* getLibhidx();
        void loadSettings();
    };
}


#endif //HIDVIZ_WINDOW_HH
