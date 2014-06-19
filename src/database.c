/*
 * Copyright (c) 2013-2014 InfluxDB
 * Pierre-Olivier Mercier <pomercier@novaquark.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#include <json-c/json.h>
#include <string.h>

#include "database.h"
#include "utils.h"

int
influxdb_create_database(s_influxdb_client *client, char *database_name)
{
    int c;
    json_object *jo = json_object_new_object();
    json_object_object_add(jo, "name", json_object_new_string(database_name));

    c = influxdb_client_post(client, "/db", jo, NULL);

    json_object_put(jo);

    return c;
}

int
influxdb_delete_database(s_influxdb_client *client, char *database_name)
{
    int c;
    char path[INFLUXDB_URL_MAX_SIZE];
    json_object *jo = json_object_new_object();
    json_object_object_add(jo, "name", json_object_new_string(database_name));

    path[0] = '\0';
    strncat(path, "/db/", INFLUXDB_URL_MAX_SIZE);
    strncat(path, database_name, INFLUXDB_URL_MAX_SIZE);

    c = influxdb_client_delete(client, path, NULL);

    json_object_put(jo);

    return c;
}

void
*influxdb_database_extractor(json_object *obj)
{
    return influxdb_strdup(
        json_object_get_string(
            json_object_object_get(obj, "name")
        ));
}

size_t
influxdb_get_database_list(s_influxdb_client *client, char ***databases_list)
{
    return influxdb_client_list_something(client, "/db",
                                          (void ***) databases_list,
                                          &influxdb_database_extractor);
}
