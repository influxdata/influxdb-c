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

#include "cluster_admin.h"
#include "utils.h"

int influxdb_create_cluster_admin(s_influxdb_client *client,
                                  char *name,
                                  char *password)
{
    int c;
    json_object *jo = json_object_new_object();
    json_object_object_add(jo, "name", json_object_new_string(name));
    json_object_object_add(jo, "password", json_object_new_string(password));

    c = influxdb_client_post(client, "/cluster_admins", jo, NULL);

    json_object_put(jo);

    return c;
}

int influxdb_update_cluster_admin(s_influxdb_client *client,
                                  char *name,
                                  char *password)
{
    int c;
    char path[INFLUXDB_URL_MAX_SIZE];
    json_object *jo = json_object_new_object();
    json_object_object_add(jo, "password", json_object_new_string(password));

    path[0] = '\0';
    strncat(path, "/cluster_admins/", INFLUXDB_URL_MAX_SIZE);
    strncat(path, name, INFLUXDB_URL_MAX_SIZE);

    c = influxdb_client_post(client, path, jo, NULL);

    json_object_put(jo);

    return c;
}

int influxdb_delete_cluster_admin(s_influxdb_client *client,
                                  char *name)
{
    int c;
    char path[INFLUXDB_URL_MAX_SIZE];
    json_object *jo = json_object_new_object();
    json_object_object_add(jo, "name", json_object_new_string(name));

    path[0] = '\0';
    strncat(path, "/cluster_admins/", INFLUXDB_URL_MAX_SIZE);
    strncat(path, name, INFLUXDB_URL_MAX_SIZE);

    c = influxdb_client_delete(client, path, NULL);

    json_object_put(jo);

    return c;
}

void
*influxdb_cluster_admin_extractor(json_object *obj)
{
    return influxdb_strdup(
        json_object_get_string(
            json_object_object_get(obj, "name")
        ));
}

size_t influxdb_get_cluster_admin_list(s_influxdb_client *client,
                                       char ***cluster_admin_list)
{
    return influxdb_client_list_something(client, "/cluster_admins",
                                          (void ***) cluster_admin_list,
                                          &influxdb_cluster_admin_extractor);
}
