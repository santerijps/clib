#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include "curl/include/curl/curl.h"
#include "Buffer.h"

static CURLcode http_internal_curl_code;
static CURLHcode http_internal_curlh_code;

CURLcode http_get_last_error(void) {
    return http_internal_curl_code;
}

/**
 * HTTP status codes enum.
 * @todo Add the missing codes
 */
typedef enum HttpStatusCode {
    HttpOk = 200,
    HttpBadRequest = 400,
    HttpUnauthorized = 401,
    HttpPaymentRequired = 402,
    HttpForbidden = 403,
    HttpNotFound = 404,
} HttpStatusCode;

/**
 * A lookup table for HTTP response status messages.
 * The indeces are equivalent to the status codes.
 */
const char *http_internal_status_message_lookup_table[] = {
    [HttpOk] = "OK",
    [HttpBadRequest] = "Bad Request",
    [HttpUnauthorized] = "Unauthorized",
    [HttpPaymentRequired] = "Payment Required",
    [HttpForbidden] = "Forbidden",
    [HttpNotFound] = "Not Found",
};

/**
 * Get the status message for a given status code.
 * @param code An HTTP status code (e.g. 200)
 * @return Status message (e.g. 200 => "OK")
 */
const char *http_get_status_message(HttpStatusCode code) {
    return http_internal_status_message_lookup_table[code];
}

typedef struct HttpResponse {
    CURL *curl_handle;
    long code, version;
} HttpResponse;

/**
 * Free all the associated memory of the HTTP response.
 * Call this for every response.
 * @param r HttpResponse
 */
void http_response_release(HttpResponse *r) {
    curl_easy_cleanup(r->curl_handle);
}

/**
 * Get a header value from an HTTP response.
 * @param r HttpResponse
 * @param header_name Name of the header
 * @return If found, the header value, else `NULL`
 */
char *http_response_get_header(HttpResponse *r, const char *header_name) {
    struct curl_header *header;
    http_internal_curlh_code = curl_easy_header(r->curl_handle, header_name, 0, CURLH_HEADER, 0, &header);
    if (http_internal_curlh_code != 0) {
        return NULL;
    }
    return header->value;
}

/**
 * Get a header value from an HTTP response with a default value.
 * @param r HttpResponse
 * @param header_name Name of the header
 * @param default_value The default value to use if header is not found
 * @return If found, the header value, else the default value
 */
char *http_response_get_header_default(HttpResponse *r, const char *header_name, const char *default_value) {
    struct curl_header *header;
    http_internal_curlh_code = curl_easy_header(r->curl_handle, header_name, 0, CURLH_HEADER, 0, &header);
    if (http_internal_curlh_code != 0 || header->value == NULL) {
        return (char*)default_value;
    }
    return header->value;
}

/**
 * The write function for curl.
 * https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
 */
typedef size_t (*BodyWriteFunction)(char *chunk, size_t size, size_t count, void *user_data);


/**
 * Perform an HTTP GET request.
 * @param url The request URL
 * @param body_write_function Callback function (curl write function) for the response body
 * @param user_data Any user specified data to be passed into the callback function
 * @return An `HttpResponse` struct.
 */
HttpResponse http_request_get(const char *url, BodyWriteFunction body_write_function, void *user_data) {
    HttpResponse response = {0};
    CURL *curl = curl_easy_init();

    if (curl == NULL) {
        goto FunctionReturn;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);                           // Set the request URL
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/117.0.0.0 Safari/537.36");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);                 // Follow HTTP 3xx redirects
    curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");               // SSL certificate
    curl_easy_setopt(curl, CURLOPT_CAPATH, "cacert.pem");               // SSL certificate
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, body_write_function); // Body write function
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, user_data);               // Body write function user data
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, NULL);               // Header write function
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, NULL);                   // Header write function user data

    http_internal_curl_code = curl_easy_perform(curl);
    if(http_internal_curl_code != CURLE_OK) {
        goto FunctionReturn;
    }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.code);
    curl_easy_getinfo(curl, CURLINFO_HTTP_VERSION, &response.version);

FunctionReturn:
    response.curl_handle = curl;
    return response;
}

#endif
