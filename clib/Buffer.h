#ifndef BUFFER_H
#define BUFFER_H

#ifndef _INC_STDARG
#include <stdarg.h>
#endif

#ifndef _INC_STDLIB
#include <stdlib.h>
#endif

#ifndef _INC_STRING
#include <string.h>
#endif

#pragma region Internals

/**
 * Writes the provided int64 `value` as a string to `buffer`.
 * @param buffer Pointer to the output buffer.
 * @param buffer_size Size of the buffer.
 * @param value The integer value to convert to a string.
 * @param base Base of the number.
 * @return 1 if error, else 0
 */
int buffer_internal_i64_to_string(char *buffer, size_t buffer_size, int64_t value, int32_t base) {
    if (base < 2 || base > 36) {
        *buffer = '\0';
        return 1;
    }
    char *ptr1 = buffer;
    while (1) {
        if (--buffer_size == 0) {
            return 1;
        }
        int tmp = value;
        value /= base;
        *ptr1++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp - value * base)];
        if (value == 0) {
            if (tmp < 0) {
                *ptr1++ = '-';
            }
            *ptr1-- = '\0';
            break;
        }
    }
    while(buffer < ptr1) {
        char tmp = *ptr1;
        *ptr1-- = *buffer;
        *buffer++ = tmp;
    }
    return 0;
}

/**
 * Writes the provided uint64 `value` as a string to `buffer`.
 * @param buffer Pointer to the output buffer.
 * @param buffer_size Size of the buffer.
 * @param value The integer value to convert to a string.
 * @param base Base of the number.
 * @return 1 if error, else 0
 */
int buffer_internal_u64_to_string(char *buffer, size_t buffer_size, uint64_t value, int32_t base) {
    if (base < 2 || base > 36) {
        *buffer = '\0';
        return 0;
    }
    char *ptr1 = buffer;
    while (1) {
        if (--buffer_size == 0) {
            return 0;
        }
        uint64_t tmp = value;
        value /= base;
        *ptr1++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp - value * base)];
        if (value == 0) {
            *ptr1-- = '\0';
            break;
        }
    }
    while(buffer < ptr1) {
        char tmp = *ptr1;
        *ptr1-- = *buffer;
        *buffer++ = tmp;
    }
    return 1;
}

#pragma endregion

/**
 * Macro for accessing `Buffer.ptr`.
 * @param BufferAsValue Buffer as a value, not a pointer!
 * @return String content of the buffer.
 */
#define buffer_to_string(BufferAsValue) ((char*)((BufferAsValue).ptr))

typedef struct Buffer {
    char *ptr;
    size_t cap, len;
} Buffer;

/**
 * The primary return type for all buffer functions.
 * The happy case is equal to 0 (`BUFFER_ERROR_NONE`);
 */
typedef enum BufferError {
    BUFFER_ERROR_NONE,
    BUFFER_ERROR_NULL_POINTER,
    BUFFER_ERROR_ALLOCATION_FAILURE,
    BUFFER_ERROR_EMPTY_BUFFER,
    BUFFER_ERROR_INVALID_FORMAT,
    BUFFER_ERROR_INDEX_OUT_OF_BOUNDS,
} BufferError;

char *buffer_error_to_string(BufferError error) {
    switch (error) {
        case BUFFER_ERROR_NONE:
            return "BUFFER_ERROR_NONE";
        case BUFFER_ERROR_NULL_POINTER:
            return "BUFFER_ERROR_NULL_POINTER";
        case BUFFER_ERROR_ALLOCATION_FAILURE:
            return "BUFFER_ERROR_ALLOCATION_FAILURE";
        case BUFFER_ERROR_EMPTY_BUFFER:
            return "BUFFER_ERROR_EMPTY_BUFFER";
        case BUFFER_ERROR_INVALID_FORMAT:
            return "BUFFER_ERROR_INVALID_FORMAT";
        case BUFFER_ERROR_INDEX_OUT_OF_BOUNDS:
            return "BUFFER_ERROR_INDEX_OUT_OF_BOUNDS";
        default:
            return NULL;
    }
}

/**
 * Initializes and returns a new buffer.
 * NOTE: Allocates memory! Remember to null check `Buffer.ptr`!
 * @param cap Initial capacity. If 0, will default to 2.
 * @return `Buffer` struct value.
 */
Buffer buffer_make(size_t cap) {
    if (cap == 0) {
        cap = 2;
    }
    return (Buffer) {
        .ptr = calloc(cap, 1),
        .cap = cap,
        .len = 0,
    };
}

/**
 * Initialize a new buffer from a string.
 * NOTE: Allocates memory! Remember to null check `Buffer.ptr`!
 * @param string The string to initialize the buffer with.
 * @return `Buffer` struct value.
 */
