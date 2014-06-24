/*
 * Copyright (c) 2013-2014 InfluxDB
 * Pierre-Olivier Mercier <pomercier@novaquark.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */

#include <stdio.h>

#include "tests.h"

int
main()
{
    size_t i, fails = 0;

    s_influxdb_client *client = influxdb_client_new("localhost:8086",
                                                    "root",
                                                    "root",
                                                    INFLUXDB_DATABASE_TEST,
                                                    0);

    if (client == NULL)
    {
        printf("Unable to initialize client\n");
        return 127;
    }

    for (i = 0; tests[i]; i++)
    {
        if (tests[i](client))
        {
            printf("Test %d fails.\n", i + 1);
            fails++;
        }
    }

    influxdb_client_free(client);

    printf("SUMARRY: %d tests executed, %d passed, %d failed",
           i, i - fails, fails);

    return fails > 126 ? 126 : fails;
}
