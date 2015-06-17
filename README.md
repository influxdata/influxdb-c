influxdb-c
==========

# This library is not updated for InfluxDB 0.9.0. There are breaking changes to the API, use at your own risk.

We encourage you to submit a pull request if you have a contribution.

----------

A C89 library to play with the REST API of an InfluxDB database.

This implementation is meant as a C rewrite of the influxdb-go package.

## Typical usages

### Create a new database

```c
#include <influxdb/influxdb.h>

int main() {
    int status;
    s_influxdb_client *client = influxdb_client_new("localhost:8086", "root", "root", "", 0);

    status = influxdb_create_database(client, "toto");

    influxdb_client_free(client);

    return status != 201;
}
```

```sh
gcc -I/usr/local/include/ -L/usr/local/lib -linfluxdb -o test1 test1.c
./test1
```

### List users of toto database

```c
#include <stdio.h>
#include <influxdb/influxdb.h>

int main() {
    s_influxdb_client *client = influxdb_client_new("localhost:8086", "root", "root", "", 0);
    char **users_list;
    size_t nb = influxdb_get_database_user_list(client, "toto", &users_list);
    size_t i;

    printf("There are %lu users:\n", nb);
    for (i = 0; i < nb; i++) {
        printf(" - %s\n", users_list[i]);
        free(users_list[i]);
    }

    free(users_list);

    return 0;
}
```

```sh
gcc -I/usr/local/include/ -L/usr/local/lib -linfluxdb -o test2 test2.c
./test2
```

### Send data

```c
#include <string.h>
#include <influxdb/influxdb.h>

int main() {
    s_influxdb_client *client = influxdb_client_new("localhost:8086", "root", "root", "toto", 0);
    int status;
    s_influxdb_series *series = influxdb_series_create("my_metrics", NULL);
    char **tab1 = malloc(sizeof (char *) * 4);
    char **tab2 = malloc(sizeof (char *) * 4);

    influxdb_series_add_colums(series, "col1");
    influxdb_series_add_colums(series, "col2");
    influxdb_series_add_colums(series, "col3");
    influxdb_series_add_colums(series, "col4");

    tab1[0] = strdup("4"); tab1[1] = strdup("1"); tab1[2] = strdup("3"); tab1[3] = strdup("2");
    influxdb_series_add_points(series, tab1);

    tab2[0] = strdup("0"); tab2[1] = strdup("2"); tab2[2] = strdup("1"); tab2[3] = strdup("3");
    influxdb_series_add_points(series, tab2);

    status = influxdb_write_serie(client, series);

    influxdb_series_free(series, NULL);

    return status != 200;
}
```

```sh
gcc -I/usr/local/include/ -L/usr/local/lib -linfluxdb -o test3 test3.c
./test3
```

## Build

Requirements:
* A compiler supporting C89 (tested with GCC 4.8.2 and Clang 3.3);
* Autotools & libtool;
* [libcurl](http://libcurl.org/);
* [libjson](https://github.com/json-c/json-c);

Steps to build and install the library:
```sh
./bootstrap
./configure
make
sudo make install
```

That will install the `libinfluxdb.so` into `/usr/local/lib/` and headers into `/usr/local/include/influxdb/`.

## Documentation

Generate documentation with:

```sh
make docs
```

Then, you will find HTML and LaTeX in `docs` directory.
