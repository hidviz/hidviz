#ifndef HIDVIZ_LIBHIDXFACTORY_HH
#define HIDVIZ_LIBHIDXFACTORY_HH

#include "libhidx/LibHidx.hh"

namespace libhidx {

    class LibHidxFactory {
    public:
        static LibHidx& get() {
            static LibHidx instance;

            return instance;
        }
    };

}


#endif //HIDVIZ_LIBHIDXFACTORY_HH
