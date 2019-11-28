#include "symbols.h"
#include "android/target.h"
#include "utils/dlfcn_custom.h"
#include "utils/logging.h"
#include <dlfcn.h>
#include <cstdlib>

extern int SDK_INT;

namespace art {
    namespace jit {

        const char* Symbols::LibArtPath;
        const char* Symbols::LibJitPath;

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

            LOGI("Found %p", Symbols::JitLoad);

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
