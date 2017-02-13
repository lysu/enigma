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

int packet_conn_read_packet(packet_conn_t *c, byte_buffer_t *buffer) {
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
