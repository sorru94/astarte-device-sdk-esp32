#include "zlib_tests.h"

#include <zlib.h>
#include <string.h>
#include <stdio.h>

#include <esp_log.h>

#define TAG "ZLIB_TESTS"


/**
 * @brief Function equivalent to the `compress` function defined in zlib.h.
 *
 * @details The implementation is copied 1 to 1 from zlib, with a single difference.
 * `defalteInit` has been replaced by `deflateInit2`. This allows to reduce the memory usage of zlib
 * by setting custom values for the `windowBits` and `memLevel` parameters.
 * This could also have been done by changing the values of the defines `MAX_WBITS` and
 * `DEF_MEM_LEVEL` in zconf.h. However this would have required to change the source of a idf
 * component and has been avoided.
 *
 * @param dest See docstrings for `compress` in zlib.h
 * @param destLen See docstrings for `compress` in zlib.h
 * @param source See docstrings for `compress` in zlib.h
 * @param sourceLen See docstrings for `compress` in zlib.h
 * @return See docstrings for `compress` in zlib.h
 */
static int ZEXPORT zlib_compress(Bytef *dest, uLongf *destLen, const Bytef *source,
                      uLong sourceLen) {
    z_stream stream;
    int err;
    const uInt max = (uInt)-1;
    uLong left;

    left = *destLen;
    *destLen = 0;

    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;
    stream.opaque = (voidpf)0;

    int windowBits = 9; // Smallest possible window
    int memLevel = 1; // Smallest possible memory usage
    err = deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, windowBits, memLevel, Z_DEFAULT_STRATEGY);
    if (err != Z_OK) return err;

    stream.next_out = dest;
    stream.avail_out = 0;
    stream.next_in = (z_const Bytef *)source;
    stream.avail_in = 0;

    do {
        if (stream.avail_out == 0) {
            stream.avail_out = left > (uLong)max ? max : (uInt)left;
            left -= stream.avail_out;
        }
        if (stream.avail_in == 0) {
            stream.avail_in = sourceLen > (uLong)max ? max : (uInt)sourceLen;
            sourceLen -= stream.avail_in;
        }
        err = deflate(&stream, sourceLen ? Z_NO_FLUSH : Z_FINISH);
    } while (err == Z_OK);

    *destLen = stream.total_out;
    deflateEnd(&stream);
    return err == Z_STREAM_END ? Z_OK : err;
}

// Dynamically allocates memory and does not free it
static char* array_to_string(uint8_t *input, size_t input_len)
{
    size_t output_len = input_len * 5 + 1;
    char *output = calloc(output_len, sizeof(char));
    for (size_t i = 0; i < input_len; i++)
    {
        sprintf(output + (i * 5), "0x%02x ", input[i]);
    }
    return output;
}

void zlib_compression_test(void)
{
    char input[] = "org.astarteplatform.esp32.examples.DeviceProperties1/112/double_endpoint;org.astarteplatform.esp32.examples.DeviceProperties1/5/integer_endpoint;";
    size_t input_len = strlen(input);

    uLongf output_len = compressBound(input_len);
    uint8_t *output = malloc(output_len * sizeof(uint8_t));
    if (output == NULL) {
        ESP_LOGE(TAG, "Out of memory %s: %d", __FILE__, __LINE__);
        return;
    }
    int compress_res = zlib_compress(output, &output_len, (char unsigned *) input, input_len);
    if (compress_res != Z_OK) {
        ESP_LOGE(TAG, "Compression error %d.", compress_res);
        return;
    }

    ESP_LOGW(TAG, "Input size %zu", input_len);
    ESP_LOGW(TAG, "Compress bound size %lu", output_len);
    ESP_LOGW(TAG, "Compression result %s", array_to_string(output, output_len));

}

void zlib_decompression_test(void)
{
    uint8_t input[78] = {
        0x00, 0x00, 0x00, 0x4d, 0x78, 0x9c, 0x05, 0xc1, 0x81, 0x09, 0x80, 0x30, 0x0c, 0x04, 0xc0,
        0x8d, 0x5a, 0xaa, 0x8b, 0x08, 0x0e, 0x20, 0x11, 0x5f, 0x29, 0xb4, 0x49, 0xf8, 0x04, 0x71,
        0x7c, 0xef, 0x8c, 0x4f, 0x91, 0x48, 0x61, 0xc2, 0x87, 0xe4, 0x6d, 0x9c, 0x05, 0xe1, 0xeb,
        0x52, 0xf0, 0xc9, 0xf4, 0x81, 0x28, 0x3b, 0xf8, 0x82, 0x1b, 0xcd, 0xc1, 0xec, 0x88, 0x56,
        0x03, 0x1a, 0xc6, 0x56, 0x4f, 0xb3, 0x01, 0xd1, 0x03, 0x7a, 0xb9, 0x75, 0xcd, 0x1f, 0x8e,
        0x38, 0x1e, 0x04};
    size_t input_len = 78;

    uLongf output_len = (size_t) input[0] << 24 | (size_t) input[1] << 16 | (size_t) input[2] << 8 | (size_t) input[3];

    ESP_LOGW(TAG, "%zu", sizeof(size_t));
    ESP_LOGW(TAG, "Input size %zu", input_len);
    ESP_LOGW(TAG, "Output size %lu", output_len);

    char *output = calloc(output_len + 1, sizeof(char));
    if (output == NULL) {
        ESP_LOGE(TAG, "Out of memory %s: %d", __FILE__, __LINE__);
        return;
    }

    int decompress_res = uncompress((char unsigned *) output, &output_len, (char unsigned *) input + 4, input_len);
    if (decompress_res != Z_OK) {
        ESP_LOGE(TAG, "Decompression error %d.", decompress_res);
        return;
    }

    ESP_LOGW(TAG, "Decompression result %s", output);
}
