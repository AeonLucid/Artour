#ifndef ARTOUR_DLFCN_CUSTOM_H
#define ARTOUR_DLFCN_CUSTOM_H

#include <sys/stat.h>
#include <elf.h>

#ifdef __LP64__
#define Elf_Ehdr Elf64_Ehdr
#define Elf_Shdr Elf64_Shdr
#define Elf_Sym  Elf64_Sym
#define Elf_Addr Elf64_Addr
#else
#define Elf_Ehdr Elf32_Ehdr
#define Elf_Shdr Elf32_Shdr
#define Elf_Sym  Elf32_Sym
#define Elf_Addr Elf32_Addr
#endif

struct ElfSummary {
    void *base_addr;
    char *dynstr;
    Elf_Sym *dynsym;
    int dynsym_num;
    off_t bias;
};

inline bool file_exists(const char *name) {
    struct stat buffer{};
    return (stat(name, &buffer) == 0);
}

void *dlopen_compat(const char *path, int flags, bool *is_fake);

void *dlsym_compat(void *handle, const char *name);

void dlclose_compat(void *handle);

#endif //ARTOUR_DLFCN_CUSTOM_H
