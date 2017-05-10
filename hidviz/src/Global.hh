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

#ifndef PROJECT_GLOBAL_HH
#define PROJECT_GLOBAL_HH

namespace hidviz {

    /// Struct to hold hidviz's global constants
    struct Global {
        /// Application name
        static constexpr const char* appName = "hidviz";

        /// Constants for names of settings store in QSettings
        struct Settings {
            /// Hide inactive usages settings name
            static constexpr const char* hideInactiveUsages = "hideInactiveUsages";

            /// Clamp values settings name
            static constexpr const char* clampValues = "clampValues";
        };
    };
}

#endif
