#ifndef LIBHIDX_LIBHIDXFACTORY_HH
#define LIBHIDX_LIBHIDXFACTORY_HH

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


#endif //LIBHIDX_LIBHIDXFACTORY_HH
