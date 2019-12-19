#ifndef ARTOUR_ART_METHOD_H
#define ARTOUR_ART_METHOD_H

#include <jni.h>
#include <cstddef>
#include "art/art_runtime.h"
#include "art/art_method_offsets.h"
#include "utils/memory.h"

namespace art {

    class ArtMethod {

    public:
        ArtMethod(jmethodID method) : method(method) {
            offsets = ArtRuntime::Get()->GetMethodOffsets();
        }

        uint32_t GetAccessFlags() {
            return MemberOf<uint32_t>(this->method, this->offsets->access_flags_offset);
        }

        void SetAccessFlags(uint32_t access_flags) {
            AssignOffset<uint32_t>(this->method, this->offsets->access_flags_offset, access_flags);
        }

        void *GetEntryPointFromQuickCompiledCode() {
            return MemberOf<void *>(this->method, this->offsets->quick_code_offset);
        }

        void SetEntryPointFromQuickCompiledCode(void *entry_point) {
            AssignOffset<void *>(this->method, this->offsets->quick_code_offset, entry_point);
        }

        void *GetEntryPointFromInterpreterCode() {
            return MemberOf<void *>(this->method, this->offsets->interpreter_code_offset);
        }

        void SetEntryPointFromInterpreterCode(void *entry_point) {
            AssignOffset<void *>(this->method, this->offsets->interpreter_code_offset, entry_point);
        }

        void *GetEntryPointFromJni() {
            return MemberOf<void *>(this->method, this->offsets->jni_code_offset);
        }

        void SetEntryPointFromJni(void *entry_point) {
            AssignOffset<void *>(this->method, this->offsets->jni_code_offset, entry_point);
        }

    public:
        jmethodID method;
        const ArtMethodOffsets *offsets;
    };

}

#endif //ARTOUR_ART_METHOD_H
