/*
 * Copyright (c) 2013-2014 InfluxDB
 * Pierre-Olivier Mercier <pomercier@novaquark.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#include <stdio.h>

#include "../src/database.h"
#include "tests.h"

int
find(s_influxdb_client *client)
{
    char **database_list;
    size_t nb = influxdb_get_database_list(client, &database_list);
    int find = 0;
    size_t i;

    for (i = 0; i < nb; i++) {
        if (!strcmp(database_list[i], INFLUXDB_DATABASE_TEST))
            find = 1;
        free(database_list[i]);
    }

    free(database_list);

    return !find;
}

int
create(s_influxdb_client *client)
{
    char *status_txt;
    int status_code;

    status_code = influxdb_create_database(client, INFLUXDB_DATABASE_TEST);

    return status_code != 201 || find(client);
}

int
delete(s_influxdb_client *client)
{
    char *status_txt;
    int status_code;

    status_code = influxdb_delete_database(client, INFLUXDB_DATABASE_TEST);

    return status_code != 204 || !find(client);
}

int
get(s_influxdb_client *client)
{
    char **database_list;
    size_t nb = influxdb_get_database_list(client, &database_list);
    size_t i;

    for (i = 0; i < nb; i++)
        free(database_list[i]);

    free(database_list);

    return nb <= 0;
}

influxdb_test tests[] = {
    &create,
    &get,
    &delete,
    NULL
};
