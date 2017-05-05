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

class QLabel;
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

    class UsageWidget : public QFrame {
    Q_OBJECT
    public:
        explicit UsageWidget(libhidx::hid::Usage& usage, QWidget* parent = nullptr);
        ~UsageWidget() override;
        void updateData();

    private:
        void initInput();
        void initOutput();

        Ui::UsageWidget* ui = nullptr;
        libhidx::hid::Usage& m_usage;
        QProgressBar* m_label = nullptr;
        QLineEdit* m_lineEdit = nullptr;
        const libhidx::hid::Control& m_control;
        QPushButton* m_button = nullptr;
        int m_clampMinValue = 0;
        int m_clampMaxValue = 0;

    signals:
        void dataUpdated();
    public slots:
        void initBinaryButton(bool enabled);
    };

}}


#endif //HIDVIZ_USAGEWIDGET_HH
