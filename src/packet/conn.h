#ifndef ENIGMA_PACKET_CONN_H
#define ENIGMA_PACKET_CONN_H

#include <stdint.h>
#include <stdlib.h>

#include "buffer.h"

struct packet_conn_s {
    int fd;
    uint8_t seq;
};

typedef struct packet_conn_s packet_conn_t;

packet_conn_t *packet_conn_new(int fd);

int packet_conn_read(packet_conn_t *c, byte_buffer_t *buffer);

int packet_conn_write(packet_conn_t *c, byte_buffer_t *buffer);

int packet_conn_reset(packet_conn_t *c);

int packet_conn_close(packet_conn_t *c);

#endif //ENIGMA_CONN_H
