#include "art/art_runtime.h"
#include "art/modifiers.h"
#include "utils/logging.h"
#include "utils/memory.h"
#include "android/target.h"

extern int SDK_INT;

namespace art {

    ArtRuntime *ArtRuntime::Get() {
        static ArtRuntime instance;
        return &instance;
    }

    bool ArtRuntime::Initialize(JNIEnv *env) {
        // Find libandroid_runtime.so in memory.
        uintptr_t runtime_start = 0;
        uintptr_t runtime_end = 0;

        if (!FindLibrary("libandroid_runtime.so", &runtime_start, &runtime_end)) {
            LOGE("Unable to find libandroid_runtime.so.");
            return false;
        }

        if (runtime_start == 0 || runtime_end == 0) {
            LOGE("Invalid result for libandroid_runtime.so.");
            return false;
        }

        // Find jni_code_offset and access_flags_offset.
        jclass process = env->FindClass("android/os/Process");
        jmethodID set_arg_v0 = env->GetStaticMethodID(process, "setArgV0", "(Ljava/lang/String;)V");

        uint32_t expected_access_flags = kAccPublic | kAccStatic | kAccFinal | kAccNative;
        uint32_t all_flags_except_public_api = ~kAccPublicApi >> 0U;

        size_t jni_code_offset = INT32_MAX;
        size_t access_flags_offset = INT32_MAX;
        uint32_t remaining = 2;

        for (size_t offset = 0; offset != 64 && remaining != 0; offset += 4) {
            if (jni_code_offset == INT32_MAX) {
                auto addr = MemberOf<uintptr_t>(set_arg_v0, offset);
                if (addr >= runtime_start && addr < runtime_end) {
                    jni_code_offset = offset;
                    remaining--;
                }
            }

            if (access_flags_offset == INT32_MAX) {
                auto flags = MemberOf<uint32_t>(set_arg_v0, offset);
                if ((flags & all_flags_except_public_api) == expected_access_flags) {
                    access_flags_offset = offset;
                    remaining--;
                }
            }
        }

        env->DeleteLocalRef(process);

        if (jni_code_offset == INT32_MAX) {
            LOGE("Unable to find jni_code_offset.");
            return false;
        } else if (access_flags_offset == INT32_MAX) {
            LOGE("Unable to find access_flags_offset.");
            return false;
        }

        // Store offsets.
        size_t entrypoint_field_size = (SDK_INT <= ANDROID_LOLLIPOP) ? 8 : kPointerSize;
        size_t quick_code_offset = jni_code_offset + entrypoint_field_size;

        this->method_offsets.method_size = (SDK_INT < ANDROID_LOLLIPOP) ? quick_code_offset + 32 : quick_code_offset + kPointerSize;
        this->method_offsets.jni_code_offset = jni_code_offset;
        this->method_offsets.quick_code_offset = quick_code_offset;
        this->method_offsets.access_flags_offset = access_flags_offset;
        this->method_offsets.dex_code_item_offset_offset = access_flags_offset + sizeof(uint32_t);
        this->method_offsets.dex_method_index_offset = access_flags_offset + sizeof(uint32_t) * 2;
        this->method_offsets.method_index_offset = access_flags_offset + sizeof(uint32_t) * 3;

        return true;
    }

    bool ArtRuntime::HookMethod(JNIEnv *env, jobject method) {
        return false;
    }

}

