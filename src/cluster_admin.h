/*
 * Copyright (c) 2013-2014 InfluxDB
 * Pierre-Olivier Mercier <pomercier@novaquark.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#ifndef CLUSTER_ADMIN_H_
# define CLUSTER_ADMIN_H_

# include "client.h"

/**
 * Create a new cluster admin
 *
 * \param client A initialized client
 * \param name The admin name
 * \param password The admin password
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_create_cluster_admin(s_influxdb_client *client,
                                  char              *name,
                                  char              *password);

/**
 * Update a cluster admin password
 *
 * \param client A initialized client
 * \param name The admin name
 * \param password The new admin password to use
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_update_cluster_admin(s_influxdb_client *client,
                                  char              *name,
                                  char              *password);

/**
 * Remove a cluster admin password
 *
 * \param client A initialized client
 * \param name The admin name
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_delete_cluster_admin(s_influxdb_client *client,
                                  char              *name);

/**
 * List existing cluster admins
 *
 * \param client A initialized client
 * \param cluster_admin_list The list to fill with cluster admin names
 * \return The number of cluster admins
 */
size_t influxdb_get_cluster_admin_list(s_influxdb_client *client,
                                       char             ***cluster_admin_list);

#endif /* !CLUSTER_ADMIN_H_ */
