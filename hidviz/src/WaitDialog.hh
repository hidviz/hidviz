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

#ifndef PROJECT_WAITDIALOG_HH
#define PROJECT_WAITDIALOG_HH

#include <QDialog>

#include <functional>
#include <chrono>

namespace Ui {
    class WaitDialog; }
namespace hidviz {

    /// Dialog used when waiting for connection to libhidx's backend
    class WaitDialog : public QDialog {
    public:
        /**
         * Construct WaitDialog instance.
         *
         * WaitDialog will periodically try to poll predicate function.
         * If the predicate returns true, the dialog will close with success status.
         * If user clicks on Abort button, the dialog will close with failure status.
         * @param interval Interval at which the predicate should be polled
         * @param predicate Preditace to be checked
         * @param parent parent widget
         */
        WaitDialog(std::chrono::milliseconds interval, std::function<bool()> predicate, QWidget* parent = nullptr);

        /// Destructs WaitDialog instance.
        ~WaitDialog() override;

    private:
        /// Ui instance
        Ui::WaitDialog* ui;
    };
}


#endif //PROJECT_WAITDIALOG_HH
