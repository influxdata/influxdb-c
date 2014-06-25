/*
 * Copyright (c) 2013-2014 InfluxDB
 * Pierre-Olivier Mercier <pomercier@novaquark.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#ifndef TESTS_H_
# define TESTS_H_

# include <string.h>

# include "../src/client.h"

# define INFLUXDB_DATABASE_TEST "tests"
# define INFLUXDB_CLUSTERADMIN_TEST "toto"

typedef int (*influxdb_test)(s_influxdb_client *);

extern influxdb_test tests[];

#endif /* !TESTS_H_ */
