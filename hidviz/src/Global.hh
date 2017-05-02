#ifndef PROJECT_GLOBAL_HH
#define PROJECT_GLOBAL_HH

namespace hidviz {
    struct Global {
        static constexpr const char* appName = "hidviz";

        struct Settings {
            static constexpr const char* hideInactiveUsages = "hideInactiveUsages";
            static constexpr const char* clampValues = "clampValues";
        };
    };
}

#endif
