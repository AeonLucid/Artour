#include <art/art_runtime.h>
#include "artour.h"
#include "android/target.h"
#include "art/symbols.h"
#include "utils/macros.h"
#include "utils/logging.h"

extern int SDK_INT;

INIT void __initialize() {
    initApiLevel();
    art::jit::initSymbols();
}

bool ArtourInit(JNIEnv *env) {
    if (!art::ArtRuntime::Get()->Initialize(env)) {
        LOGE("Failed to initialize ArtRuntime.");
        return false;
    }

    LOGI("Initialized ArtRuntime, running on SDK %u.", SDK_INT);

    return true;
}