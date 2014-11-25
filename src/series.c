/*
 * Copyright (c) 2013-2014 InfluxDB
 * Pierre-Olivier Mercier <pomercier@novaquark.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#include "series.h"
#include "utils.h"

char
*influxdb_series_get_name(s_influxdb_series *series)
{
    return series->name;
}

size_t
influxdb_series_get_columns(s_influxdb_series *series, char ***list)
{
    *list = series->columns;
    return series->columns_length;
}

size_t influxdb_series_get_points(s_influxdb_series *series, char ****list)
{
    *list = series->points;
    return series->points_length;
}

char
influxdb_series_set_name(s_influxdb_series *series, const char *name)
{
    free(series->name);
    series->name = influxdb_strdup(name);
    return 1;
}

char
**influxdb_series_set_columns(s_influxdb_series *series,
                              char **columns,
                              size_t length)
{
    char **old = series->columns;
    series->columns = columns;
    series->columns_length = length;
    return old;
}

char
influxdb_series_add_colums(s_influxdb_series *series,const char *name)
{
    if (series->columns_length == 0) {
        series->columns = malloc(sizeof (char *) * INFLUXDB_SERIES_STEP);
    } else if (series->columns_length % INFLUXDB_SERIES_STEP == 0) {
        series->columns = realloc(series->columns,
            sizeof (char *) * (series->columns_length + INFLUXDB_SERIES_STEP));
    }

    if (series->columns == NULL)
        return 0;

    series->columns[series->columns_length++] = influxdb_strdup(name);

    return 1;
}

char
***influxdb_series_set_points(s_influxdb_series *series,
                              char ***matrix,
                              size_t length)
{
    char ***old = series->points;
    series->points = matrix;
    series->points_length = length;
    return old;
}

char
influxdb_series_add_points(s_influxdb_series *series, char **row)
{
    if (series->points_length == 0) {
        series->points = malloc(sizeof (char *) * INFLUXDB_SERIES_STEP);
    } else if (series->points_length % INFLUXDB_SERIES_STEP == 0) {
        series->points = realloc(series->points,
            sizeof (char *) * (series->points_length + INFLUXDB_SERIES_STEP));
    }

    if (series->points == NULL)
        return 0;

    series->points[series->points_length++] = row;

    return 1;
}

s_influxdb_series
*influxdb_series_create(char *name, influxdb_series_free_callback free_cb)
{
    s_influxdb_series *series = malloc(sizeof (s_influxdb_series));

    if (series != NULL)
    {
        series->name = influxdb_strdup(name);
        series->columns = NULL;
        series->points = NULL;
        series->columns_length = 0;
        series->points_length = 0;
        series->free_cb = free_cb;
    }

    return series;
}

void
influxdb_series_free(s_influxdb_series *series,
                     influxdb_series_free_callback free_cb)
{
    if (series)
    {
        size_t i, j;

        free(series->name);

        for (i = 0; i < series->points_length; i++)
        {
            if (free_cb != NULL)
                free_cb(series->points[i]);
            else if (series->free_cb != NULL)
                series->free_cb(series->points[i]);
            else
            {
                for (j = 0; j < series->columns_length; j++)
                    free(series->points[i][j]);
                free(series->points[i]);
            }
        }
        free(series->points);
        series->points_length = 0;

        for (i = 0; i < series->columns_length; i++)
            free(series->columns[i]);
        free(series->columns);
        series->columns_length = 0;

        free(series);
    }
}

json_object
*influxdb_serie_to_json(s_influxdb_series *series)
{
    json_object *jo = json_object_new_object();

    /* Name */
    json_object_object_add(jo, "name", json_object_new_string(series->name));

    /* Columns */
    {
        size_t i;
        json_object *cols = json_object_new_array();

        for (i = 0; i < series->columns_length; i++)
            json_object_array_add(cols,
                                  json_object_new_string(series->columns[i]));

        json_object_object_add(jo, "columns", cols);
    }

    /* Points */
    {
        size_t i, j;
        json_object *cols = json_object_new_array();

        for (i = 0; i < series->points_length; i++)
        {
            json_object *row = json_object_new_array();

            for (j = 0; j < series->columns_length; j++)
                json_object_array_add(row,
                    json_object_new_string(series->points[i][j]));

            json_object_array_add(cols, row);
        }

        json_object_object_add(jo, "points", cols);
    }

    return jo;
}

s_influxdb_series
*influxdb_serie_from_json(json_object *jo)
{
    s_influxdb_series *series = malloc(sizeof (s_influxdb_series));

    series->free_cb = NULL;

    /* Name */
    series->name = influxdb_strdup(
        json_object_get_string(
            json_object_object_get(jo, "name")
            )
        );

    /* Columns */
    {
        json_object *cols = json_object_object_get(jo, "columns");

        series->columns_length = json_object_array_length(cols);
        series->columns = malloc(sizeof (char *) * series->columns_length);

        {
            size_t i;
            for (i = 0; i < series->columns_length; i++)
                series->columns[i] = influxdb_strdup(
                    json_object_get_string(
                        json_object_array_get_idx(cols, i)
                        )
                    );
        }
    }

    /* Points */
    {
        json_object *points = json_object_object_get(jo, "points");

        series->points_length = json_object_array_length(points);
        series->points = malloc(sizeof (char **) * series->points_length);

        {
            size_t i, j;
            for (i = 0; i < series->points_length; i++)
            {
                json_object *cols = json_object_array_get_idx(points, i);

                series->points[i] = malloc(
                    sizeof (char *) * series->columns_length);

                for (j = 0; j < series->columns_length; j++)
                    series->points[i][j] = influxdb_strdup(
                        json_object_get_string(
                            json_object_array_get_idx(cols, j)
                            )
                        );
            }
        }
    }

    return series;
}

json_object
*influxdb_series_to_json(s_influxdb_series *series[], size_t series_length)
{
    size_t i;
    json_object *sa = json_object_new_array();

    for (i = 0; i < series_length; i++)
    {
        json_object_array_add(sa,
                              influxdb_serie_to_json(series[i])
            );
    }

    return sa;
}

size_t
influxdb_series_from_json(json_object *jo, s_influxdb_series ***series)
{
    size_t i, len = json_object_array_length(jo);

    *series = malloc(sizeof (s_influxdb_series *) * len);

    for (i = 0; i < len; i++)
    {
        (*series)[i] = influxdb_serie_from_json(
            json_object_array_get_idx(jo, i)
            );
    }

    return len;
}