Buffer buffer_from_string(const char *string) {
    Buffer buffer;
    size_t string_len = strlen(string);
    buffer.cap = string_len + 1;
    buffer.len = string_len;
    buffer.ptr = calloc(buffer.cap, 1);
    if (buffer.ptr != NULL) {
        memcpy(buffer.ptr, string, string_len);
    }
    return buffer;
}

/**
 * Initializes the buffer.
 * NOTE: Allocates memory!
 * @param buf Pointer to a `Buffer` struct.
 * @param cap Initial capacity. If 0, will default to 1.
 * @return `BufferError` (errors are non-zero).
 */
BufferError buffer_init(Buffer *buf, size_t cap) {
    if (buf == NULL) {
        return BUFFER_ERROR_NULL_POINTER;
    }
    *buf = buffer_make(cap);
    if (buf->ptr == NULL) {
        return BUFFER_ERROR_ALLOCATION_FAILURE;
    }
    return BUFFER_ERROR_NONE;
}

/**
 * Releases the buffer of everything.
 * Performs a free on `Buffer.ptr` and sets `cap` and `len` to 0.
 * @param buf Pointer to a `Buffer` struct.
 * @return `BufferError` (errors are non-zero).
 */
BufferError buffer_release(Buffer *buf) {
    if (buf == NULL) {
        return BUFFER_ERROR_NULL_POINTER;
    }
    free(buf->ptr);
    buf->ptr = NULL;
    buf->cap = 0;
    buf->len = 0;
    return BUFFER_ERROR_NONE;
}

/**
 * Clears the buffer, but does not free any memory.
 * Sets `len` to 0 and zeroes out the memory.
 * @param buf Pointer to a `Buffer` struct.
 * @return `BufferError` (errors are non-zero).
 */
BufferError buffer_clear(Buffer *buf) {
    if (buf == NULL) {
        return BUFFER_ERROR_NULL_POINTER;
    }
    memset(buf->ptr, 0, buf->len);
    buf->len = 0;
    return BUFFER_ERROR_NONE;
}

/**
 * Grows the buffer capacity if needed.
 * Extra allocated memory is zeroed.
 * NOTE: Allocates memory!
 * @param buf Pointer to a `Buffer` struct.
 * @param required_cap The target capacity of the buffer.
 * @return `BufferError` (errors are non-zero).
 */
BufferError buffer_grow(Buffer *buf, size_t required_cap) {
    if (buf == NULL) {
        return BUFFER_ERROR_NULL_POINTER;
    }
    if (required_cap >= buf->cap) {
        while (required_cap >= buf->cap) {
            buf->cap <<= 1;
        }
        buf->ptr = realloc(buf->ptr, buf->cap);
        if (buf->ptr == NULL) {
            return BUFFER_ERROR_ALLOCATION_FAILURE;
        }
        memset(buf->ptr + buf->len, 0, buf->cap - buf->len);
    }
    return BUFFER_ERROR_NONE;
}

/**
 * Writes a byte to the end of the buffer.
 * NOTE: Allocates memory!
 * @param buf Pointer to a `Buffer` struct.
 * @param b Byte to be written.
 * @return `BufferError` (errors are non-zero).
 */
BufferError buffer_write_byte(Buffer *buf, char b) {
    if (buf == NULL) {
        return BUFFER_ERROR_NULL_POINTER;
    }
    size_t required_cap = buf->len + 2;
    BufferError error = buffer_grow(buf, required_cap);
    if (error != BUFFER_ERROR_NONE) {
        return error;
    }
    buf->ptr[buf->len] = b;
    buf->len += 1;
    return BUFFER_ERROR_NONE;
}

/**
 * Write a byte in the specified index.
 * This can overwrite existing data.
 * Returns an error if index is larger than the capacity.
 * @param buf Pointer to a `Buffer` struct.
 * @param b Byte to be written.
 * @param index The index to write to.
 * @return `BufferError` (errors are non-zero).
 */
BufferError buffer_write_byte_at(Buffer *buf, char b, size_t index) {
    if (buf == NULL) {
        return BUFFER_ERROR_NULL_POINTER;
    }
    size_t required_cap = buf->len + 2;
    BufferError error = buffer_grow(buf, required_cap);
    if (error != BUFFER_ERROR_NONE) {
        return error;
    }
    if (index > buf->cap - 1) {
        return BUFFER_ERROR_INDEX_OUT_OF_BOUNDS;
    }
    buf->ptr[index] = b;
    if (index == buf->len) {
        buf->len += 1;
    }
    return BUFFER_ERROR_NONE;
}

