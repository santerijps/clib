#ifndef SLICE_H
#define SLICE_H

#ifndef _STDINT_H
#include <stdint.h>
#endif

/**
 * A slice is a section of memory with a pointer to the beginning of memory and a length.
 * The struct doesn't contain any information of the size of each element.
 */
typedef struct Slice {
    void *ptr;
    size_t len;
} Slice;

/**
 * Initialize a slice from an array.
 * @param A The array to cut a slice from
 * @param S The starting index of the slice
 * @param E The last index of the slice (exclusive)
 * @return A new slice
 */
#define slice_from(A, S, E) (Slice) {.ptr = A + S, .len = E - S}

/**
 * @param T The type of the values to store
 * @param ... Comma-separated values
 * @return A new slice
 */
#define slice_create(T, ...) (Slice) {.ptr = (void*)(T[]){__VA_ARGS__}, .len = sizeof((T[]){__VA_ARGS__}) / sizeof(T)}

/**
 * Get a value from a slice at the specified index.
 * @param S Slice passed as a value
 * @param I Index to accesss in the slice
 * @param T Type of the value
 * @return Value stored at index
 */
#define slice_get(T, S, I) (((T*)S.ptr)[I])

/**
 * Cast the slice pointer to a specified type.
 * @param T Type to cast to
 * @param S Slice passed as value
 * @return Slice.ptr cast to `T`
 */
#define slice_as(T, S) ((T)((S).ptr))

/**
 * Get the slice's length.
 * @param S Slice passed as a value
 * @return Length of the slice
 */
#define slice_length(S) ((S).len)

/**
 * Copy the pointer with `memcpy`.
 * @param SliceValue Slice passed as value
 * @param DstPointer Destination to copy to
 * @param Type Cast the Slice pointer to the correct type
 */
#define slice_copy(SliceValue, DstPointer, Type)\
    memcpy(DstPointer, ((Type)(SliceValue).ptr), (SliceValue).len)

/**
 * Check if the two slices are equal.
 * @param s1 Pointer to the first slice
 * @param s2 Pointer to the second slice
 * @return 1 if equal, else 0
 */
int slice_equals(Slice *s1, Slice *s2) {
    if (s1->len != s2->len) {
        return 0;
    }
    for (size_t i = 0; i < s1->len; i += 1) {
        if (((char*)s1->ptr)[i] != ((char*)s2->ptr)[i]) {
            return 0;
        }
    }
    return 1;
}

/**
 * Check if the slice pointer equals to a string.
 * @param slice Pointer to slice
 * @param string String to compare against
 * @return 1 if equal, else 0
 */
int slice_equals_string(Slice *slice, const char *string) {
    size_t string_len = 0;
    for (size_t i = 0; string[i] != '\0'; i += 1) {
        string_len += 1;
    }
    if (slice->len != string_len) {
        return 0;
    }
    for (size_t i = 0; i < slice->len; i += 1) {
        if (((char*)slice->ptr)[i] != string[i]) {
            return 0;
        }
    }
    return 1;
}

/**
 * Check if the slice starts with the specified prefix.
 * @param slice Slice pointer
 * @param prefix Prefix string
 * @return 1 if true, 0 if false
 */
int slice_has_prefix(Slice *slice, const char *prefix) {
    size_t prefix_length = 0, i;
    for (i = 0; prefix[i] != '\0'; i += 1) {
        prefix_length += 1;
    }
    if (prefix_length > slice->len) {
        return 0;
    }
    for (i = 0; i < prefix_length; i += 1) {
        if (((char*)slice->ptr)[i] != prefix[i]) {
            return 0;
        }
    }
    return 1;
}

/**
 * Check if the slice ends with the specified suffix.
 * @param slice Slice pointer
 * @param suffix Suffix string
 * @return 1 if true, 0 if false
 */
int slice_has_suffix(Slice *slice, const char *suffix) {
    size_t suffix_length = 0, i;
    for (i = 0; suffix[i] != '\0'; i += 1) {
        suffix_length += 1;
    }
    if (suffix_length > slice->len) {
        return 0;
    }
    for (i = slice->len - suffix_length; i < slice->len; i += 1) {
        if (((char*)slice->ptr)[i] != suffix[i]) {
            return 0;
        }
    }
    return 1;
}

/**
 * Check if the slice contains the specified string.
 * @param slice Slice pointer
 * @param string Search string
 * @return 1 if true, 0 if false
 */
int slice_has_string(Slice *slice, const char *string) {
    size_t string_length = 0, i, j;
    for (i = 0; string[i] != '\0'; i += 1) {
        string_length += 1;
    }
    if (string_length > slice->len) {
        return 0;
    }
    for (i = 0; i < slice->len; i += 1) {
        for (j = 0; j < string_length && i + j < slice->len; j += 1) {
            if (((char*)slice->ptr)[i + j] != string[j]) {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

#endif
