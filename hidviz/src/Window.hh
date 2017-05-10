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

    /// Main hidviz window, contains all other widgets.
    class Window : public QWidget {
    Q_OBJECT
    public:
        /**
         * Construct Window instance.
         * @todo Rename to HidvizWindow
         * @param parent Parent widget
         */
        explicit Window(QWidget* parent = nullptr);

        /// Descturs Window instance.
        ~Window() override;

    protected:
        /**
         * Saves information about window geometry.
         * @param event Event object.
         */
        void closeEvent(QCloseEvent* event) override;

    private:
        /// Ui instance
        Ui::Window* ui = nullptr;

        /// Device view instance
        DeviceView *m_deviceView = nullptr;

        /// Selected interface instance
        libhidx::Interface* m_selectedInterface = nullptr;

        /// Libhidx instance (this is the actual place where the pointer is stored!)
        std::unique_ptr<libhidx::LibHidx> m_lib;

    public slots:
        /// Opens device selector dialog.
        void openDeviceSelector();

        /**
         * Selects device and creates DeviceView and other views.
         * @param interface Selected interface
         */
        void selectDevice(libhidx::Interface& interface);

        /// Clears all views, so the interface can be freed.
        void clearModel();

        /// Reads settings from widget and applies it.
        void updateSettings();

        /**
         * Returns libhidx instance.
         *
         * This method is also responsible for creating libhidx instance if no such exists.
         * Note that because of libhidx structure, this function may take a long time
         * and spawns multiple dialogs.
         * It can return nullptr when creation of libhidx instance fails!
         * @return Pointer to libhidx instance or nullptr if its construction fails
         */
        libhidx::LibHidx* getLibhidx();

        /// Loads settings from QSettings and writes them into appropriate widgets.
        void loadSettings();
    };
}


#endif //HIDVIZ_WINDOW_HH
