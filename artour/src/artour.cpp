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

bool ArtourInit() {
    return true;
}