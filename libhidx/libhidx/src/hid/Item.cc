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

#include <libhidx/hid/Item.hh>

#include <algorithm>

namespace libhidx {
namespace hid {

    void Item::appendChild(Item* child) {
        m_children.push_back(std::unique_ptr<Item>(child));
    }

    Item* Item::child(int index) {
        return m_children[index].get();
    }

    size_t Item::childCount() const {
        return m_children.size();
    }

    void Item::forEach(const std::function<void(Item*)>& f) {
        f(this);
        for(const auto& child: m_children){
            child->forEach(f);
        }
    }

    Item* Item::topItem() {
        if(m_parent)
            return m_parent->topItem();
        return this;
    }


}
}
