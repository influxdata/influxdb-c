/*
 * Copyright (c) 2013-2014 InfluxDB
 * Pierre-Olivier Mercier <pomercier@novaquark.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#ifndef SERVERS_H_
# define SERVERS_H_

# include "client.h"

typedef struct influxdb_server {
    unsigned int id;
    char         *protobufConnectString;
} s_influxdb_server;


/**
 * List servers in the cluster
 *
 * \param client A initialized client
 * \param servers_list The list to fill with server struct
 * \return The number of servers in the cluster
 */
size_t influxdb_servers(s_influxdb_client *client,
                        s_influxdb_server ***servers_list);

/**
 * Remove from cluster an existing server
 *
 * \param client A initialized client
 * \param id The server identifier to remove
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_remove_server(s_influxdb_client *client,
                           unsigned int      id);

/**
 * Clean memory used by a server
 */
void influxdb_server_free(s_influxdb_server *server);

#endif /* !SERVERS_H_ */
