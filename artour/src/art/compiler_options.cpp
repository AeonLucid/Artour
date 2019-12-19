#include "compiler_options.h"
#include "android/target.h"

extern int SDK_INT;

namespace art {

    size_t CompilerOptions::getInlineMaxCodeUnitsOffset() {
        if (SDK_INT < ANDROID_N)
            return sizeof(art::CompilerOptions) + 1;

        if (SDK_INT >= ANDROID_O) {
            return 6 * sizeof(size_t);
        } else {
            return 7 * sizeof(size_t);
        }
    }

    size_t CompilerOptions::getInlineMaxCodeUnits() {
        return *(size_t *) (this + this->getInlineMaxCodeUnitsOffset());
    }

    bool CompilerOptions::setInlineMaxCodeUnits(size_t units) {
        *(size_t *) (this + this->getInlineMaxCodeUnitsOffset()) = units;
        return true;
    }

    bool CompilerOptions::disableJitInline() {
        size_t originOptions = this->getInlineMaxCodeUnits();
        //maybe a real inlineMaxCodeUnits
        if (originOptions > 0 && originOptions <= 1024) {
            this->setInlineMaxCodeUnits(0);
            return true;
        } else {
            return false;
        }
    }

}