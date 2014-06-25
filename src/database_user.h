/*
 * Copyright (c) 2013-2014 InfluxDB
 * Pierre-Olivier Mercier <pomercier@novaquark.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#ifndef DATABASE_USER_H_
# define DATABASE_USER_H_

# include "client.h"

/**
 * Create a new database user.
 *
 * \param client A initialized client
 * \param database The database into which add the user
 * \param name The user name to create
 * \param password The user password
 * \param r_perm Optional read permissions
 * \param w_perm Optional write permissions
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_create_database_user(s_influxdb_client *client,
                                  char              *database,
                                  char              *name,
                                  char              *password,
                                  char              *r_perm,
                                  char              *w_perm);

/**
 * Change the user password, adming flag and optionally permissions
 *
 * \param client A initialized client
 * \param name The user name to update
 * Check InfluxDB documentation for other parameters
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_change_database_user(s_influxdb_client *client,
                                  char              *database,
                                  char              *name,
                                  char              *newPassword,
                                  char              isAdmin,
                                  char              *new_r_perm,
                                  char              *new_w_perm);

/**
 * Update some user properties
 *
 * \param client A initialized client
 * \param name The user name to update
 * Check InfluxDB documentation for other parameters
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_update_database_user(s_influxdb_client *client,
                                  char              *database,
                                  char              *name,
                                  char              *newPassword);

/**
 * Update user permissions on database
 *
 * \param client A initialized client
 * \param database The database name on which permission will apply
 * \param name The user name to update
 * \param r_perm Optional read permissions
 * \param w_perm Optional write permissions
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_update_database_user_permissions(s_influxdb_client *client,
                                              char              *database,
                                              char              *name,
                                              char              *new_r_perm,
                                              char              *new_w_perm);

/**
 * Remove an user
 *
 * \param client A initialized client
 * \param name The user name to remove
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_delete_database_user(s_influxdb_client *client,
                                  char              *database,
                                  char              *name);

/**
 * List existing users on database
 *
 * \param client A initialized client
 * \param database The database name on which extract users
 * \param users_list The list to fill with database users
 * \return The number of users
 */
size_t influxdb_get_database_user_list(s_influxdb_client *client,
                                       char              *database,
                                       char              ***users_list);

/**
 * Update user permissions on database
 *
 * \param client A initialized client
 * \param database The database name on which permission will apply
 * \param name The user name to update
 * \param is_admin Is the user admin of the database?
 * \param new_r_perm Optional read permissions
 * \param new_w_perm Optional write permissions
 * \return HTTP status code or CURLcode (if < 100)
 */
int influxdb_alter_database_privilege(s_influxdb_client *client,
                                      char              *database,
                                      char              *name,
                                      char              isAdmin,
                                      char              *new_r_perm,
                                      char              *new_w_perm);

#endif /* !DATABASE_USER_H_ */
