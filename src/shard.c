/*
 * Copyright (c) 2013-2014 InfluxDB
 * Pierre-Olivier Mercier <pomercier@novaquark.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#include <stdio.h>
#include <string.h>

#include "shard.h"

void
*influxdb_shard_extractor(json_object *row)
{
    s_influxdb_shard *shard = malloc(sizeof (s_influxdb_shard));

    shard->id = json_object_get_int(json_object_object_get(row, "id"));
    shard->endtime = json_object_get_int(
        json_object_object_get(row, "endTime"));
    shard->starttime = json_object_get_int(
        json_object_object_get(row, "startTime"));

    {
        json_object *servers = json_object_object_get(row, "serverIds");
        size_t j;

        shard->server_ids_length = json_object_array_length(servers);
        shard->server_ids = malloc(sizeof (uint32_t) *
                                   shard->server_ids_length);
        for (j = 0; j < shard->server_ids_length; j++)
        {
            shard->server_ids[j] = json_object_get_int(
                json_object_array_get_idx(servers, j));
        }
    }

    return shard;
}

size_t
influxdb_shards_from_json(json_object *jo, s_influxdb_shard ***shards)
{
    size_t i;
    size_t len = json_object_array_length(jo);
    *shards = malloc(sizeof (s_influxdb_shard *) * len);

    for (i = 0; i < len; i++)
        (*shards)[i] = influxdb_shard_extractor(
            json_object_array_get_idx(jo, i));

    return len;
}

int
influxdb_shards_get(s_influxdb_client *client,
                    s_influxdb_long_term_short_term_shards *response)
{
    json_object *jo;
    int status;

    status = influxdb_client_get(client, "/cluster/shards",
                                 &jo);

    if (status == 200)
    {
        response->longterm_length = influxdb_shards_from_json(
            json_object_object_get(jo, "longTerm"), &response->longterm);
        response->shortterm_length = influxdb_shards_from_json(
            json_object_object_get(jo, "shortTerm"), &response->shortterm);
    }

    return status;
}

int
influxdb_shard_drop(s_influxdb_client *client, uint32_t id,
                    uint32_t server_ids[], size_t server_ids_length)
{
    char path[INFLUXDB_URL_MAX_SIZE];
    int status;
    json_object *jo = json_object_new_object();
    json_object *jo_array = json_object_new_array();
    size_t i;

    for (i = 0; i < server_ids_length; i++)
        json_object_array_add(jo_array,
                              json_object_new_int(server_ids[i]));

    json_object_object_add(jo, "serverIds", jo_array);

    sprintf(path, "/cluster/shards/%u", id);

    status = influxdb_client_delete(client, path, jo);

    json_object_put(jo);

    return status;
}

void
influxdb_shard_free(s_influxdb_shard *shard)
{
    if (shard)
    {
        free(shard->server_ids);
        free(shard);
    }
}
