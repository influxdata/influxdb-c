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
#include "../src/database_user.h"
#include "tests.h"

int
find(s_influxdb_client *client)
{
    int find = 0;
    char **users_list;
    size_t nb = influxdb_get_database_user_list(client, INFLUXDB_DATABASE_TEST, &users_list);
    size_t i;

    for (i = 0; i < nb; i++) {
        if (!strcmp(users_list[i], INFLUXDB_CLUSTERADMIN_TEST))
            find = 1;
        free(users_list[i]);
    }

    free(users_list);

    return !find;
}

int
create_db(s_influxdb_client *client)
{
    char *status_txt;
    int status_code;

    status_code = influxdb_create_database(client, INFLUXDB_DATABASE_TEST);

    return status_code != 201;
}

int
delete_db(s_influxdb_client *client)
{
    char *status_txt;
    int status_code;

    status_code = influxdb_delete_database(client, INFLUXDB_DATABASE_TEST);

    return status_code != 204;
}

int
create(s_influxdb_client *client)
{
    char *status_txt;
    int status_code;

    status_code = influxdb_create_database_user(client,
                                                INFLUXDB_DATABASE_TEST,
                                                INFLUXDB_CLUSTERADMIN_TEST,
                                                INFLUXDB_DATABASE_TEST, NULL, NULL);

    return status_code != 200 || find(client);
}

int
delete(s_influxdb_client *client)
{
    char *status_txt;
    int status_code;

    status_code = influxdb_delete_database_user(client,
                                                INFLUXDB_DATABASE_TEST,
                                                INFLUXDB_CLUSTERADMIN_TEST);

    return status_code != 200 || !find(client);
}

int
chpass(s_influxdb_client *client)
{
    char *status_txt;
    int status_code;

    status_code = influxdb_update_database_user(client,
                                                INFLUXDB_DATABASE_TEST,
                                                INFLUXDB_CLUSTERADMIN_TEST,
                                                "1234");

    return status_code != 200 || find(client);
}

int
get_users(s_influxdb_client *client)
{
    char **users_list;
    size_t nb = influxdb_get_database_user_list(client, INFLUXDB_DATABASE_TEST, &users_list);
    size_t i;

    for (i = 0; i < nb; i++)
        free(users_list[i]);

    free(users_list);

    return nb;
}

int
nb_user_start(s_influxdb_client *client)
{
    return get_users(client) != 0;
}
int
nb_user_middle(s_influxdb_client *client)
{
    return get_users(client) != 1;
}

influxdb_test tests[] = {
    &nb_user_start,
    &create_db,
    &create,
    &nb_user_middle,
    &chpass,
    &delete,
    &nb_user_start,
    &delete_db,
    NULL
};
