#include "utils/dlfcn_custom.h"
#include "utils/logging.h"
#include "android/target.h"
#include <dlfcn.h>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <link.h>

extern int SDK_INT;

inline bool isWorkaroundRequired() {
    return SDK_INT >= ANDROID_N;
}

void *dlopen_compat(const char *path, int flags, bool *is_fake) {
    if (isWorkaroundRequired()) {
        *is_fake = true;

        // Variables.
        FILE *maps;
        struct ElfSummary *summary;
        char buf[PATH_MAX];
        uint64_t lib_addr;
        size_t lib_size;
        int fd;
        Elf_Ehdr *elf;
        char *section_header_ptr;
        bool found = false;

        // Find library in memory.
        maps = fopen("/proc/self/maps", "re");

        if (!maps) {
            LOGE("Failed to open /proc/self/maps.");
            abort();
        }

        while(!feof(maps)) {
            if (fgets(buf, sizeof(buf), maps) == nullptr) {
                break;
            }

            if ((strstr(buf, "r-xp") || strstr(buf, "r--p")) && strstr(buf, path)) {
                found = true;
                break;
            }
        }

        fclose(maps);

        if (!found) {
            LOGE("Library %s was not found in memory.", path);
            return nullptr;
        }

        if (sscanf(buf, "%llx", &lib_addr) != 1) {
            LOGE("Failed to read load address for library %s.", path);
            return nullptr;
        }

        // Load a copy of the library into memory.
        fd = open(path, O_RDONLY | O_CLOEXEC);

        if (fd < 0) {
            LOGE("Failed to open library %s.", path);
            return nullptr;
        }

        lib_size = static_cast<size_t>(lseek(fd, 0, SEEK_END));

        if (lib_size <= 0) {
            LOGE("Failed to seek library %s.", path);
            close(fd);
            return nullptr;
        }

        elf = reinterpret_cast<Elf32_Ehdr *>(mmap(nullptr, lib_size, PROT_READ, MAP_SHARED, fd, 0));
        close(fd);

        if (elf == MAP_FAILED) {
            LOGE("Failed to mmap library %s.", path);
            return nullptr;
        }

        // Copy interesting info from the elf file.
        summary = (struct ElfSummary *) calloc(1, sizeof(struct ElfSummary));

        if (!summary) {
            LOGE("Failed to allocate memory for %s.", path);
            munmap(elf, lib_size);
            return nullptr;
        }

        summary->base_addr = reinterpret_cast<void *>(lib_addr);
        section_header_ptr = ((char *) elf) + elf->e_shoff;

        for (int i = 0; i < elf->e_shnum; ++i, section_header_ptr += elf->e_shentsize) {
            auto section_header = reinterpret_cast<Elf_Shdr *>(section_header_ptr);

            switch (section_header->sh_type) {
                case SHT_DYNSYM:
                    if (summary->dynsym) {
                        break;
                    }

                    summary->dynsym = static_cast<Elf32_Sym *>(malloc(section_header->sh_size));
                    summary->dynsym_num = section_header->sh_size / sizeof(Elf_Sym);

                    if (!summary->dynsym) {
                        LOGE("Failed to allocate memory for dynsym %s.", path);

                        munmap(elf, lib_size);

                        if (summary->dynstr) {
                            free(summary->dynstr);
                        }

                        return nullptr;
                    }

                    memcpy(summary->dynsym, ((char *) elf) + section_header->sh_offset, section_header->sh_size);
                    break;

                case SHT_STRTAB:
                    if (summary->dynstr) {
                        break;
                    }

                    summary->dynstr = static_cast<char *>(malloc(section_header->sh_size));

                    if (!summary->dynstr) {
                        LOGE("Failed to allocate memory for dynstr %s.", path);

                        munmap(elf, lib_size);

                        if (summary->dynsym) {
                            free(summary->dynsym);
                        }

                        return nullptr;
                    }

                    memcpy(summary->dynstr, ((char *) elf) + section_header->sh_offset, section_header->sh_size);
                    break;

                case SHT_PROGBITS:
                    if (!summary->dynstr || !summary->dynsym) {
                        break;
                    }

                    summary->bias = (off_t) (section_header->sh_addr - section_header->sh_offset);

                    // Stop for loop.
                    i = elf->e_shnum;
                    break;

                default:
                    break;
            }
        }

        munmap(elf, lib_size);

        if (!summary->dynstr || !summary->dynsym) {
            LOGE("Dynamic sections not found in %s.", path);

            if (summary->dynstr) {
                free(summary->dynstr);
            }

            if (summary->dynsym) {
                free(summary->dynsym);
            }

            return nullptr;
        }

        return summary;
    } else {
        *is_fake = false;
        return dlopen(path, flags);
    }
}

void *dlsym_compat(void *handle, const char *name) {
    if (isWorkaroundRequired()) {
        if (handle != nullptr) {
            auto summary = reinterpret_cast<ElfSummary *>(handle);
            auto symbol = summary->dynsym;

            for (int i = 0; i < summary->dynsym_num; ++i, symbol++) {
                if (strcmp(summary->dynstr + symbol->st_name, name) == 0) {
                    return (char *) summary->base_addr + symbol->st_value - summary->bias;
                }
            }
        }
    } else {
        return dlsym(handle, name);
    }

    return nullptr;
}

void dlclose_compat(void *handle) {
    if (isWorkaroundRequired()) {
        if (handle != nullptr) {
            auto summary = reinterpret_cast<ElfSummary *>(handle);

            if (summary->dynstr) {
                free(summary->dynstr);
            }

            if (summary->dynsym) {
                free(summary->dynsym);
            }

            free(summary);
        }
    } else {
        dlclose(handle);
    }
}
