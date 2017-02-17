#ifndef ENIGMA_AUTH_H
#define ENIGMA_AUTH_H

#include "conn.h"
#include "../packet/buffer.h"

#define ServerVersion "mysql-5.5"

int srv_auth_write_initial_handshake(srv_conn_t *c);

int srv_auth_read_handshake_response(srv_conn_t *c, char *password);

#endif //ENIGMA_AUTH_H
