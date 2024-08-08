#ifndef MIME_H
#define MIME_H

#ifndef _STDINT_H
#include <stdint.h>
#endif

#define MIME_INTERNAL_LENGTH (sizeof(MIMES) / sizeof(*MIMES))

/**
 * Mime struct that holds the mime type and file extension.
 */
typedef struct Mime {
    char *ext;
    char *type;
} Mime;

const Mime MIMES[] = {
    //Text MIME types
    {".css",   "text/css"},
    {".csv",   "text/csv"},
    {".htc",   "text/x-component"},
    {".html",  "text/html"},
    {".htm",   "text/html"},
    {".shtm",  "text/html"},
    {".shtml", "text/html"},
    {".stm",   "text/html"},
    {".txt",   "text/plain"},
    {".vcf",   "text/vcard"},
    {".vcard", "text/vcard"},
    {".xml",   "text/xml"},

    //Image MIME types
    {".gif",   "image/gif"},
    {".ico",   "image/x-icon"},
    {".jpg",   "image/jpeg"},
    {".jpeg",  "image/jpeg"},
    {".png",   "image/png"},
    {".svg",   "image/svg+xml"},
    {".tif",   "image/tiff"},

    //Audio MIME types
    {".aac",   "audio/x-aac"},
    {".aif",   "audio/x-aiff"},
    {".mp3",   "audio/mpeg"},
    {".wav",   "audio/x-wav"},
    {".wma",   "audio/x-ms-wma"},

    //Video MIME types
    {".avi",   "video/x-msvideo"},
    {".flv",   "video/x-flv"},
    {".mov",   "video/quicktime"},
    {".mp4",   "video/mp4"},
    {".mpg",   "video/mpeg"},
    {".mpeg",  "video/mpeg"},
    {".wmv",   "video/x-ms-wmv"},

    //Application MIME types
    {".doc",   "application/msword"},
    {".gz",    "application/x-gzip"},
    {".gzip",  "application/x-gzip"},
    {".js",    "application/javascript"},
    {".json",  "application/json"},
    {".ogg",   "application/ogg"},
    {".pdf",   "application/pdf"},
    {".ppt",   "application/vnd.ms-powerpoint"},
    {".rar",   "application/x-rar-compressed"},
    {".rtf",   "application/rtf"},
    {".tar",   "application/x-tar"},
    {".tgz",   "application/x-gzip"},
    {".xht",   "application/xhtml+xml"},
    {".xhtml", "application/xhtml+xml"},
    {".xls",   "application/vnd.ms-excel"},
    {".zip",   "application/zip"}
};

/**
 * Resolve the file extension for the given mime/content type.
 * Returns `NULL` if there's no match.
 * @param type Mime type
 * @return Resolved file extension or `NULL`
 */
char *mime_resolve_ext(const char *type) {
    for (size_t i = 0; i < MIME_INTERNAL_LENGTH; i += 1) {
        Mime mime = MIMES[i];
        for (size_t j = 0; mime.type[j] != '\0' && mime.type[j] != ';'; j += 1) {
            if (mime.type[j] != type[j]) {
                goto ContinueOuterLoop;
            }
        }
        return mime.ext;
ContinueOuterLoop:
    }
    return NULL;
}

/**
 * Resolve the file extension for the given mime/content type.
 * Returns `default_value` if there's no match.
 * @param type Mime type
 * @param default_value Default value
 * @return Resolved file extension or default value
 */
char *mime_resolve_ext_default(const char *type, const char *default_value) {
    for (size_t i = 0; i < MIME_INTERNAL_LENGTH; i += 1) {
        Mime mime = MIMES[i];
        for (size_t j = 0; mime.type[j] != '\0' && mime.type[j] != ';'; j += 1) {
            if (mime.type[j] != type[j]) {
                goto ContinueOuterLoop;
            }
        }
        return mime.ext;
ContinueOuterLoop:
    }
    return (char*)default_value;
}

/**
 * Resolve the mime/content type for the given file extension.
 * Returns `NULL` if there's no match.
 * @param type File extension as `.ext`
 * @return Mime type as `char*` or `NULL`
 */
char *mime_resolve_type(const char *ext) {
    for (size_t i = 0; i < MIME_INTERNAL_LENGTH; i += 1) {
        Mime mime = MIMES[i];
        for (size_t j = 0; mime.ext[j] != '\0'; j += 1) {
            if (mime.ext[j] != ext[j]) {
                goto ContinueOuterLoop;
            }
        }
        return mime.type;
ContinueOuterLoop:
    }
    return NULL;
}

#endif
