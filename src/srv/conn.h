#ifndef ENIGMA_SRV_CONN_H
#define ENIGMA_SRV_CONN_H

#include <stdint.h>

struct srv_conn_s {
    uint32_t capability;
    uint32_t connection_id;
    uint16_t status;
    char *user;
    unsigned char *salt;
};

typedef struct srv_conn_s srv_conn_t;

#endif //ENIGMA_CONN_H
