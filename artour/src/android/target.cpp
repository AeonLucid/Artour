#include "target.h"
#include "utils/logging.h"

#include <cstdint>
#include <cstdlib>
#include <sys/system_properties.h>

int SDK_INT = 0;

static inline int32_t getApiLevel() {
    char prop_value[PROP_VALUE_MAX];
    __system_property_get("ro.build.version.sdk", prop_value);
    return atoi(prop_value);
}

void initApiLevel() {
    SDK_INT = getApiLevel();

    if (SDK_INT < SDK_MIN || SDK_INT > SDK_MAX) {
        LOGE("Unsupported SDK %u", SDK_INT);
        abort();
    }

    LOGD("Running on SDK %u", SDK_INT);
}
