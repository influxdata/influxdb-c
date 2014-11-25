/*
 * Copyright (c) 2013-2014 InfluxDB
 * Pierre-Olivier Mercier <pomercier@novaquark.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#ifndef DATABASE_H_
# define DATABASE_H_

# include "client.h"

/**
 * Create a new database.
 *
 * \param client A initialized client
 * \param database_name The database name to create
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_create_database(s_influxdb_client *client,
                             const char        *database_name);

/**
 * Delete an existing database
 *
 * \param client A initialized client
 * \param database_name The database name to delete
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_delete_database(s_influxdb_client *client,
                             const char        *database_name);

/**
 * List existing databases
 *
 * \param client A initialized client
 * \param database_list The list to fill with database names
 * \return The number of databases
 */
size_t influxdb_get_database_list(s_influxdb_client *client,
                                  char              ***databases_list);

#endif /* !DATABASE_H_ */
