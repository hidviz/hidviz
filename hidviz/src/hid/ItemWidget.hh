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

#ifndef HIDVIZ_ITEM_HH
#define HIDVIZ_ITEM_HH

#include <QWidget>

namespace Ui {
    class ItemWidget;
}

namespace hidviz {
namespace hid {

    /**
     * Base abstract widget for showing HID items.
     *
     * This widget can contain child widgets. This is useful mainly for collections.
     */
    class ItemWidget : public QWidget {
    public:
        /**
         * Contructs ItemWidget object.
         * @param level Depth of item, for top level collection depth = 0
         * @param parent Parent widget
         */
        explicit ItemWidget(size_t level, QWidget* parent = nullptr);

        /// Destructs ItemWidget object.
        ~ItemWidget() override;

        /**
         * Appends next child child object (useful primarily for collections).
         * @param widget Child object to appended.
         */
        void appendWidget(QWidget *widget);

    protected:
        /**
         * Sets name of this widget, name show as main header on the left side.
         * @param name New name
         */
        void setName(const QString& name);

        /**
         * Sets "usage" of this widget, usage show as secondary header on the right side.
         * @param usage New usage.
         */
        void setUsage(const QString& usage);

    private:
        /// Ui instance
        Ui::ItemWidget* ui;

        /// Vector of children
        QList<QWidget*> m_children;

    };
}}


#endif //HIDVIZ_ITEM_HH
