#include "utils/memory.h"
#include "utils/logging.h"
#include <cstdlib>
#include <cstring>
#include <climits>

bool FindLibrary(const char *name, uintptr_t *out_begin, uintptr_t *out_end) {
    FILE *maps;

    char buf[PATH_MAX];
    char perm[12] = {'\0'};
    char dev[12] = {'\0'};
    char mapname[PATH_MAX] = {'\0'};

    uintptr_t begin;
    uintptr_t end;
    uintptr_t inode;
    uintptr_t foo;

    bool found = false;

    maps = fopen("/proc/self/maps", "re");

    if (!maps) {
        LOGE("Failed to open /proc/self/maps.");
        abort();
    }

    while(!feof(maps)) {
        if (fgets(buf, sizeof(buf), maps) == nullptr) {
            break;
        }

        if (strstr(buf, name)) {
            sscanf(buf, "%lx-%lx %s %lx %s %ld %s", &begin, &end, perm, &foo, dev, &inode, mapname);

            found = true;
            *out_begin = begin;
            *out_end = end;
            break;
        }
    }

    fclose(maps);

    return found;
}
