/*
 * Copyright (c) 2013-2014 InfluxDB
 * Pierre-Olivier Mercier <pomercier@novaquark.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#include <stdio.h>

#include "../src/server.h"
#include "tests.h"

int
ping(s_influxdb_client *client)
{
    char *status_txt;
    int status_code;

    status_code = influxdb_ping(client, (char **) &status_txt);

    if (strcmp(status_txt, "ok"))
        return 1;

    free(status_txt);

    return status_code != 200;
}

int
get(s_influxdb_client *client)
{
    s_influxdb_server **server_list;
    size_t nb = influxdb_servers_get(client, &server_list);
    size_t i;

    for (i = 0; i < nb; i++)
        influxdb_server_free(server_list[i]);

    free(server_list);

    return nb <= 0;
}

influxdb_test tests[] = {
    &ping,
    &get,
    NULL
};
