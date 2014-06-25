/*
 * Copyright (c) 2013-2014 InfluxDB
 * Pierre-Olivier Mercier <pomercier@novaquark.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#ifndef QUERY_H_
# define QUERY_H_

# include "client.h"
# include "series.h"

typedef enum influxdb_time_precision {
    INFLUXDB_TP_DEFAULT = 0,
    INFLUXDB_TP_SEC,
    INFLUXDB_TP_MILLISEC,
    INFLUXDB_TP_MICROSEC
} e_influxdb_time_precision;

/**
 * Send a serie to the DB
 *
 * \param client A initialized client
 * \param serie The serie to write
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_write_serie(s_influxdb_client *client,
                         s_influxdb_series *serie);

/**
 * Send a serie to the DB
 *
 * \param client A initialized client
 * \param serie The serie to write
 * \param time_precision The time precision
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_write_serie_with_time_precision(s_influxdb_client *client,
                                     s_influxdb_series *serie,
                                     e_influxdb_time_precision time_precision);

/**
 * Send some series to the DB
 *
 * \param client A initialized client
 * \param serie The serie to write
 * \param series_length The number of series in the precedent argument
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_write_series(s_influxdb_client *client,
                          s_influxdb_series *series[],
                          size_t series_length);

/**
 * Send some series to the DB
 *
 * \param client A initialized client
 * \param serie The serie to write
 * \param series_length The number of series in the precedent argument
 * \param time_precision The time precision
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_write_series_with_time_precision(s_influxdb_client *client,
                                     s_influxdb_series *series[],
                                     size_t series_length,
                                     e_influxdb_time_precision time_precision);

/**
 * Retrive some series from database
 *
 * \param client A initialized client
 * \param query The query to execute
 * \param time_precision The time precision
 * \param response The series array to fill
 * \param response_size The number of series retrived
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_query(s_influxdb_client *client,
                   char *query,
                   e_influxdb_time_precision time_precision,
                   s_influxdb_series ***response,
                   size_t *response_size);

typedef struct influxdb_continuous_queries {
    int  id;
    char *query;
} s_influxdb_continuous_query;

/**
 * Retrive list of continuous queries
 *
 * \param client An initialized client
 * \param response The continuous queries array to fill
 * \return The size of the filled array
 */
size_t influxdb_get_continuous_queries(s_influxdb_client *client,
                                      s_influxdb_continuous_query ***response);

/**
 * Send a delete request for a given module
 *
 * \param client A initialized client
 * \param id The database to remove
 * return HTTP status code or CURLcode (if < 100)
 */
int influxdb_delete_continuous_query(s_influxdb_client *client,
                                     int id);

/**
 * Clean memory used by a continous query
 */
void influxdb_continuous_query_free(s_influxdb_continuous_query *cq);

#endif /* !QUERY_H_ */
