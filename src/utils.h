/*
 * Copyright (c) 2013-2014 InfluxDB
 * Pierre-Olivier Mercier <pomercier@novaquark.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#ifndef UTILS_H_
# define UTILS_H_

/**
 * Returns a pointer to a new string which is a duplicate of the string s
 */
char *influxdb_strdup(const char *s);

#endif /* !UTILS_H_ */
