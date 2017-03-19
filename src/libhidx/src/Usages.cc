#include "libhidx/Usages.hh"

#include <sstream>

static std::string getString(const char* str, uint16_t usagePage, uint16_t usage){
    std::ostringstream os;
    os << str << " {" << std::hex << usagePage << ':' << usage << ')';
    return os.str();
}

static std::string getUnknown(uint16_t usagePage, uint16_t usage){
    return getString("Unknown", usagePage, usage);
}

static std::string getVendorDefined(uint16_t usagePage, uint16_t usage){
    return getString("Vendor-defined", usagePage, usage);
}

std::string getHidUsageText(uint32_t usageRaw) {
    const uint16_t usagePage = static_cast<uint16_t>(usageRaw >> 16U);
    const uint16_t usage = static_cast<uint16_t>(usageRaw & 0xffff);

    const char* text = nullptr;

    if (usagePage == 0x01)	// Generic Desktop
    {
        static const char* lookup[] = {
            0, "Pointer", "Mouse", 0, "Joystick", "Game Pad", "Keyboard", "Keypad",
            "Multi-axis Controller", "Tablet PC System Controls", "Water Cooling Device", "Computer Chassis Device", "Wireless Radio Controls", "Portable Device Control", 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "X", "Y", "Z", "Rx", "Ry", "Rz", "Slider", "Dial",
            "Wheel", "Hat switch", "Counted Buffer", "Byte Count", "Motion Wakeup", "Start", "Select", 0,
            "Vx", "Vy", "Vz", "Vbrx", "Vbry", "Vbrz", "Vno", "Feature Notification",
            "Resolution Multiplier", 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "System Control", "System Power Down", "System Sleep", "System Wake Up", "System Context Menu", "System Main Menu", "System App Menu", "System Menu Help",
            "System Menu Exit", "System Menu Select", "System Menu Right", "System Menu Left", "System Menu Up", "System Menu Down", "System Cold Restart", "System Warm Restart",
            "D-pad Up", "D-pad Down", "D-pad Right", "D-pad Left", 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "System Dock", "System Undock", "System Setup", "System Break", "System Debugger Break", "Application Break", "Application Debugger Break", "System Speaker Mute",
            "System Hibernate", 0, 0, 0, 0, 0, 0, 0,
            "System Display Invert", "System Display Internal", "System Display External", "System Display Both", "System Display Dual", "System Display Toggle Int/Ext", "System Display Swap Primary/Secondary", "System Display LCD Autoscale",
            0, 0, 0, 0, 0, 0, 0, 0,
            "Sensor Zone", "RPM", "Coolant Level", "Coolant Critical Level", "Coolant Pump", "Chassis Enclosure", "Wireless Radio Button", "Wireless Radio LED",
            "Wireless Radio Slider Switch", "System Display Rotation Lock Button", "System Display Rotation Lock Slider Switch", "Control Enable", 0, 0, 0, 0,
        };
        if(usage >= (sizeof(lookup) / sizeof(const char *))){
            return getUnknown(usagePage, usage);
        }
        text = lookup[usage];

    } else if (usagePage == 0x02) {	// Simulation Control
        static const char* lookup[] = {
            0, "Flight Simulation Device", "Automobile Simulation Device", "Tank Simulation Device", "Spaceship Simulation Device", "Submarine Simulation Device", "Sailing Simulation Device", "Motorcycle Simulation Device",
            "Sports Simulation Device", "Airplane Simulation Device", "Helicopter Simulation Device", "Magic Carpet Simulation Device", "Bicycle Simulation Device", 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Flight Control Stick", "Flight Stick", "Cyclic Control", "Cyclic Trim", "Flight Yoke", "Track Control", 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Aileron", "Aileron Trim", "Anti-Torque Control", "Autopilot Enable", "Chaff Release", "Collective Control", "Dive Brake", "Electronic Countermeasures",
            "Elevator", "Elevator Trim", "Rudder", "Throttle", "Flight Communications", "Flare Release", "Landing Gear", "Toe Brake",
            "Trigger", "Weapons Arm", "Weapons Select", "Wing Flaps", "Accelerator", "Brake", "Clutch", "Shifter",
            "Steering", "Turret Direction", "Barrel Elevation", "Dive Plane", "Ballast", "Bicycle Crank", "Handle Bars", "Front Brake",
            "Rear Brake", 0, 0, 0, 0, 0, 0, 0,
        };
        if(usage >= (sizeof(lookup) / sizeof(const char *))){
            return getUnknown(usagePage, usage);
        }
        text = lookup[usage];

    } else if (usagePage == 0x03) {	// VR Controls
        static const char* lookup[] = {
            0, "Belt", "Body Suit", "Flexor", "Glove", "Head Tracker", "Head Mounted Display", "Hand Tracker",
            "Oculometer", "Vest", "Animatronic Device", 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Stereo Enable", "Display Enable", 0, 0, 0, 0, 0, 0,
        };
        if(usage >= (sizeof(lookup) / sizeof(const char *))){
            return getUnknown(usagePage, usage);
        }
        text = lookup[usage];

    } else if (usagePage == 0x04) {	// Sport Controls
        static const char* lookup[] = {
            0, "Baseball Bat", "Golf Club", "Rowing Machine", "Treadmill", 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Oar", "Slope", "Rate", "Stick Speed", "Stick Face Angle", "Stick Heel/Toe", "Stick Follow Through", "Stick Tempo",
            "Stick Type", "Stick Height", 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Putter", "1 Iron", "2 Iron", "3 Iron", "4 Iron", "5 Iron", "6 Iron", "7 Iron",
            "8 Iron", "9 Iron", "10 Iron", "11 Iron", "Sand Wedge", "Loft Wedge", "Power Wedge", "1 Wood",
            "3 Wood", "5 Wood", "7 Wood", "9 Wood", 0, 0, 0, 0,
        };
        if(usage >= (sizeof(lookup) / sizeof(const char *))){
            return getUnknown(usagePage, usage);
        }
        text = lookup[usage];

    } else if (usagePage == 0x05) {	// Game Controls
        static const char* lookup[] = {
            0, "3D Game Controller", "Pinball Device", "Gun Device", 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Point of View", "Turn Right/Left", "Pitch Forward/Backward", "Roll Right/Left", "Move Right/Left", "Move Forward/Backward", "Move Up/Down", "Lean Right/Left",
            "Lean Forward/Backward", "Height of POV", "Flipper", "Secondary Flipper", "Bump", "New Game", "Shoot Ball", "Player",
            "Gun Bolt", "Gun Clip", "Gun Selector", "Gun Single Shot", "Gun Burst", "Gun Automatic", "Gun Safety", "Gamepad Fire/Jump",
            "Gamepad Trigger", 0, 0, 0, 0, 0, 0, 0,
        };
        if(usage >= (sizeof(lookup) / sizeof(const char *))){
            return getUnknown(usagePage, usage);
        }
        text = lookup[usage];

    } else if (usagePage == 0x06) {	// Generic Device Controls
        static const char* lookup[] = {
            0, "Background Controls", 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Battery Strength", "Wireless Channel", "Wireless ID", "Discover Wireless Control", "Security Code Character Entered", "Security Code Character Erased", "Security Code Cleared", "Sequence ID",
            "Sequence ID Reset", "RF Signal Strength", 0, 0, 0, 0, 0, 0,
        };
        if(usage >= (sizeof(lookup) / sizeof(const char *))){
            return getUnknown(usagePage, usage);
        }
        text = lookup[usage];

    } else if (usagePage == 0x07) {	// Keyboard/Keypad
        static const char* lookup[] = {
            0, "Keyboard ErrorRollOver", "Keyboard POSTFail", "Keyboard ErrorUndefined", "Keyboard a and A", "Keyboard b and B", "Keyboard c and C", "Keyboard d and D",
            "Keyboard e and E", "Keyboard f and F", "Keyboard g and G", "Keyboard h and H", "Keyboard i and I", "Keyboard j and J", "Keyboard k and K", "Keyboard l and L",
            "Keyboard m and M", "Keyboard n and N", "Keyboard o and O", "Keyboard p and P", "Keyboard q and Q", "Keyboard r and R", "Keyboard s and S", "Keyboard t and T",
            "Keyboard u and U", "Keyboard v and V", "Keyboard w and W", "Keyboard x and X", "Keyboard y and Y", "Keyboard z and Z", "Keyboard 1 and !", "Keyboard 2 and @",
            "Keyboard 3 and #", "Keyboard 4 and $", "Keyboard 5 and %", "Keyboard 6 and ^", "Keyboard 7 and &", "Keyboard 8 and *", "Keyboard 9 and (", "Keyboard 0 and )",
            "Keyboard Return (ENTER)", "Keyboard ESCAPE", "Keyboard DELETE (Backspace)", "Keyboard Tab", "Keyboard Spacebar", "Keyboard - and _", "Keyboard = and +", "Keyboard [ and {",
            "Keyboard ] and }", "Keyboard \\ and |", "Keyboard Non-US # and ~", "Keyboard ; and :", "Keyboard ‘ and “", "Keyboard Grave Accent and Tilde", "Keyboard , and <", "Keyboard . and >",
            "Keyboard / and ?", "Keyboard Caps Lock", "Keyboard F1", "Keyboard F2", "Keyboard F3", "Keyboard F4", "Keyboard F5", "Keyboard F6",
            "Keyboard F7", "Keyboard F8", "Keyboard F9", "Keyboard F10", "Keyboard F11", "Keyboard F12", "Keyboard PrintScreen", "Keyboard Scroll Lock",
            "Keyboard Pause", "Keyboard Insert", "Keyboard Home", "Keyboard PageUp", "Keyboard Delete Forward", "Keyboard End", "Keyboard PageDown", "Keyboard RightArrow",
            "Keyboard LeftArrow", "Keyboard DownArrow", "Keyboard UpArrow", "Keypad Num Lock and Clear", "Keypad /", "Keypad *", "Keypad -", "Keypad +",
            "Keypad ENTER", "Keypad 1 and End", "Keypad 2 and Down Arrow", "Keypad 3 and PageDn", "Keypad 4 and Left Arrow", "Keypad 5", "Keypad 6 and Right Arrow", "Keypad 7 and Home",
            "Keypad 8 and Up Arrow", "Keypad 9 and PageUp", "Keypad 0 and Insert", "Keypad . and Delete", "Keyboard Non-US \\ and |", "Keyboard Application", "Keyboard Power", "Keypad =",
            "Keyboard F13", "Keyboard F14", "Keyboard F15", "Keyboard F16", "Keyboard F17", "Keyboard F18", "Keyboard F19", "Keyboard F20",
            "Keyboard F21", "Keyboard F22", "Keyboard F23", "Keyboard F24", "Keyboard Execute", "Keyboard Help", "Keyboard Menu", "Keyboard Select",
            "Keyboard Stop", "Keyboard Again", "Keyboard Undo", "Keyboard Cut", "Keyboard Copy", "Keyboard Paste", "Keyboard Find", "Keyboard Mute",
            "Keyboard Volume Up", "Keyboard Volume Down", "Keyboard Locking Caps Lock", "Keyboard Locking Num Lock", "Keyboard Locking Scroll Lock", "Keypad Comma", "Keypad Equal Sign", "Keyboard International1",
            "Keyboard International2", "Keyboard International3", "Keyboard International4", "Keyboard International5", "Keyboard International6", "Keyboard International7", "Keyboard International8", "Keyboard International9",
            "Keyboard LANG1", "Keyboard LANG2", "Keyboard LANG3", "Keyboard LANG4", "Keyboard LANG5", "Keyboard LANG6", "Keyboard LANG7", "Keyboard LANG8",
            "Keyboard LANG9", "Keyboard Alternate Erase", "Keyboard SysReq/Attention", "Keyboard Cancel", "Keyboard Clear", "Keyboard Prior", "Keyboard Return", "Keyboard Separator",
            "Keyboard Out", "Keyboard Oper", "Keyboard Clear/Again", "Keyboard CrSel/Props", "Keyboard ExSel", 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Keypad 00", "Keypad 000", "Thousands Separator", "Decimal Separator", "Currency Unit", "Currency Sub-unit", "Keypad (", "Keypad )",
            "Keypad {", "Keypad }", "Keypad Tab", "Keypad Backspace", "Keypad A", "Keypad B", "Keypad C", "Keypad D",
            "Keypad E", "Keypad F", "Keypad XOR", "Keypad ^", "Keypad %", "Keypad <", "Keypad >", "Keypad &",
            "Keypad &&", "Keypad |", "Keypad ||", "Keypad :", "Keypad #", "Keypad Space", "Keypad @", "Keypad !",
            "Keypad Memory Store", "Keypad Memory Recall", "Keypad Memory Clear", "Keypad Memory Add", "Keypad Memory Subtract", "Keypad Memory Multiply", "Keypad Memory Divide", "Keypad +/-",
            "Keypad Clear", "Keypad Clear Entry", "Keypad Binary", "Keypad Octal", "Keypad Decimal", "Keypad Hexadecimal", 0, 0,
            "Keyboard LeftControl", "Keyboard LeftShift", "Keyboard LeftAlt", "Keyboard Left GUI", "Keyboard RightControl", "Keyboard RightShift", "Keyboard RightAlt", "Keyboard Right GUI",
        };
        if(usage >= (sizeof(lookup) / sizeof(const char *))){
            return getUnknown(usagePage, usage);
        }
        text = lookup[usage];

    } else if (usagePage == 0x08) {	// LED
        static const char* lookup[] = {
            0, "Num Lock", "Caps Lock", "Scroll Lock", "Compose", "Kana", "Power", "Shift",
            "Do Not Disturb", "Mute", "Tone Enable", "High Cut Filter", "Low Cut Filter", "Equalizer Enable", "Sound Field On", "Surround On",
            "Repeat", "Stereo", "Sampling Rate Detect", "Spinning", "CAV", "CLV", "Recording Format Detect", "Off-Hook",
            "Ring", "Message Waiting", "Data Mode", "Battery Operation", "Battery OK", "Battery Low", "Speaker", "Head Set",
            "Hold", "Microphone", "Coverage", "Night Mode", "Send Calls", "Call Pickup", "Conference", "Stand-by",
            "Camera On", "Camera Off", "On-Line", "Off-Line", "Busy", "Ready", "Paper-Out", "Paper-Jam",
            "Remote", "Forward", "Reverse", "Stop", "Rewind", "Fast Forward", "Play", "Pause",
            "Record", "Error", "Usage Selected Indicator", "Usage In Use Indicator", "Usage Multi Mode Indicator", "Indicator On", "Indicator Flash", "Indicator Slow Blink",
            "Indicator Fast Blink", "Indicator Off", "Flash On Time", "Slow Blink On Time", "Slow Blink Off Time", "Fast Blink On Time", "Fast Blink Off Time", "￼Usage Indicator Color",
            "Indicator Red", "Indicator Green", "Indicator Amber", "Generic Indicator", "System Suspend", "External Power Connected", /* "Indicator Blue", "Indicator Orange", */ "Player Indicator", "Player 1",	// Conflict between RR33 and RR47
            /* "Good Status", "Warning Status", "RGB LED", "Red LED Channel", "Greed LED Channel", "Blue LED Channel", "LED Intensity", 0, */ "Player 2", "Player 3", "Player 4",  "Player 5",  "Player 6",  "Player 7",  "Player 8", 0,	// Conflict between RR33 and RR47
        };
        if(usage >= (sizeof(lookup) / sizeof(const char *))){
            return getUnknown(usagePage, usage);
        }
        text = lookup[usage];

    } else if (usagePage == 0x09) {	// Button
        return std::string{"Button "} + std::to_string(usage);
    }

    else if (usagePage == 0x0a)	// Ordinal
    {
        return std::string{"Instance "} + std::to_string(usage);
    }

    else if (usagePage == 0x0b)	// Telephony Device
    {
        static const char* lookup[] = {
            0, "Phone", "Answering Machine", "Message Controls", "Handset", "Headset", "Telephony Key Pad", "Programmable Button",
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Hook Switch", "Flash", "Feature", "Hold", "Redial", "Transfer", "Drop", "Park",
            "Forward Calls", "Alternate Function", "Line", "Speaker Phone", "Conference", "Ring Enable", "Ring Select", "Phone Mute",
            "Caller ID", "Send", 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Speed Dial", "Store Number", "Recall Number", "Phone Directory", 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Voice Mail", "Screen Calls", "Do Not Disturb", "Message", "Answer On/Off", 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Inside Dial Tone", "Outside Dial Tone", "Inside Ring Tone", "Outside Ring Tone", "Priority Ring Tone", "Inside Ringback", "Priority Ringback", "Line Busy Tone",
            "Reorder Tone", "Call Waiting Tone", "Confirmation Tone", "Confirmation Tone", "Tones Off", "Outside Ringback", "Ringer", 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Phone Key 0", "Phone Key 1", "Phone Key 2", "Phone Key 3", "Phone Key 4", "Phone Key 5", "Phone Key 6", "Phone Key 7",
            "Phone Key 8", "Phone Key 9", "Phone Key Star", "Phone Key Pound", "Phone Key A", "Phone Key B", "Phone Key C", "Phone Key D",
            "Phone Call History Key", "Phone Caller ID Key", "Phone Settings Key", 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Host Control", "Host Available", "Host Call Active", "Activate Handset Audio", "Ring Type", "Re-dialable Phone Number", 0, 0,
            "Stop Ring Tone", "PSTN Ring Tone", "Host Ring Tone", "Alert Sound Error", "Alert Sound Confirm", "Alert Sound Notification", "Silent Ring", 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Email Message Waiting", "Voicemail Message Waiting", "Host Hold", 0, 0, 0, 0, 0,
            "Incoming Call History Count", "Outgoing Call History Count", "Incoming Call History", "Outgoing Call History", "Phone Locale", 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Phone Time Second", "Phone Time Minute", "Phone Time Hour", "Phone Date Day", "Phone Date Month", "Phone Date Year", "Handset Nickname", "Address Book ID",
            "Call Duration", "Dual Mode Phone", 0, 0, 0, 0, 0, 0,
        };
        if(usage >= (sizeof(lookup) / sizeof(const char *))){
            return getUnknown(usagePage, usage);
        }
        text = lookup[usage];
    }

    else if (usagePage == 0x0c)	// Consumer
    {
        static const char* lookup[] = {
            0, "Consumer Control", "Numeric Key Pad", "Programmable Buttons", "Microphone", "Headphone", "Graphic Equalizer", 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "+10", "+100", "AM/PM", 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Power", "Reset", "Sleep", "Sleep After", "Sleep Mode", "Illumination", "Function Buttons", 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Menu", "Menu Pick", "Menu Up", "Menu Down", "Menu Left", "Menu Right", "Menu Escape", "Menu Value Increase",
            "Menu Value Decrease", 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Data On Screen", "Closed Caption", "Closed Caption Select", "VCR/TV", "Broadcast Mode", "Snapshot", "Still", "Picture-in-Picture Toggle",
            "Picture-in-Picture Swap", "Red Menu Button", "Green Menu Button", "Blue Menu Button", "Yellow Menu Button", "Aspect", "3D Mode Select", "Display Brightness Increment",
            "Display Brightness Decrement", "Display Brightness", "Display Backlight Toggle", "Display Set Brightness to Minimum", "Display Set Brightness to Maximum", "Display Set Auto Brightness", 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, "Assign Selection", "Mode Step", "Recall Last", "Enter Channel", "Order Movie", "Channel", 0,
            "Media Select Computer", "Media Select TV", "Media Select WWW", "Media Select DVD", "Media Select Telephone", "Media Select Program Guide", "Media Select Video Phone", "Media Select Games",
            "Media Select Messages", "Media Select CD", "Media Select VCR", "Media Select Tuner", "Quit", "Help", "Media Select Tape", "Media Select Cable",
            "Media Select Satellite", "Media Select Security", "Media Select Home", "Media Select Call", "Channel Increment", "Channel Decrement", "Media Select SAP", 0,
            "VCR Plus", "Once", "Daily", "Weekly", "Monthly", 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Play", "Pause", "Record", "Fast Forward", "Rewind", "Scan Next Track", "Scan Previous Track", "Stop",
            "Eject", "Random Play", "Select Disc", "Enter Disc", "Repeat", "Tracking", "Track Normal", "Slow Tracking",
            "Frame Forward", "Frame Back", "Mark", "Clear Mark", "Repeat From Mark", "Return To Mark", "Search Mark Forward", "Search Mark Backwards",
            "Counter Reset", "Show Counter", "Tracking Increment", "Tracking Decrement", "Stop/Eject", "Play/Pause", "Play/Skip", "Voice Command",
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Volume", "Balance", "Mute", "Bass", "Treble", "Bass Boost", "Surround Mode", "Loudness",
            "MPX", "Volume Increment", "Volume Decrement", 0, 0, 0, 0, 0,
            "Speed Select", "Playback Speed", "Standard Play", "Long Play", "Extended Play", "Slow", 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Fan Enable", "Fan Speed", "Light Enable", "Light Illumination Level", "Climate Control Enable", "Room Temperature", "Security Enable", "Fire Alarm",
            "Police Alarm", "Proximity", "Motion", "Duress Alarm", "Holdup Alarm", "Medical Alarm", 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Balance Right", "Balance Left", "Bass Increment", "Bass Decrement", "Treble Increment", "Treble Decrement", 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Speaker System", "Channel Left", "Channel Right", "Channel Center", "Channel Front", "Channel Center Front", "Channel Side", "Channel Surround",
            "Channel Low Frequency Enhancement", "Channel Top", "Channel Unknown", 0, 0, 0, 0, 0,
            "Sub-channel", "Sub-channel Increment", "Sub-channel Decrement", "Alternate Audio Increment", "Alternate Audio Decrement", 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Application Launch Buttons", "AL Launch Button Configuration Tool", "AL Programmable Button Configuration", "AL Consumer Control Configuration", "AL Word Processor", "AL Text Editor", "AL Spreadsheet", "AL Graphics Editor",
            "AL Presentation App", "AL Database App", "AL Email Reader", "AL Newsreader", "AL Voicemail", "AL Contacts/Address Book", "AL Calendar/Schedule", "AL Task/Project Manager",
            "AL Log/Journal/Timecard", "AL Checkbook/Finance", "AL Calculator", "AL A/V Capture/Playback", "AL Local Machine Browser", "AL LAN/WAN Browser", "AL Internet Browser", "AL Remote Networking/ISP Connect",
            "AL Network Conference", "AL Network Chat", "AL Telephony/Dialer", "AL Logon", "AL Logoff", "AL Logon/Logoff", "AL Terminal Lock/Screensaver", "AL Control Panel",
            "AL Command Line Processor/Run", "AL Process/Task Manager", "AL Select Task/Application", "AL Next Task/Application", "AL Previous Task/Application", "AL Preemptive Halt Task/Application", "AL Integrated Help Center", "AL Documents",
            "AL Thesaurus", "AL Dictionary", "AL Desktop", "AL Spell Check", "AL Grammar Check", "AL Wireless Status", "AL Keyboard Layout", "AL Virus Protection",
            "AL Encryption", "AL Screen Saver", "AL Alarms", "AL Clock", "AL File Browser", "AL Power Status", "AL Image Browser", "AL Audio Browser",
            "AL Movie Browser", "AL Digital Rights Manager", "AL Digital Wallet", 0, "AL Instant Messaging", "AL OEM Features/Tips/Tutorial Browser", "AL OEM Help", "AL Online Community",
            "AL Entertainment Content Browser", "AL Online Shopping Browser", "AL SmartCard Information/Help", "AL Market Monitor/Finance Browser", "AL Customized Corporate News Browser", "AL Online Activity Browser", "AL Research/Search Browser", "AL Audio Player",
            "AL Message Status", "AL Contact Sync", 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Generic GUI Application Controls", "AC New", "AC Open", "AC Close", "AC Exit", "AC Maximize", "AC Minimize", "AC Save",
            "AC Print", "AC Properties", "AC Undo", "AC Copy", "AC Cut", "AC Paste", "AC Select All", "AC Find",
            "AC Find and Replace", "AC Search", "AC Go To", "AC Home", "AC Back", "AC Forward", "AC Stop", "AC Refresh",
            "AC Previous Link", "AC Next Link", "AC Bookmarks", "AC History", "AC Subscriptions", "AC Zoom In", "AC Zoom Out", "AC Zoom",
            "AC Full Screen View", "AC Normal View", "AC View Toggle", "AC Scroll Up", "AC Scroll Down", "AC Scroll", "AC Pan Left", "AC Pan Right",
            "AC Pan", "AC New Window", "AC Tile Horizontally", "AC Tile Vertically", "AC Format", "AC Edit", "AC Bold", "AC Italics",
            "AC Underline", "AC Strikethrough", "AC Subscript", "AC Superscript", "AC All Caps", "AC Rotate", "AC Resize", "AC Flip horizontal",
            "AC Flip Vertical", "AC Mirror Horizontal", "AC Mirror Vertical", "AC Font Select", "AC Font Color", "AC Font Size", "AC Justify Left", "AC Justify Center H",
            "AC Justify Right", "AC Justify Block H", "AC Justify Top", "AC Justify Center V", "AC Justify Bottom", "AC Justify Block V", "AC Indent Decrease", "AC Indent Increase",
            "AC Numbered List", "AC Restart Numbering", "AC Bulleted List", "AC Promote", "AC Demote", "AC Yes", "AC No", "AC Cancel",
            "AC Catalog", "AC Buy/Checkout", "AC Add to Cart", "AC Expand", "AC Expand All", "AC Collapse", "AC Collapse All", "AC Print Preview",
            "AC Paste Special", "AC Insert Mode", "AC Delete", "AC Lock", "AC Unlock", "AC Protect", "AC Unprotect", "AC Attach Comment",
            "AC Delete Comment", "AC View Comment", "AC Select Word", "AC Select Sentence", "AC Select Paragraph", "AC Select Column", "AC Select Row", "AC Select Table",
            "AC Select Object", "AC Redo/Repeat", "AC Sort", "AC Sort Ascending", "AC Sort Descending", "AC Filter", "AC Set Clock", "AC View Clock",
            "AC Select Time Zone", "AC Edit Time Zones", "AC Set Alarm", "AC Clear Alarm", "AC Snooze Alarm", "AC Reset Alarm", "AC Synchronize", "AC Send/Receive",
            "AC Send To", "AC Reply", "AC Reply All", "AC Forward Msg", "AC Send", "AC Attach File", "AC Upload", "AC Download (Save Target As)",
            "AC Set Borders", "AC Insert Row", "AC Insert Column", "AC Insert File", "AC Insert Picture", "AC Insert Object", "AC Insert Symbol", "AC Save and Close",
            "AC Rename", "AC Merge", "AC Split", "AC Distribute Horizontally", "AC Distribute Vertically", 0, 0, 0,
            "AC Soft Key Left", "AC Soft Key Right", 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "AC Idle Keep Alive", 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Extended Keyboard Attributes Collection", "Keyboard Form Factor", "Keyboard Key Type", "Keyboard Physical Layout", "Vendor-Specific Keyboard Physical Layout", "Keyboard IETF Language Tag Index", "Implemented Keyboard Input Assist Controls", "Keyboard Input Assist Previous",
            "Keyboard Input Assist Next", "Keyboard Input Assist Previous Group", "Keyboard Input Assist Next Group", "Keyboard Input Assist Accept", "Keyboard Input Assist Cancel", 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Contact Edited", "Contact Added", "Contact Record Active", "Contact Index", "Contact Nickname", "Contact First Name", "Contact Last Name", "Contact Full Name",
            "Contact Phone Number Personal", "Contact Phone Number Business", "Contact Phone Number Mobile", "Contact Phone Number Pager", "Contact Phone Number Fax", "Contact Phone Number Other", "Contact Email Personal", "Contact Email Business",
            "Contact Email Other", "Contact Email Main", "Contact Speed Dial Number", "Contact Status Flag", "Contact Misc.", 0, 0, 0,
        };
        if(usage >= (sizeof(lookup) / sizeof(const char *))){
            return getUnknown(usagePage, usage);
        }
        text = lookup[usage];;
    }

    else if (usagePage == 0x0d)	// Digitizers
    {
        static const char* lookup[] = {
            0, "Digitizer", "Pen", "Light Pen", "Touch Screen", "Touch Pad", "White Board", "Coordinate Measuring",
            "3D Digitizer", "Stereo Plotter", "Articulated Arm", "Armature", "Multiple Point Digitizer", "Free Space Wand", "Device configuration", 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Stylus", "Puck", "Finger", "Device settings", 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Tip Pressure", "Barrel Pressure", "In Range", "Touch", "Untouch", "Tap", "Quality", "Data Valid",
            "Transducer Index", "Tablet Function Keys", "Program Change Keys", "Battery Strength", "Invert", "X Tilt", "Y Tilt", "Azimuth",
            "Altitude", "Twist", "Tip Switch", "Secondary Tip Switch", "Barrel Switch", "Eraser", "Tablet Pick", "Touch Valid",
            "Width", "Height", 0, 0, 0, 0, 0, 0,
            0, "Contact identifier", "Device mode", "Device identifier", "Contact count", "Contact count maximum", 0, 0,
            0, 0, "Secondary Barrel Switch", "Transducer Serial Number", 0, 0, 0, 0,
        };
        if(usage >= (sizeof(lookup) / sizeof(const char *))){
            return getUnknown(usagePage, usage);
        }
        text = lookup[usage];

    } else if (usagePage == 0x14) {	// Alphanumeric Display / Auxiliary Display
        static const char* lookup[] = {
            0, "Alphanumeric Display", "Bitmapped/Auxiliary Display", 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Display Attributes Report", "ASCII Character Set", "Data Read Back", "Font Read Back", "Display Control Report", "Clear Display", "Display Enable", "Screen Saver Delay",
            "Screen Saver Enable", "Vertical Scroll", "Horizontal Scroll", "Character Report", "Display Data", "Display Status", "Stat Not Ready", "Stat Ready",
            "Err Not a loadable character", "Err Font data cannot be read", "Cursor Position Report", "Row", "Column", "Rows", "Columns", "Cursor Pixel Positioning",
            "Cursor Mode", "Cursor Enable", "Cursor Blink", "Font Report", "Font Data", "Character Width", "Character Height", "Character Spacing Horizontal",
            "Character Spacing Vertical", "Unicode Character Set", "Font 7-Segment", "7-Segment Direct Map", "Font 14-Segment", "14-Segment Direct Map", "Display Brightness", "Display Contrast",
            "Character Attribute", "Attribute Readback", "Attribute Data", "Char Attr Enhance", "Char Attr Underline", "Char Attr Blink", 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Bitmap Size X", "Bitmap Size Y", "Max Blit Size", "Bit Depth Format", "Display Orientation", "Palette Report", "Palette Data Size", "Palette Data Offset",
            "Palette Data", "Blit Report", "Blit Rectangle X1", "Blit Rectangle Y1", "Blit Rectangle X2", "Blit Rectangle Y2", "Blit Data", "Soft Button",
            "Soft Button ID", "Soft Button Side", "Soft Button Offset 1", "Soft Button Offset 2", "Soft Button Report", 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, "Soft Keys", 0, 0, 0, 0, 0,
            0, 0, 0, 0, "Display Data Extensions", 0, 0, "Character Mapping",
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, "Unicode Equivalent", 0, "Character Page Mapping",
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, "Request Report",
        };
        if(usage >= (sizeof(lookup) / sizeof(const char *))){
            return getUnknown(usagePage, usage);
        }
        text = lookup[usage];
    }

    else if (usagePage == 0x20)	// Sensor
    {
        static const char* lookup[] = {
            0, "Sensor", 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Biometric", "Biometric: Human Presence", "Biometric: Human Proximity", "Biometric: Human Touch", 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Electrical", "Electrical: Capacitance", "Electrical: Current", "Electrical: Power", "Electrical: Inductance", "Electrical: Resistance", "Electrical: Voltage", "Electrical: Potentiometer",
            "Electrical: Frequency", "Electrical: Period", 0, 0, 0, 0, 0, 0,
            "Environmental", "Environmental: Atmospheric Pressure", "Environmental: Humidity", "Environmental: Temperature", "Environmental: Wind Direction", "Environmental: Wind Speed", 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Light", "Light: Ambient Light", "Light: Consumer Infrared", 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Location", "Location: Broadcast", "Location: Dead Reckoning", "Location: GPS (Global Positioning System)", "Location: Lookup", "Location: Other", "Location: Static", "Location: Triangulation",
            0, 0, 0, 0, 0, 0, 0, 0,
            "Mechanical", "Mechanical: Boolean Switch", "Mechanical: Boolean Switch Array", "Mechanical: Multivalue Switch", "Mechanical: Force", "Mechanical: Pressure", "Mechanical: Strain", "Mechanical: Weight",
            "Mechanical: Haptic Vibrator", "Mechanical: Hall Effect Switch", 0, 0, 0, 0, 0, 0,
            "Motion", "Motion: Accelerometer 1D", "Motion: Accelerometer 2D", "Motion: Accelerometer 3D", "Motion: Gyrometer 1D", "Motion: Gyrometer 2D", "Motion: Gyrometer 3D", "Motion: Motion Detector",
            "Motion: Speedometer", "Motion: Accelerometer", "Motion: Gyrometer", 0, 0, 0, 0, 0,
            "Orientation", "Orientation: Compass 1D", "Orientation: Compass 2D", "Orientation: Compass 3D", "Orientation: Inclinometer 1D", "Orientation: Inclinometer 2D", "Orientation: Inclinometer 3D", "Orientation: Distance 1D",
            "Orientation: Distance 2D", "Orientation: Distance 3D", "Orientation: Device Orientation", "Orientation: Compass", "Orientation: Inclinometer", "Orientation: Distance", 0, 0,
            "Scanner", "Scanner: Barcode", "Scanner: RFID", "Scanner: NFC", 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Time", "Time: Alarm Timer", "Time: Real Time Clock", 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Other", "Other: Custom", "Other: Generic", "Other: Generic Enumerator", 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
        };

        auto usageWithoutModifier = usage & 0xffU;
        if(usageWithoutModifier >= (sizeof(lookup) / sizeof(const char *))){
            return getUnknown(usagePage, usage);
        }
        text = lookup[usageWithoutModifier];
    }

    else if (usagePage == 0x40)	// Medical Instrument
    {
        static const char* lookup[] = {
            0, "Medical Ultrasound", 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "VCR/Acquisition", "Freeze/Thaw", "Clip Store", "Update", "Next", "Save", "Print", "Microphone Enable",
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Cine", "Transmit Power", "Volume", "Focus", "Depth", 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Soft Step - Primary", "Soft Step - Secondary", 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Depth Gain Compensation", 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Zoom Select", "Zoom Adjust", "Spectral Doppler Mode Select", "Spectral Doppler Adjust", "Color Doppler Mode Select", "Color Doppler Adjust", "Motion Mode Select", "Motion Mode Adjust",
            "2-D Mode Select", "2-D Mode Adjust", 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Soft Control Select", "Soft Control Adjust", 0, 0, 0, 0, 0, 0,
        };
        if(usage >= (sizeof(lookup) / sizeof(const char *))){
            return getUnknown(usagePage, usage);
        }
        text = lookup[usage];

    } else if (usagePage == 0x84) {	// Power Device
        static const char* lookup[] = {
            0, "iName", "PresentStatus", "ChangedStatus", "UPS", "PowerSupply", 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "BatterySystem", "BatterySystemID", "Battery", "BatteryID", "Charger", "ChargerID", "PowerConverter", "PowerConverterID",
            "OutletSystem", "OutletSystemID", "Input", "InputID", "Output", "OutputID", "Flow", "FlowID",
            "Outlet", "OutletID", "Gang", "GangID", "Sink", "SinkID", 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Voltage", "Current", "Frequency", "ApparentPower", "ActivePower", "PercentLoad", "Temperature", "Humidity",
            "BadCount", 0, 0, 0, 0, 0, 0, 0,
            "ConfigVoltage", "ConfigCurrent", "ConfigFrequency", "ConfigApparentPower", "ConfigActivePower", "ConfigPercentLoad", "ConfigTemperature", "ConfigHumidity",
            0, 0, 0, 0, 0, 0, 0, 0,
            "SwitchOnControl", "SwitchOffControl", "ToggleControl", "LowVoltageTransfer", "HighVoltageTransfer", "DelayBeforeReboot", "DelayBeforeStartup", "DelayBeforeShutdown",
            "Test", "ModuleReset", "AudibleAlarmControl", 0, 0, 0, 0, 0,
            "Present", "Good", "InternalFailure", "VoltageOutOfRange", "FrequencyOutOfRange", "Overload", "OverCharged", "OverTemperature",
            "ShutdownRequested", "ShutdownImminent", 0, "SwitchOn/Off", "Switchable", "Used", "Boost", "Buck",
            "Initialized", "Tested", "AwaitingPower", "CommunicationLost", 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, "iManufacturer", "iProduct", "iSerialNumber",
        };
        if(usage >= (sizeof(lookup) / sizeof(const char *))){
            return getUnknown(usagePage, usage);
        }
        text = lookup[usage];

    } else if (usagePage == 0x85) {	// Battery System
        static const char* lookup[] = {
            0, "SMBBatteryMode", "SMBBatteryStatus", "SMBAlarmWarning", "SMBChargerMode", "SMBChargerStatus", "SMBChargerSpecInfo", "SMBSelectorState",
            "SMBSelectorPresets", "SMBSelectorInfo", 0, 0, 0, 0, 0, 0,
            "OptionalMfgFunction1", "OptionalMfgFunction2", "OptionalMfgFunction3", "OptionalMfgFunction4", "OptionalMfgFunction5", "ConnectionToSMBus", "OutputConnection", "ChargerConnection",
            "BatteryInsertion", "Usenext", "OKToUse", "BatterySupported", "SelectorRevision", "ChargingIndicator", 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "ManufacturerAccess", "RemainingCapacityLimit", "RemainingTimeLimit", "AtRate", "CapacityMode", "BroadcastToCharger", "PrimaryBattery", "ChargeController",
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "TerminateCharge", "TerminateDischarge", "BelowRemainingCapacityLimit", "RemainingTimeLimitExpired", "Charging", "Discharging", "FullyCharged", "FullyDischarged",
            "ConditioningFlag", "AtRateOK", "SMBErrorCode", "NeedReplacement", 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "AtRateTimeToFull", "AtRateTimeToEmpty", "AverageCurrent", "Maxerror", "RelativeStateOfCharge", "AbsoluteStateOfCharge", "RemainingCapacity", "FullChargeCapacity",
            "RunTimeToEmpty", "AverageTimeToEmpty", "AverageTimeToFull", "CycleCount", 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "BattPackModelLevel", "InternalChargeController", "PrimaryBatterySupport", "DesignCapacity", "SpecificationInfo", "ManufacturerDate", "SerialNumber", "iManufacturerName",
            "iDevicename", "iDeviceChemistry", "ManufacturerData", "Rechargeable", "WarningCapacityLimit", "CapacityGranularity1", "CapacityGranularity2", "iOEMInformation",
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "InhibitCharge", "EnablePolling", "ResetToZero", 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "ACPresent", "BatteryPresent", "PowerFail", "AlarmInhibited", "ThermistorUnderRange", "ThermistorHot", "ThermistorCold", "ThermistorOverRange",
            "VoltageOutOfRange", "CurrentOutOfRange", "CurrentNotRegulated", "VoltageNotRegulated", "MasterMode", 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "ChargerSelectorSupport", "ChargerSpec", "Level2", "Level3", 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
        };
        if(usage >= (sizeof(lookup) / sizeof(const char *))){
            return getUnknown(usagePage, usage);
        }
        text = lookup[usage];

    } else if (usagePage == 0x90) {	// Camera Control
        static const char* lookup[] = {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Camera Auto-focus", "Camera Shutter", 0, 0, 0, 0, 0, 0,
        };
        if(usage >= (sizeof(lookup) / sizeof(const char *))){
            return getUnknown(usagePage, usage);
        }
        text = lookup[usage];

    } else if (usagePage == 0xff) {	// Fn key on Apple Keyboards
        static const char* lookup[] = {
            0, 0, 0, "Fn key", 0, 0, 0, 0,
        };
        if(usage >= (sizeof(lookup) / sizeof(const char *))){
            return getUnknown(usagePage, usage);
        }
        text = lookup[usage];

    } else if (usagePage == 0xf1d0) { // Fast IDentity Online Alliance
        static const char* lookup[] = {
            0, "U2F Authenticator Device", 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            "Input Report Data", "Output Report Data", 0, 0, 0, 0, 0, 0,
        };
        if(usage >= (sizeof(lookup) / sizeof(const char *))){
            return getUnknown(usagePage, usage);
        }
        text = lookup[usage];

    } else if (usagePage >= 0xff00) {
        return getVendorDefined(usagePage, usage);
    }

    if (!text){
        return getUnknown(usagePage, usage);
    }

    return text;
}
