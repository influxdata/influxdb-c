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

#include "database_user.h"
#include "utils.h"

int
influxdb_create_database_user(s_influxdb_client *client, char *database,
                              char *name, char *password,
                              char *r_perm, char *w_perm)
{
    int c;
    char path[INFLUXDB_URL_MAX_SIZE];
    json_object *jo = json_object_new_object();
    json_object_object_add(jo, "name", json_object_new_string(name));
    json_object_object_add(jo, "password", json_object_new_string(password));
    json_object_object_add(jo, "readFrom", json_object_new_string(r_perm?r_perm:".*"));
    json_object_object_add(jo, "writeTo", json_object_new_string(w_perm?w_perm:".*"));

    path[0] = '\0';
    strncat(path, "/db/", INFLUXDB_URL_MAX_SIZE);
    strncat(path, database, INFLUXDB_URL_MAX_SIZE);
    strncat(path, "/users", INFLUXDB_URL_MAX_SIZE);

    c = influxdb_client_post(client, path, jo, NULL);

    json_object_put(jo);

    return c;
}

int
influxdb_change_database_user_common(s_influxdb_client *client, char *database,
                                   char *name, char *password, char isAdmin,
                                   char *r_perm, char *w_perm)
{
    int c;
    char path[INFLUXDB_URL_MAX_SIZE];
    json_object *jo = json_object_new_object();
    if (password)
        json_object_object_add(jo, "password", json_object_new_string(password));
    if (isAdmin >= 0)
        json_object_object_add(jo, "admin", json_object_new_boolean(isAdmin));
    if (r_perm)
        json_object_object_add(jo, "readFrom", json_object_new_string(r_perm));
    if (w_perm)
        json_object_object_add(jo, "writeTo", json_object_new_string(w_perm));

    path[0] = '\0';
    strncat(path, "/db/", INFLUXDB_URL_MAX_SIZE);
    strncat(path, database, INFLUXDB_URL_MAX_SIZE);
    strncat(path, "/users/", INFLUXDB_URL_MAX_SIZE);
    strncat(path, name, INFLUXDB_URL_MAX_SIZE);

    c = influxdb_client_post(client, path, jo, NULL);

    json_object_put(jo);

    return c;
}

int
influxdb_change_database_user(s_influxdb_client *client, char *database,
                              char *name, char *newPassword, char isAdmin,
                              char *new_r_perm, char *new_w_perm)
{
    return influxdb_change_database_user_common(client, database, name,
                                                newPassword, isAdmin,
                                                new_r_perm, new_w_perm);
}


int
influxdb_update_database_user(s_influxdb_client *client, char *database,
                              char *name, char *password)
{
    return influxdb_change_database_user_common(client, database, name,
                                                password, -1, NULL, NULL);
}

int influxdb_update_database_user_permissions(s_influxdb_client *client,
                                              char *database, char *name,
                                              char *r_perm, char *w_perm)
{
    return influxdb_change_database_user_common(client, database, name,
                                                NULL, -1, r_perm, w_perm);
}

int influxdb_alter_database_privilege(s_influxdb_client *client, char *database,
                                      char *name, char isAdmin,
                                      char *r_perm, char *w_perm)
{
    return influxdb_change_database_user_common(client, database, name,
                                                NULL, isAdmin, r_perm, w_perm);
}

int
influxdb_delete_database_user(s_influxdb_client *client, char *database,
                              char *name)
{
    int c;
    char path[INFLUXDB_URL_MAX_SIZE];
    json_object *jo = json_object_new_object();
    json_object_object_add(jo, "name", json_object_new_string(database));


    path[0] = '\0';
    strncat(path, "/db/", INFLUXDB_URL_MAX_SIZE);
    strncat(path, database, INFLUXDB_URL_MAX_SIZE);
    strncat(path, "/users/", INFLUXDB_URL_MAX_SIZE);
    strncat(path, name, INFLUXDB_URL_MAX_SIZE);

    c = influxdb_client_delete(client, path, NULL);

    json_object_put(jo);

    return c;
}

void
*influxdb_dbusers_extractor(json_object *obj)
{
    return influxdb_strdup(
        json_object_get_string(
            json_object_object_get(obj, "name")
        ));
}

size_t
influxdb_get_database_user_list(s_influxdb_client *client, char *database,
                                char ***users_list)
{
    char path[INFLUXDB_URL_MAX_SIZE];

    path[0] = '\0';
    strncat(path, "/db/", INFLUXDB_URL_MAX_SIZE);
    strncat(path, database, INFLUXDB_URL_MAX_SIZE);
    strncat(path, "/users", INFLUXDB_URL_MAX_SIZE);

    return influxdb_client_list_something(client, path,
                                          (void ***) users_list,
                                          &influxdb_dbusers_extractor);
}
