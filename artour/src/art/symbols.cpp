#include "symbols.h"
#include "android/target.h"
#include "art/compiler_options.h"
#include "utils/dlfcn_custom.h"
#include "utils/logging.h"
#include <dlfcn.h>
#include <cstdlib>

extern int SDK_INT;

namespace art {
    namespace jit {

        const char* Symbols::LibArtPath;
        const char* Symbols::LibJitPath;

        art::jit::JitCompiler **Symbols::JitCompilerHandleGlobal;
        art::jit::JitCompiler *Symbols::JitCompilerHandle;

        t_jitLoad Symbols::JitLoad;
        t_jitCompileMethod Symbols::JitCompileMethod;

        void initSymbols() {
            // Find the correct paths.
            if (kPointerSize == 8) {
                if (SDK_INT >= ANDROID_Q && file_exists("/apex/com.android.runtime/lib64/libart.so")) {
                    Symbols::LibArtPath = "/apex/com.android.runtime/lib64/libart.so";
                    Symbols::LibJitPath = "/apex/com.android.runtime/lib64/libart-compiler.so";
                } else {
                    Symbols::LibArtPath = "/system/lib64/libart.so";
                    Symbols::LibJitPath = "/system/lib64/libart-compiler.so";
                }
            } else {
                if (SDK_INT >= ANDROID_Q && file_exists("/apex/com.android.runtime/lib/libart.so")) {
                    Symbols::LibArtPath = "/apex/com.android.runtime/lib/libart.so";
                    Symbols::LibJitPath = "/apex/com.android.runtime/lib/libart-compiler.so";
                } else {
                    Symbols::LibArtPath = "/system/lib/libart.so";
                    Symbols::LibJitPath = "/system/lib/libart-compiler.so";
                }
            }

            LOGD("ArtLibPath: %s", Symbols::LibArtPath);
            LOGD("JitLibPath: %s", Symbols::LibJitPath);

            // Open libraries.
            bool lib_art_fake;
            void *lib_art = dlopen_compat(Symbols::LibArtPath, RTLD_LAZY | RTLD_GLOBAL, &lib_art_fake);

            bool lib_jit_fake;
            void *lib_jit = dlopen_compat(Symbols::LibJitPath, RTLD_LAZY | RTLD_GLOBAL, &lib_jit_fake);

            if (lib_art == nullptr || lib_jit == nullptr) {
                LOGE("Something went wrong while loading libart.so or libart-compiler.so.");

                if (lib_art_fake) {
                    dlclose_compat(lib_art);
                }

                if (lib_jit_fake) {
                    dlclose_compat(lib_jit);
                }

                abort();
            }

            // Resolve symbols.
            Symbols::JitLoad = reinterpret_cast<t_jitLoad>(dlsym_compat(lib_jit, "jit_load"));

            if (SDK_INT >= ANDROID_N) {
                Symbols::JitCompilerHandleGlobal = reinterpret_cast<art::jit::JitCompiler **>(dlsym_compat(lib_art, "_ZN3art3jit3Jit20jit_compiler_handle_E"));

                if (SDK_INT >= ANDROID_Q) {
                    // TODO:
                    LOGE("TODO Android Q..");
                    abort();
                } else {
                    Symbols::JitCompileMethod = reinterpret_cast<t_jitCompileMethod>(dlsym_compat(lib_jit, "jit_compile_method"));
                }

                Symbols::JitLoad = reinterpret_cast<t_jitLoad>(dlsym_compat(lib_jit, "jit_load"));

                if (Symbols::JitLoad != nullptr) {
                    bool generate_debug_info = false;
                    Symbols::JitCompilerHandle = Symbols::JitLoad(&generate_debug_info);
                    LOGD("Loaded JIT using jit_load.");
                } else {
                    Symbols::JitCompilerHandle = *Symbols::JitCompilerHandleGlobal;
                    LOGD("Loaded JIT using jit_compiler_handle.");
                }

                if (Symbols::JitCompilerHandle != nullptr) {
                    art::CompilerOptions* compilerOptions = Symbols::JitCompilerHandle->compilerOptions.get();
                    LOGD("Current JIT InlineMaxCodeUnits: %u", compilerOptions->getInlineMaxCodeUnits());

                    compilerOptions->disableJitInline();

                    LOGD("Disabled JIT inlining.");
                }
            }

            // Close libraries.
            if (lib_art_fake) {
                dlclose_compat(lib_art);
            }

            if (lib_jit_fake) {
                dlclose_compat(lib_jit);
            }
        }

    }
}
