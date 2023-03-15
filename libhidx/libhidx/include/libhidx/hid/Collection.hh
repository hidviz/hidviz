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

#ifndef LIBHIDX_COLLECTION_HH
#define LIBHIDX_COLLECTION_HH

#include "Item.hh"

namespace libhidx {
    class Parser;

namespace hid {

    /// Class representing HID collection
    class Collection : public Item {
    friend class libhidx::Parser;
    public:
        /**
         * Contructs Collection.
         * @param parent Parent collection
         */
        explicit Collection(Item* parent = nullptr) : Item{parent} {}

        /// Returns collection type
        auto getType(){return m_type;}

        /// Returns text represantation of collection type
        std::string getTypeStr();

        /// Returns collection usage
        auto getUsage(){return m_usage;}

        /// Returns text representation of collection usage
        std::string getUsageStr();

    private:
        /// Collection type
        uint32_t m_type = 0;

        /// Collection usage
        unsigned m_usage = 0;
    };
}
}


#endif //LIBHIDX_COLLECTION_HH
