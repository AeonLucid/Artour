#ifndef ARTOUR_ART_METHOD_OFFSETS_H
#define ARTOUR_ART_METHOD_OFFSETS_H

namespace art {

    struct ArtMethodOffsets {
        size_t method_size;
        size_t jni_code_offset;
        size_t quick_code_offset;
        size_t interpreter_code_offset;
        size_t access_flags_offset;
        size_t dex_code_item_offset_offset;
        size_t dex_method_index_offset;
        size_t method_index_offset;
        size_t hotness_count_offset;
    };

}

#endif //ARTOUR_ART_METHOD_OFFSETS_H
