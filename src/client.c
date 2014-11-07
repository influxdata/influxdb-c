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

#include "client.h"
#include "utils.h"

s_influxdb_client
*influxdb_client_new(char *host,
                     char *username,
                     char *password,
                     char *database,
                     char ssl)
{
    return influxdb_client_new_with_timeout(host, username, password, database, ssl, 0, 0);
}

s_influxdb_client
*influxdb_client_new_with_timeout(char *host,
                     char *username,
                     char *password,
                     char *database,
                     char ssl,
		     int connection_timeout,
                     int max_time)
{
    s_influxdb_client *client = malloc(sizeof (s_influxdb_client));

    client->schema = ssl ? "https" : "http";
    client->host = influxdb_strdup(host);
    client->username = influxdb_strdup(username);
    client->password = influxdb_strdup(password);
    client->database = curl_easy_escape(NULL, database, 0);
    client->ssl = ssl;
    client->connection_timeout=connection_timeout;
    client->max_time=max_time;

    return client;

}


/**
 * Forge real URL to the API using given client config and parameters
 */
int
influxdb_client_get_url_with_credential(s_influxdb_client *client,
                                        char (*buffer)[],
                                        size_t size,
                                        char *path,
                                        char *username,
                                        char *password)
{
    char *username_enc = curl_easy_escape(NULL, username, 0);
    char *password_enc = curl_easy_escape(NULL, password, 0);

    (*buffer)[0] = '\0';
    strncat(*buffer, client->schema, size);
    strncat(*buffer, "://", size);
    strncat(*buffer, client->host, size);
    strncat(*buffer, path, size);

    if (strchr(path, '?'))
        strncat(*buffer, "&", size);
    else
        strncat(*buffer, "?", size);

    strncat(*buffer, "u=", size);
    strncat(*buffer, username_enc, size);
    strncat(*buffer, "&p=", size);
    strncat(*buffer, password_enc, size);

    free(username_enc);
    free(password_enc);

    return strlen(*buffer);
}

/**
 * Forge real URL to the API using only given client config
 */
int
influxdb_client_get_url(s_influxdb_client *client,
                        char (*buffer)[],
                        size_t size,
                        char *path)
{
    return influxdb_client_get_url_with_credential(client, buffer, size, path,
                                                   client->username,
                                                   client->password);
}

/**
 * CURL Callback reading data to userdata buffer
 */
size_t
influxdb_client_write_data(char *buf,
                           size_t size,
                           size_t nmemb,
                           void *userdata)
{
    size_t realsize = size * nmemb;
    if (userdata != NULL)
    {
        char **buffer = userdata;

        *buffer = realloc(*buffer, strlen(*buffer) + realsize + 1);

        strncat(*buffer, buf, realsize);
    }
    return realsize;
}

/**
 * Low level function performing real HTTP request
 */
int
influxdb_client_curl(char *url,
                     char *reqtype,
                     json_object *body,
                     char **response,
                     int connection_timeout,
                     int max_time)
{
    CURLcode c;
    CURL *handle = curl_easy_init();

    if (reqtype != NULL)
        curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, reqtype);
    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION,
                     influxdb_client_write_data);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, response);

    if (connection_timeout>0)
        curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT,
                     connection_timeout);

    if (max_time>0)
        curl_easy_setopt(handle, CURLOPT_TIMEOUT,
                     max_time);

    if (body != NULL)
        curl_easy_setopt(handle, CURLOPT_POSTFIELDS,
                         json_object_to_json_string(body));

    c = curl_easy_perform(handle);

    if (c == CURLE_OK)
    {
        long status_code = 0;
        if (curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE,
                              &status_code) == CURLE_OK)
            c = status_code;
    }

    curl_easy_cleanup(handle);

    return c;
}

int
influxdb_client_delete(s_influxdb_client *client,
                       char *path,
                       json_object *body)
{
    char url[INFLUXDB_URL_MAX_SIZE];

    influxdb_client_get_url(client, &url, INFLUXDB_URL_MAX_SIZE, path);

    return influxdb_client_curl(url, "DELETE", body, NULL, client->connection_timeout, client->max_time);
}

int
influxdb_client_get(s_influxdb_client *client, char *path, json_object **res)
{
    int status;
    char url[INFLUXDB_URL_MAX_SIZE];
    char *buffer = calloc(1, sizeof (char));

    influxdb_client_get_url(client, &url, INFLUXDB_URL_MAX_SIZE, path);

    status = influxdb_client_curl(url, NULL, NULL, &buffer, client->connection_timeout, client->max_time);

    if (status >= 200 && status < 300 && res != NULL)
        *res = json_tokener_parse(buffer);

    free(buffer);

    return status;
}

int
influxdb_client_post(s_influxdb_client *client,
                     char *path,
                     json_object *body,
                     json_object **res)
{
    int status;
    char url[INFLUXDB_URL_MAX_SIZE];
    char *buffer = calloc(1, sizeof (char));

    influxdb_client_get_url(client, &url, INFLUXDB_URL_MAX_SIZE, path);

    status = influxdb_client_curl(url, NULL, body, &buffer, client->connection_timeout, client->max_time);

    if (status >= 200 && status < 300 && res != NULL)
        *res = json_tokener_parse(buffer);

    free(buffer);

    return status;
}

size_t
influxdb_client_list_something(s_influxdb_client *client,
                               char *path,
                               void ***list,
                               influxdb_client_object_extract extractor)
{
    json_object *jo = NULL;
    size_t len, i;

    if (influxdb_client_get(client, path, &jo) != 200)
        return 0;

    len = json_object_array_length(jo);

    if (list != NULL)
    {
        *list = malloc(sizeof (char *) * (len + 1));

        for (i = 0; i < len; i++) {
            (*list)[i] = extractor(json_object_array_get_idx(jo, i));
        }
    }

    json_object_put(jo);

    return len;
}

void
influxdb_client_free(s_influxdb_client *client)
{
    if (client)
    {
        curl_free(client->username);
        curl_free(client->password);
        curl_free(client->database);
        free(client->host);
        free(client);
    }
}
