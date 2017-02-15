#ifndef ENIGMA_SRV_CONN_H
#define ENIGMA_SRV_CONN_H

#include <stdint.h>
#include "../packet/conn.h"

struct srv_conn_s {
    packet_conn_t *pc;
    uint32_t capability;
    uint32_t connection_id;
    uint16_t status;
    char *user;
    unsigned char *salt;
};

typedef struct srv_conn_s srv_conn_t;

static inline srv_conn_t *srv_conn_new(int fd, char *user, char *password) {
    srv_conn_t *sc = calloc(1, sizeof(srv_conn_t));
    sc->user = user;
    sc->pc = packet_conn_new(fd);


    return sc;
}

#endif //ENIGMA_CONN_H
