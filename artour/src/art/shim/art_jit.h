#ifndef ARTOUR_ART_JIT_H
#define ARTOUR_ART_JIT_H

#include "art/compiler_options.h"
#include <memory>

namespace art {
    namespace jit {

        class JitCompiler {
            virtual ~JitCompiler();

        public:
            std::unique_ptr<art::CompilerOptions> compilerOptions;
        };

        class Jit {

        };

    };
}

#endif //ARTOUR_ART_JIT_H
