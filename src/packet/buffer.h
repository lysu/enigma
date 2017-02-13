#ifndef ENIGMA_BUFFER_H
#define ENIGMA_BUFFER_H

#define IO_SUCCESS 0
#define IO_ERROR -1
#define IO_WAIT 1
#define IO_DOWN 2

typedef unsigned char byte;

struct byte_buffer_s {
    byte *data;
    size_t size;
    int offset;
    size_t capacity;
};

typedef struct byte_buffer_s byte_buffer_t;

static inline byte_buffer_t *byte_buffer_new(size_t capacity) {
    if (capacity <= 0) {
        return NULL;
    }
    byte_buffer_t *buffer = calloc(1, sizeof(byte_buffer_t));
    if (buffer == NULL) {
        return NULL;
    }
    buffer->data = malloc(capacity);
    if (buffer->data = NULL) {
        free(buffer);
        return NULL;
    }
    buffer->capacity = capacity;
    return buffer;
}

static inline void byte_buffer_from(byte_buffer_t* buffer, byte *data, size_t capacity) {
    if (capacity <= 0) {
        return NULL;
    }
    if (buffer == NULL) {
        return NULL;
    }
    if (data == NULL) {
        return NULL;
    }
    buffer->data = data;
    buffer->capacity = capacity;
}

static inline int byte_buffer_extend(byte_buffer_t *buffer, size_t len, int is_pow) {
    size_t want_alloc = buffer->capacity + len;
    if (is_pow == 1) {
        size_t acc_alloc = 1;
        while (acc_alloc < want_alloc) {
            acc_alloc <<= 1;
        }
        want_alloc = acc_alloc;
    }
    byte *p = realloc(buffer->data, want_alloc);
    if (p == NULL) {
        return IO_ERROR;
    }
    buffer->capacity = want_alloc;
    buffer->data = p;
    return 1;
}

static inline int byte_array_append(byte_buffer_t *buffer, const byte *data, size_t data_size) {
    if (buffer == NULL) {
        return IO_ERROR;
    }
    if (data == NULL) {
        return IO_ERROR;
    }
    if (data_size <= 0) {
        return IO_SUCCESS;
    }
    if ((buffer->capacity - buffer->size) < data_size) {
        size_t want_alloc = buffer->size + data_size;
        size_t acc_alloc = 1;
        while (acc_alloc < want_alloc) {
            acc_alloc <<= 1;
        }
        void *p = realloc(buffer->data, acc_alloc);
        if (p == NULL) {
            return IO_ERROR;
        }
        buffer->data = p;
        buffer->capacity = acc_alloc;
    }
    memcpy(buffer->data + buffer->size, data, data_size);
    buffer->size += data_size;
    return IO_SUCCESS;
}

static inline int byte_buffer_reset(byte_buffer_t *buffer) {
    if (buffer == NULL) {
        return IO_ERROR;
    }
    buffer->size = 0;
    buffer->offset = 0;
    return IO_SUCCESS;
}

static inline void byte_buffer_destroy(byte_buffer_t *buffer) {
    if (buffer == NULL) {
        return;
    }
    if (buffer->data != NULL) {
        free(buffer->data);
        buffer->data = NULL;
    }
    free(buffer);
}

#endif //ENIGMA_BUFFER_H
