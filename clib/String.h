#ifndef STRING_H
#define STRING_H

#ifndef _INC_STDLIB
#include <stdlib.h>
#endif

#ifndef _INC_STRING
#include <string.h>
#endif

#define string_last_index(String) ((String).len - 1)

typedef struct String {
    char *ptr;
    size_t cap, len;
} String;

void string_print(String *string) {
    printf("String{\"%s\", cap = %llu, len = %llu}\n", string->ptr, string->cap, string->len);
}

String string_make(size_t capacity) {
    if (capacity == 0) {
        capacity = 2;
    }
    return (String) {
        .ptr = calloc(capacity, 1),
        .cap = capacity,
        .len = 0,
    };
}

void string_init(String *string, size_t capacity) {
    if (string == NULL) {
        return;
    }
    *string = string_make(capacity);
}

void string_release(String *string) {
    if (string == NULL) {
        return;
    }
    free(string->ptr);
    string->cap = 0;
    string->len = 0;
}

void string_clear(String *string) {
    if (string == NULL) {
        return;
    }
    memset(string->ptr, 0, string->len);
    string->len = 0;
}

void string_grow(String *string, size_t required_capacity) {
    if (string == NULL) {
        return;
    }
    if (required_capacity >= string->cap) {
        while (required_capacity >= string->cap) {
            string->cap <<= 1;
        }
        string->ptr = realloc(string->ptr, string->cap);
        if (string->ptr == NULL) {
            return;
        }
        memset(string->ptr + string->len, 0, string->cap - string->len);
    }
}

void string_append_byte(String *string, char byte) {
    if (string == NULL) {
        return;
    }
    size_t required_capacity = string->len + 2;
    string_grow(string, required_capacity);
    string->ptr[string->len] = byte;
    string->len += 1;
}

void string_append_string(String *string, const char *append_string) {
    if (string == NULL) {
        return;
    }
    size_t append_string_length = strlen(append_string);
    size_t required_capacity = string->len + append_string_length + 1;
    string_grow(string, required_capacity);
    memcpy(string->ptr + string->len, append_string, append_string_length);
    string->len += append_string_length;
}

void string_insert_byte(String *string, size_t index, char byte) {
    if (string == NULL) {
        return;
    }
    if (index > string->len - 1) {
        return;
    }
    size_t required_capacity = string->len + 2;
    string_grow(string, required_capacity);
    for (size_t i = string->len; i > index; i -= 1) {
        string->ptr[i] = string->ptr[i - 1];
    }
    string->ptr[index] = byte;
    string->len += 1;
}

void string_insert_string(String *string, size_t index, const char *insert_string) {
    if (string == NULL) {
        return;
    }
    if (index > string->len - 1) {
        return;
    }
    size_t insert_string_length = strlen(insert_string);
    size_t required_capacity = string->len + insert_string_length + 1;
    string_grow(string, required_capacity);
    for (size_t i = string->len - 1 + insert_string_length; i > index; i -= 1) {
        string->ptr[i] = string->ptr[i - insert_string_length];
    }
    for (size_t i = 0; i < insert_string_length; i += 1) {
        string->ptr[index + i] = insert_string[i];
    }
    string->len += insert_string_length;
}

void string_remove_byte(String *string, size_t index) {
    if (string == NULL) {
        return;
    }
    if (string->len == 0) {
        return;
    }
    if (index > string->len - 1) {
        return;
    }
    for (size_t i = index; i < string->len; i += 1) {
        string->ptr[i] = string->ptr[i + 1];
    }
    string->len -= 1;
}

void string_remove_string(String *string, size_t index, size_t length) {
    if (string == NULL) {
        return;
    }
    if (string->len == 0) {
        return;
    }
    if (index > string->len - 1) {
        return;
    }
    if (length == 0) {
        return;
    }
    for (size_t i = index; i < string->len; i += 1) {
        if (i + length > string->cap) {
            break;
        }
        string->ptr[i] = string->ptr[i + length];
    }
    if (length >= string->len) {
        string->len = index;
    }
    else {
        string->len -= length;
    }
    memset(&string->ptr[string->len], 0, string->cap - string->len);
}

void string_copy(String *dst, String *src) {
    if (dst == NULL || src == NULL) {
        return;
    }
    string_grow(dst, src->len + 1);
    memset(dst->ptr, 0, dst->len);
    strcpy(dst->ptr, src->ptr);
    dst->len = src->len;
}

#endif
