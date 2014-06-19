/*
 * Copyright (c) 2013-2014 InfluxDB
 * Pierre-Olivier Mercier <pomercier@novaquark.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#ifndef CLIENT_H_
# define CLIENT_H_

# include <json-c/json.h>

# define INFLUXDB_URL_MAX_SIZE 1024

typedef struct influxdb_client {
    char *schema;
    char *host;
    char *username;
    char *password;
    char *database;
    char ssl;
} s_influxdb_client;

/**
 * Initialize a client struct with given parameters
 *
 * \return An initialized client struct
 */
s_influxdb_client *influxdb_client_new(char *host,
                                       char *username,
                                       char *password,
                                       char *database,
                                       char ssl);

/**
 * Clean memory used by a client
 */
void influxdb_client_free(s_influxdb_client *client);

/**
 * Perform a DELETE request to API
 *
 * \param client A initialized client
 * \param path Relative path from API root
 * \param body JSON sent as request content, can be NULL
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_client_delete(s_influxdb_client *client,
                           char              *path,
                           json_object       *body);

/**
 * Perform a GET request to API
 *
 * \param client A initialized client
 * \param path Relative path from API root
 * \param res Parsed JSON response, can be NULL
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_client_get(s_influxdb_client *client,
                        char              *path,
                        json_object       **res);

/**
 * Perform a POST request to API
 *
 * \param client A initialized client
 * \param path Relative path from API root
 * \param body JSON sent as request content, can be NULL
 * \param res Parsed JSON response, can be NULL
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_client_post(s_influxdb_client *client,
                         char              *path,
                         json_object       *body,
                         json_object       **res);

typedef void *(*influxdb_client_object_extract)(json_object *obj);

/**
 * Return a list of elements
 *
 * \param client A initialized client
 * \param path Relative path from API root
 * \param db_list The non-malloced list where store response, can be NULL (just count)
 * \return The list size
 */
size_t influxdb_client_list_something(s_influxdb_client             *client,
                                     char                           *path,
                                     void                           ***list,
                                     influxdb_client_object_extract extractor);

#endif /* !CLIENT_H_ */
