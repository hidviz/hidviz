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

#ifndef LIBHIDX_USAGES_HH
#define LIBHIDX_USAGES_HH

#include <string>
#include <cstdint>

namespace libhidx {
    /**
     * Returns a string containing a verbal description of the specified USB HID usage page.
     *
     * Based on "HID Usage Tables 10/28/2004 Version 1.12" (with a few spelling errors corrected),
     */
    std::string getHidUsagePageText(uint32_t usagePageRaw);

    /**
     * Returns a string containing a verbal description of the specified USB HID usage.
     *
     * Based on "HID Usage Tables 10/28/2004 Version 1.12" (with a few spelling errors corrected),
     * plus changes from Review Requests 28 through 52.  Exceptions:
     *
     *    - Implementation of RR39 is incomplete.
     *    - The conflicting assignments between RR33 (status "Review") and RR47 (status "Approved") are noted below; I went with the latter.
     *    - Includes usage 0x07:0xffffffff (the 8-bit USB HID keycodes of up to 8 simultanouely-pressed keys).
     *    - Includes usage pages 0x84 and 0x85 from "Universal Serial Bus Usage Tables for HID Power Devices Release 1.0 November 1, 1997".
     *    - Includes usage page 0xff (Fn key on Apple Keyboards).
     *
     * Copyright (C) 2015 Steve Mokris.  Use and distribution permitted under the terms of the Apache License Version 2.0.
     */
    std::string getHidUsageText(uint32_t usageRaw);
}
#endif
