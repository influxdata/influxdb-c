/*
 * Copyright (c) 2013-2014 InfluxDB
 * Pierre-Olivier Mercier <pomercier@novaquark.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#include <stdio.h>

#include "../src/cluster_admin.h"
#include "tests.h"

int
find(s_influxdb_client *client)
{
    int find = 0;
    char **cluster_admin_list;
    size_t nb = influxdb_get_cluster_admin_list(client, &cluster_admin_list);
    size_t i;

    for (i = 0; i < nb; i++) {
        if (!strcmp(cluster_admin_list[i], INFLUXDB_CLUSTERADMIN_TEST))
            find = 1;
        free(cluster_admin_list[i]);
    }

    free(cluster_admin_list);

    return !find;
}

int
create(s_influxdb_client *client)
{
    char *status_txt;
    int status_code;

    status_code = influxdb_create_cluster_admin(client,
                                                INFLUXDB_CLUSTERADMIN_TEST,
                                                INFLUXDB_DATABASE_TEST);

    return status_code != 200 || find(client);
}


int
delete(s_influxdb_client *client)
{
    char *status_txt;
    int status_code;

    status_code = influxdb_delete_cluster_admin(client, INFLUXDB_CLUSTERADMIN_TEST);

    return status_code != 200 || !find(client);
}

int
chpass(s_influxdb_client *client)
{
    char *status_txt;
    int status_code;

    status_code = influxdb_change_cluster_admin_password(client, INFLUXDB_CLUSTERADMIN_TEST, "1234");

    return status_code != 200 || find(client);
}

int
get(s_influxdb_client *client)
{
    char **cluster_admin_list;
    size_t nb = influxdb_get_cluster_admin_list(client, &cluster_admin_list);
    size_t i;

    for (i = 0; i < nb; i++) {
        free(cluster_admin_list[i]);
    }

    free(cluster_admin_list);

    return nb <= 0;
}

influxdb_test tests[] = {
    &create,
    &get,
    &chpass,
    &delete,
    NULL
};
