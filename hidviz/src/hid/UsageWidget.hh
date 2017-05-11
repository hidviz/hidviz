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

#ifndef HIDVIZ_USAGEWIDGET_HH
#define HIDVIZ_USAGEWIDGET_HH

#include <QFrame>

class QLineEdit;
class QPushButton;
class QProgressBar;

namespace libhidx {
namespace hid {
    class Usage;
    class Control;
}
}

namespace Ui {
    class UsageWidget;
}

namespace hidviz {
namespace hid {

    /**
     * Widget for visualizing data of usages.
     *
     * This widget is capable of visualizing usage data in most appropriate form.
     * For example boolean value is shown as button, other types are visualised
     * as slider or simple text input.
     */
    class UsageWidget : public QFrame {
    Q_OBJECT
    public:
        /**
         * Constructs UsageWidget instance.
         * @param usage Related libhidx Usage instance
         * @param parent Parent widget
         */
        explicit UsageWidget(libhidx::hid::Usage& usage, QWidget* parent = nullptr);

        /// Destructs UsageWidget instance.
        ~UsageWidget() override;

        /**
         * Updates usage's data.
         *
         * The new value is taken from libhidx's Usage instance.
         */
        void updateData();

    private:
        /// Inits input widget for visualisation.
        void initInput();

        /// Inits output widget for visualisation.
        void initOutput();

        /// Ui instance.
        Ui::UsageWidget* ui = nullptr;

        /// Related libhidx Usage instance.
        libhidx::hid::Usage& m_usage;

        // TODO: merge following 3 members to variant
        /// Progress bar instance for visualisation of non-boolean input.
        QProgressBar* m_progressBar = nullptr;

        /// LineEdit instance for visualisation of non-boolean output.
        QLineEdit* m_lineEdit = nullptr;

        /// PushButton instance for visualisaion of boolean input or output.
        QPushButton* m_button = nullptr;

        /// Related libhidx Control instance.
        const libhidx::hid::Control& m_control;

        /// Minimum value for clamping.
        int m_clampMinValue = 0;

        /// Maximum value for clamping.
        int m_clampMaxValue = 0;

        /**
         * Inits button for visualisation of boolean value.
         *
         * Button is used both for input and output, this method needes to know
         * if the button should be enabled for output or disabled for input.
         * @param enabled Desired state of button
         */
        void initBinaryButton(bool enabled);

    signals:
        /// Signal emitted when this usage is output and has been updated.
        void dataUpdated();
    };

}}


#endif //HIDVIZ_USAGEWIDGET_HH
