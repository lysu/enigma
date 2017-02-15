#include <unistd.h>
#include <errno.h>
#include "conn.h"

#define HEADER_SIZE 4
#define MAX_PAY_LOAD_LEN (1<<24 - 1)

packet_conn_t *packet_conn_new(int fd) {
    packet_conn_t *pc = malloc(sizeof(packet_conn_t));
    pc->fd = fd;
    pc->seq = 0;
    return pc;
}

static inline int packet_conn_read_(packet_conn_t *c, byte_buffer_t *buffer, size_t want_len) {
    if (c == NULL || buffer == NULL) {
        return IO_ERROR;
    }
    if (want_len <= 0) {
        return IO_SUCCESS;
    }
    if ((buffer->capacity - buffer->size) < want_len) {
        int ret = byte_buffer_extend(buffer, want_len - buffer->capacity + buffer->size, 0);
        if (ret != IO_SUCCESS) {
            return ret;
        }
    }
    ssize_t len = read(c->fd, buffer->data + buffer->size, want_len);
    if (len == 0) {
        return IO_ERROR;
    }
    if (len < want_len) {
        return IO_ERROR;
    }
    ssize_t ret_len = (size_t) len >= 0 ? len : 0;
    buffer->size += ret_len;
    return IO_SUCCESS;
}

int packet_conn_read(packet_conn_t *c, byte_buffer_t *buffer) {
    do {
        byte header_data[HEADER_SIZE];
        byte_buffer_t header_buffer;
        byte_buffer_from(&header_buffer, header_data, HEADER_SIZE);

        size_t packet_len = header_data[0] | header_data[1] << 8 | header_data[2] << 16;
        uint8_t packet_seq = header_data[3];

        if (packet_seq != c->seq) {
            return IO_ERROR;
        }

        c->seq++;

        int ret = packet_conn_read_(c, buffer, packet_len);
        if (ret != IO_SUCCESS) {
            return ret;
        }

        if (packet_len < MAX_PAY_LOAD_LEN) {
            break;
        }
    } while (1);
    return IO_SUCCESS;
}

static inline int packet_conn_write_(packet_conn_t *c, byte_buffer_t *buffer, size_t want_len) {
    if (c == NULL || buffer == NULL) {
        return IO_ERROR;
    }
    if (want_len <= 0) {
        return IO_SUCCESS;
    }
    ssize_t write_len = write(c->fd, buffer->data, want_len);
    if (write_len == 0) {
        return IO_ERROR;
    }
    if (write_len < want_len) {
        return IO_ERROR;
    }
    return IO_SUCCESS;
}

int packet_conn_write(packet_conn_t *c, byte_buffer_t *buffer) {
    size_t len = buffer->size - 4;
    while (len >= MAX_PAY_LOAD_LEN) {
        buffer->data[0] = 0xff;
        buffer->data[1] = 0xff;
        buffer->data[2] = 0xff;
        buffer->data[3] = c->seq;

        int ret = packet_conn_write_(c, buffer, MAX_PAY_LOAD_LEN + 4);
        if (ret != IO_SUCCESS) {
            return IO_ERROR;
        }

        c->seq++;
        len -= MAX_PAY_LOAD_LEN;
        buffer->size += MAX_PAY_LOAD_LEN;
    }

    buffer->data[0] = (byte)len;
    buffer->data[1] = (byte)(len >> 8);
    buffer->data[2] = (byte)(len >> 16);
    buffer->data[3] = c->seq;

    int ret = packet_conn_write_(c, buffer, len + 4);
    if (ret != IO_SUCCESS) {
        return IO_ERROR;
    }
    c->seq++;
    return IO_SUCCESS;
}

int packet_conn_reset(packet_conn_t *c) {
    c->seq = 0;
    return IO_SUCCESS;
}


int packet_conn_close(packet_conn_t *c) {
    c->seq = 0;
    close(c->fd);
    return IO_SUCCESS;
}