/**
 * Writes some bytes to the end of the buffer.
 * NOTE: Allocates memory! Prefer this function over `buffer_write_string` if the data is not a C-string (e.g. a file).
 * @param buf Pointer to a `Buffer` struct.
 * @param bytes The bytes to be written.
 * @param n The number of bytes to be written.
 * @return `BufferError` (errors are non-zero).
 */
BufferError buffer_write_bytes(Buffer *buf, const char *bytes, size_t n) {
    if (buf == NULL) {
        return BUFFER_ERROR_NULL_POINTER;
    }
    size_t required_cap = buf->len + n + 1;
    BufferError error = buffer_grow(buf, required_cap);
    if (error != BUFFER_ERROR_NONE) {
        return error;
    }
    memcpy(buf->ptr + buf->len, bytes, n);
    buf->len += n;
    return BUFFER_ERROR_NONE;
}

/**
 * Writes a string to the end of the buffer.
 * NOTE: Allocates memory!
 * @param buf Pointer to a `Buffer` struct.
 * @param s String to be written.
 * @return `BufferError` (errors are non-zero).
 */
BufferError buffer_write_string(Buffer *buf, const char *s) {
    if (buf == NULL) {
        return BUFFER_ERROR_NULL_POINTER;
    }
    size_t s_len = strlen(s);
    size_t required_cap = buf->len + s_len + 1;
    BufferError error = buffer_grow(buf, required_cap);
    if (error != BUFFER_ERROR_NONE) {
        return error;
    }
    memcpy(buf->ptr + buf->len, s, s_len);
    buf->len += s_len;
    return BUFFER_ERROR_NONE;
}

/**
 * Writes a formatted string to the end of the buffer.
 * NOTE: Allocates memory!
 * @param buf Pointer to a `Buffer` struct.
 * @param format Format of the string output.
 * @param ... Format arguments.
 * @return `BufferError` (errors are non-zero).
 */
BufferError buffer_write_format(Buffer *buf, const char *format, ...) {
    if (buf == NULL) {
        return BUFFER_ERROR_NULL_POINTER;
    }
    va_list valist;
    va_start(valist, format);
    BufferError error = BUFFER_ERROR_NONE;
    while (*format) {
        if (*format != '%') {
            buffer_write_byte(buf, *format);
            format += 1;
            continue;
        }
        format += 1;
        switch (*format) {
            case 'b': {
                char *arg = va_arg(valist, int) ? "true" : "false";
                if ((error = buffer_write_string(buf, arg))) {
                    return error;
                }
                break;
            }
            case 'c': {
                char arg = va_arg(valist, int);
                if ((error = buffer_write_byte(buf, arg))) {
                    return error;
                }
                break;
            }
            case 'd':
            case 'i': {
                char arg[20] = {0};
                buffer_internal_i64_to_string(arg, 20, va_arg(valist, int64_t), 10);
                if ((error = buffer_write_string(buf, arg))) {
                    return error;
                }
                break;
            }
            case 'u': {
                char arg[21] = {0};
                buffer_internal_u64_to_string(arg, 21, va_arg(valist, uint64_t), 10);
                if ((error = buffer_write_string(buf, arg))) {
                    return error;
                }
                break;
            }
            case 's': {
                char *arg = va_arg(valist, char*);
                if ((error = buffer_write_string(buf, arg))) {
                    return error;
                }
                break;
            }
            default:
                return BUFFER_ERROR_INVALID_FORMAT;
        }
        format += 1;
    }
    return error;
}

/**
 * Copies the string content of the buffer.
 * NOTE: Allocates memory for the copy!
 * @param buf Pointer to a `Buffer` struct.
 * @param s String to be written.
 * @return `BufferError` (errors are non-zero).
 */
BufferError buffer_copy_string(Buffer *buf, char **dst) {
    if (buf == NULL) {
        return BUFFER_ERROR_NULL_POINTER;
    }
    if (buf->len == 0) {
        return BUFFER_ERROR_EMPTY_BUFFER;
    }
    *dst = calloc(buf->len + 1, 1);
    if (*dst == NULL) {
        return BUFFER_ERROR_ALLOCATION_FAILURE;
    }
    memcpy(*dst, buf->ptr, buf->len);
    return BUFFER_ERROR_NONE;
}

/**
 * Pop a byte from the end of a buffer.
 * @param buf Buffer
 * @return Popped byte, 0 if length is 0
 */
char buffer_pop_byte(Buffer *buf) {
    if (buf->len == 0) {
        return 0;
    }
    char *string = (char*)buf->ptr;
    char byte = string[buf->len - 1];
    string[buf->len - 1] = 0;
    buf->len -= 1;
    return byte;
}

#endif
