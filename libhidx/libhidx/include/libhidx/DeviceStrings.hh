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

#ifndef LIBHIDX_HIDLIBDEVICESTRINGS_HH
#define LIBHIDX_HIDLIBDEVICESTRINGS_HH

#include <string>

namespace libhidx {

    /// Class holding device strings (manufacturer, product, serial number).
    class DeviceStrings {
    public:
        std::string manufacturer;
        std::string product;
        std::string serialNumber;
    };

}


#endif //LIBHIDX_HIDLIBDEVICESTRINGS_HH
