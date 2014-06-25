/*
 * Copyright (c) 2013-2014 InfluxDB
 * Pierre-Olivier Mercier <pomercier@novaquark.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#ifndef SHARD_H_
# define SHARD_H_

# include <stdint.h>
# include "client.h"

typedef struct influxdb_shard {
    uint32_t id;
    int64_t endtime;
    int64_t starttime;
    uint32_t *server_ids;
    size_t server_ids_length;
} s_influxdb_shard;

typedef struct influxdb_long_term_short_term_shards {
    s_influxdb_shard **longterm;
    size_t longterm_length;
    s_influxdb_shard **shortterm;
    size_t shortterm_length;
} s_influxdb_long_term_short_term_shards;

/**
 * \param client A initialized client
 * \param response The list to fill with
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_shards_get(s_influxdb_client *client,
                        s_influxdb_long_term_short_term_shards *response);

/**
 * \param client A initialized client
 * \param id The server identifier to remove
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_shard_drop(s_influxdb_client *client,
                        uint32_t id,
                        uint32_t server_ids[],
                        size_t server_ids_length);

/**
 * Clean memory used by a shard
 */
void influxdb_shard_free(s_influxdb_shard *shard);

#endif /* !SHARD_H_ */
