/*
 * Copyright (c) 2013-2014 InfluxDB
 * Pierre-Olivier Mercier <pomercier@novaquark.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#include <curl/curl.h>
#include <string.h>

#include "query.h"
#include "utils.h"

int
influxdb_write_serie(s_influxdb_client *client,
                     s_influxdb_series *serie)
{
    return influxdb_write_serie_with_time_precision(client, serie,
                                                    INFLUXDB_TP_DEFAULT);
}

int
influxdb_write_serie_with_time_precision(s_influxdb_client *client,
                                      s_influxdb_series *serie,
                                      e_influxdb_time_precision time_precision)
{
    s_influxdb_series *series[1];
    series[0] = serie;

    return influxdb_write_series_with_time_precision(client, series, 1,
                                                     time_precision);
}

int
influxdb_write_series(s_influxdb_client *client,
                      s_influxdb_series *series[],
                      size_t series_length)
{
    return influxdb_write_series_with_time_precision(client, series,
                                                     series_length,
                                                     INFLUXDB_TP_DEFAULT);
}

int
influxdb_write_series_with_time_precision(s_influxdb_client *client,
                                      s_influxdb_series *series[],
                                      size_t series_length,
                                      e_influxdb_time_precision time_precision)
{
    int c;
    char path[INFLUXDB_URL_MAX_SIZE];
    json_object *jo = influxdb_series_to_json(series, series_length);

    path[0] = '\0';
    strncat(path, "/db/", INFLUXDB_URL_MAX_SIZE);
    strncat(path, client->database, INFLUXDB_URL_MAX_SIZE);
    strncat(path, "/series", INFLUXDB_URL_MAX_SIZE);

    if (time_precision != INFLUXDB_TP_DEFAULT)
    {
        strncat(path, "?time_precision=", INFLUXDB_URL_MAX_SIZE);
        switch (time_precision)
        {
        case INFLUXDB_TP_MILLISEC:
            strncat(path, "m", INFLUXDB_URL_MAX_SIZE);
            break;
        case INFLUXDB_TP_MICROSEC:
            strncat(path, "u", INFLUXDB_URL_MAX_SIZE);
            break;
        default:
            strncat(path, "s", INFLUXDB_URL_MAX_SIZE);
        }
    }

    c = influxdb_client_post(client, path, jo, NULL);

    json_object_put(jo);

    return c;
}

int
influxdb_query(s_influxdb_client *client,
               char *query,
               e_influxdb_time_precision time_precision,
               s_influxdb_series ***response,
               size_t *response_size)
{
    int c;
    char path[INFLUXDB_URL_MAX_SIZE * 2];
    char *escaped_query;
    json_object *jo;

    path[0] = '\0';
    strncat(path, "/db/", INFLUXDB_URL_MAX_SIZE * 2);
    strncat(path, client->database, INFLUXDB_URL_MAX_SIZE * 2);
    strncat(path, "/series?q=", INFLUXDB_URL_MAX_SIZE * 2);

    escaped_query = curl_easy_escape(NULL, query, 0);
    strncat(path, escaped_query, INFLUXDB_URL_MAX_SIZE * 2);
    free(escaped_query);

    if (time_precision != INFLUXDB_TP_DEFAULT)
    {
        strncat(path, "&time_precision=", INFLUXDB_URL_MAX_SIZE * 2);
        switch (time_precision)
        {
        case INFLUXDB_TP_MILLISEC:
            strncat(path, "m", INFLUXDB_URL_MAX_SIZE * 2);
            break;
        case INFLUXDB_TP_MICROSEC:
            strncat(path, "u", INFLUXDB_URL_MAX_SIZE * 2);
            break;
        default:
            strncat(path, "s", INFLUXDB_URL_MAX_SIZE * 2);
        }
    }

    c = influxdb_client_get(client, path, &jo);
    if (c >= 200 && c < 300) {
      *response_size = influxdb_series_from_json(jo, response);
      json_object_put(jo);
    }

    return c;
}

void
*influxdb_continuous_queries_extractor(json_object *obj)
{
    s_influxdb_continuous_query *cq =
        malloc(sizeof (s_influxdb_continuous_query));

    cq->id = json_object_get_int(
        json_object_object_get(obj, "id"));
    cq->query = influxdb_strdup(json_object_get_string(
        json_object_object_get(obj, "query")));

    return cq;
}

size_t
influxdb_get_continuous_queries(s_influxdb_client *client,
                                s_influxdb_continuous_query ***response)
{
    char path[INFLUXDB_URL_MAX_SIZE];

    path[0] = '\0';
    strncat(path, "/db/", INFLUXDB_URL_MAX_SIZE);
    strncat(path, client->database, INFLUXDB_URL_MAX_SIZE);
    strncat(path, "/continuous_queries", INFLUXDB_URL_MAX_SIZE);

    return influxdb_client_list_something(client, path, (void ***) response,
                                       &influxdb_continuous_queries_extractor);
}

int
influxdb_delete_continuous_query(s_influxdb_client *client,
                                   int id)
{
    char path[INFLUXDB_URL_MAX_SIZE];

    sprintf(path, "/db/%s/continuous_queries/%d", client->database, id);

    return influxdb_client_delete(client, path, NULL);
}


void
influxdb_continuous_query_free(s_influxdb_continuous_query *cq)
{
    if (cq != NULL)
    {
        free(cq->query);
        free(cq);
    }
}
