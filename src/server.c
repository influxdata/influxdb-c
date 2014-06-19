/*
 * Copyright (c) 2013-2014 InfluxDB
 * Pierre-Olivier Mercier <pomercier@novaquark.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#include <json-c/json.h>
#include <stdio.h>
#include <string.h>

#include "server.h"
#include "utils.h"

void
*influxdb_server_extractor(json_object *obj)
{
    s_influxdb_server *server = malloc(sizeof (s_influxdb_server));

    server->id = json_object_get_int(
        json_object_object_get(obj, "id"));
    server->protobufConnectString = influxdb_strdup(json_object_get_string(
        json_object_object_get(obj, "protobufConnectString")));

    return server;
}

size_t
influxdb_servers(s_influxdb_client *client, s_influxdb_server ***servers_list)
{
    return influxdb_client_list_something(client, "/cluster/servers",
                                          (void ***) servers_list,
                                          &influxdb_server_extractor);
}

int
influxdb_remove_servers(s_influxdb_client *client, unsigned int id)
{
    char path[INFLUXDB_URL_MAX_SIZE];

    sprintf(path, "/cluster/servers/%u", id);

    return influxdb_client_delete(client, path, NULL);
}

void
influxdb_server_free(s_influxdb_server *server)
{
    if (server)
    {
        free(server->protobufConnectString);
        free(server);
    }
}
