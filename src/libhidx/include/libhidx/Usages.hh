#ifndef LIBHIDX_USAGES_HH
#define LIBHIDX_USAGES_HH

#include <string>

/**
 * Returns a string containing a verbal description of the specified USB HID usage.
 *
 * The caller is responsible for freeing the returned string.
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

#endif
