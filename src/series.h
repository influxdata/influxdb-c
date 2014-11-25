/*
 * Copyright (c) 2013-2014 InfluxDB
 * Pierre-Olivier Mercier <pomercier@novaquark.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#ifndef SERIES_H_
# define SERIES_H_

# include <stddef.h>
# include "client.h"

# define INFLUXDB_SERIES_STEP 64

typedef void (*influxdb_series_free_callback)(char **row);

typedef struct influxdb_series {
    char   *name;
    char   **columns;
    char   ***points;
    size_t columns_length;
    size_t points_length;
    influxdb_series_free_callback free_cb;
} s_influxdb_series;

/**
 * Retrive the serie name
 */
char *influxdb_series_get_name(s_influxdb_series *series);

/**
 * Retrive the columns list
 */
size_t influxdb_series_get_columns(s_influxdb_series *series, char ***list);

/**
 * Retrive the points matrix
 */
size_t influxdb_series_get_points(s_influxdb_series *series, char ****list);


/**
 * Define the serie name
 */
char influxdb_series_set_name(s_influxdb_series *series, const char *name);

/**
 * Define the columns list
 */
char **influxdb_series_set_columns(s_influxdb_series *series,
                                   char **columns,
                                   size_t length);

/**
 * Add a new column
 */
char influxdb_series_add_colums(s_influxdb_series *series, const char *name);

/**
 * Define the points matrix
 */
char ***influxdb_series_set_points(s_influxdb_series *series,
                                   char ***matrix,
                                   size_t length);

/**
 * Add a new points row to matrix
 */
char influxdb_series_add_points(s_influxdb_series *series, char **row);


/**
 * Initialize a new series
 */
s_influxdb_series *influxdb_series_create(char *name,
                                        influxdb_series_free_callback free_cb);

/**
 * Clean memory used by a series
 */
void influxdb_series_free(s_influxdb_series *series,
                          influxdb_series_free_callback free_cb);

/**
 * Convert series into a JSON object
 */
json_object *influxdb_series_to_json(s_influxdb_series *series[], size_t series_length);

/**
 * Convert a JSON object to series struct array
 */
size_t influxdb_series_from_json(json_object *jo, s_influxdb_series ***series);

#endif /* !SERIES_H_ */
