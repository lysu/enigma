#include "auth.h"

int srv_auth_write_initial_handshake(srv_conn_t *c) {

    byte_buffer_t *buf = byte_buffer_new(128);

    // skip length.
    buf->size += 4;

    //min version 10
    byte_buffer_append(buf, (byte[1]) {10}, 1);

    //server version[00]
    char *server_version = ServerVersion;
    byte_buffer_append(buf, server_version, strlen(server_version));
    byte_buffer_append(buf, (byte[1]) {0}, 1);

    // connection id
    byte_buffer_append(buf, (byte[4]) {
            (byte) c->connection_id,
            (byte) (c->connection_id >> 8),
            (byte) (c->connection_id >> 16),
            (byte) (c->connection_id >> 24)
    }, 4);

    // auth-plugin-data-part-1
    byte_buffer_append(buf, c->salt, 8);

    //filter [00]
    byte_buffer_append(buf, (byte[1]) {0}, 1);

    //capability flag lower 2 bytes, using default capability here
    int capability = 0;
    byte_buffer_append(buf, (byte[2]) {(byte) capability, (byte) (capability >> 8)}, 2);

    //charset, utf-8 default
    byte_buffer_append(buf, (byte[1]) {33}, 1);

    //status
    byte_buffer_append(buf, (byte[2]) {(byte) c->status, (byte) (c->status >> 8)}, 2);

    //below 13 byte may not be used
    //capability flag upper 2 bytes, using default capability here
    byte_buffer_append(buf, (byte[2]) {(byte) (capability >> 16), (byte) (capability >> 24)}, 2);

    //filter [0x15]
    byte_buffer_append(buf, (byte[1]) {0x15}, 1);

    //reserved 10 [00]
    byte_buffer_append(buf, (byte[10]) {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 10);

    //auth-plugin-data-part-2
    byte_buffer_append(buf, c->salt + 8, 8);

    //filter [00]
    byte_buffer_append(buf, (byte[1]) {0}, 1);

    return packet_conn_write(c->pc, buf);

}

