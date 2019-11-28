#ifndef ARTOUR_SYMBOLS_H
#define ARTOUR_SYMBOLS_H

#include "art/shim/art_jit.h"

namespace art {
    namespace jit {

        typedef art::jit::JitCompiler *(*t_jitLoad)(bool *);

        typedef bool (*t_jitCompileMethod)(art::jit::JitCompiler *jit_compiler_handle, void *art_method, void *thread, bool osr);

        struct Symbols {

            static const char *LibArtPath;
            static const char *LibJitPath;

            static t_jitLoad JitLoad;
            static t_jitCompileMethod JitCompileMethod;

        };

        void initSymbols();

    }
}


#endif //ARTOUR_SYMBOLS_H
