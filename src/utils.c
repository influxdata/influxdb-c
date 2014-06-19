/*
 * Copyright (c) 2013-2014 InfluxDB
 * Pierre-Olivier Mercier <pomercier@novaquark.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#include <stdlib.h>
#include <string.h>

#include "utils.h"

char
*influxdb_strdup(const char *s)
{
    if (s == NULL)
        return NULL;

    char *d = malloc(sizeof (char) * (strlen(s) + 1));
    if (d == NULL)
        return NULL;

    strcpy(d, s);
    return d;
}
