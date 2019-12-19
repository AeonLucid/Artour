#ifndef ARTOUR_MEMORY_H
#define ARTOUR_MEMORY_H

#include <cstdio>

template<typename U, typename T>
U ForceCast(T *x) {
    return (U) (uintptr_t) x;
}

template<typename U, typename T>
U ForceCast(T &x) {
    return *(U *) &x;
}

template<typename T>
struct Identity {
    using type = T;
};

template<typename R>
static inline R OffsetOf(uintptr_t ptr, size_t offset) {
    return reinterpret_cast<R>(ptr + offset);
}

template<typename R>
static inline R OffsetOf(intptr_t ptr, size_t offset) {
    return reinterpret_cast<R>(ptr + offset);
}

template<typename R>
static inline R OffsetOf(void *ptr, size_t offset) {
    return (R) (reinterpret_cast<intptr_t>(ptr) + offset);
}

template<typename T>
static inline T MemberOf(void *ptr, size_t offset) {
    return *OffsetOf<T *>(ptr, offset);
}

template<typename T>
static inline void AssignOffset(void *ptr, size_t offset, T member) {
    *OffsetOf<T *>(ptr, offset) = member;
}

bool FindLibrary(const char *name, uintptr_t *out_begin, uintptr_t *out_end);

#endif //ARTOUR_MEMORY_H
