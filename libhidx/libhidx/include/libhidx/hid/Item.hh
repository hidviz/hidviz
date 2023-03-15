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

#ifndef LIBHIDX_ITEM_HH
#define LIBHIDX_ITEM_HH

#include <vector>
#include <memory>
#include <functional>

namespace libhidx {
    class Parser;
namespace hid {

    /**
     * Base class for representations of HID controls and collections.
     * @todo It should be possible to delete this class and remove inheritance,
     * because they don't share much behaviour.
     */
    class Item {
        friend class libhidx::Parser;
    public:
        /**
         * Constructs Item instance.
         * @param parent Parent item.
         */
        explicit Item(Item* parent = nullptr) : m_parent{parent}{}
        Item(const Item&) = delete;
        Item& operator= (const Item&) = delete;

        /// Destructs Item and its children.
        virtual ~Item() = default;

        /// Adds new child.
        void appendChild(Item* child);

        /**
         * Returns child at specific index.
         * @param index Index from which the childbe returned
         * @return Child at index
         */
        Item* child(int index);

        /// Returns child could.
        size_t childCount() const;

        /**
         * Runs function for this Item and all its children recursively.
         * @param function Functor to be run for all children
         */
        void forEach(const std::function<void(Item*)>& function);

        /// Returns if this Item is numbered (Report IDs are used).
        auto isNumbered(){return topItem()->m_numbered;}

        /// Returns the depth of this Item (how many levels is it from top level collection).
        unsigned getLevel(){if(!m_parent){return 0;} return m_parent->getLevel() + 1;}

    protected:
        /// Returns top level collection.
        Item* topItem();

    private:
        /// Vector of all children controls
        std::vector<std::unique_ptr<Item>> m_children;

        /// Parent item (collection)
        Item* m_parent = nullptr;

        /**
         * Holds information whether this Item is numbered (Report IDs are used in analyzed device).
         * DO NOT use this value when figuring out whether this Item is in numbered device, use isNumbered
         * instead, because this flag could be set only in top level collection.
         * @todo Make this sane.
         */
        bool m_numbered = false;

    };
}
}


#endif //LIBHIDX_ITEM_HH
