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

#ifndef HIDVIZ_CONTROL_HH
#define HIDVIZ_CONTROL_HH

#include "ItemWidget.hh"

namespace Ui {
    class ControlSubWidget;
}

namespace libhidx {
namespace hid {
    class Control;
    class Usage;
}
}

namespace hidviz {
namespace hid {
    class UsageWidget;


    /// Widget for showing information about controls (input, output).
    class ControlWidget : public ItemWidget {
    Q_OBJECT
    public:
        /**
         * Constructs ControlWidget object.
         * @param control libhidx control instance which data should be visualised
         * @param parent Parent widget
         */
        ControlWidget(libhidx::hid::Control* control, QWidget* parent = nullptr);

        /**
         * Updates data in widget.
         *
         * The new data is taken from connected libhidx control object.
         */
        void updateData();

    private:
        /// Connected libhidx control object.
        libhidx::hid::Control* m_control;

        /// Vector of Usage widgets belonging to this Control.
        QList<UsageWidget*> m_usageWidgets;

        /// Ui instance.
        Ui::ControlSubWidget* ui;

        /// Setups gui of this widgets.
        void initGui();

        /// Inits all usage widgets.
        void initUsages();

        /// Inits table contraining detailed information about control structure.
        void initInfoTable();

    signals:
        /// Signal to be emitted when (output) data in this control is updated.
        void dataUpdated();


    };
}
}


#endif //HIDVIZ_CONTROL_HH
