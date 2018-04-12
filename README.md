quasardb benchmarking tool
--------------------------

`qdb-benchmark` is a performance testing tool for quasardb.
However it's been designed to be independent of quasardb and can be used to benchmark other engines.

`qdb-benchmark` contains a pool of predefined tests. It prepares a set of tests in various configuration and runs them one after the other.

A "test" is a code that performs a basic operation running in a loop.
When a test requires some kind of preparation before running, it's done in its "setup" phase.
Similarly, when a test requires some home keeping, it's done in its "cleanup" phase.

Tests leaves the database in the same state as before: unique keys are used that should not conflict with existing data. Added entries are removed at the end of each test.
Altough it's not recommended, `qdb-benchmark` can be used on a production database.

Depending on the options provided on the command line, `qdb-benchmark` will run one or more tests. Each test is run against one or more threading configuration. Each test is run against one or more input size.

`qdb-benchmark` samples the number of iterations at a regular pace, the resulting data is saved in a `results.jsonp` in the current folder. When running quasardb tests, it also samples information from the nodes, like the memory load for example.

There is an HTML report template that can be used to view the tests results in a web browser.
To use it, just copy `results.jsonp` in the `html_report/` folder and open `index.html` in your browser.

# Usage

## Basic usage

    qdb-benchmark -c qdb://127.0.0.1:2836 --tests qdb_blob_put,qdb_blob_get --threads 1,2,4 --sizes 1K,1M

This will run 12 tests:

1. `qdb_blob_put`, with a content of 1KB, running on 1 thread
2. `qdb_blob_put`, with a content of 1KB, running on 2 threads
3. `qdb_blob_put`, with a content of 1KB, running on 4 threads
4. `qdb_blob_put`, with a content of 1MB, running on 1 thread
5. `qdb_blob_put`, with a content of 1MB, running on 2 threads
6. `qdb_blob_put`, with a content of 1MB, running on 4 threads
7. `qdb_blob_get`, with a content of 1KB, running on 1 thread
8. `qdb_blob_get`, with a content of 1KB, running on 2 threads
9. `qdb_blob_get`, with a content of 1KB, running on 4 threads
10. `qdb_blob_get`, with a content of 1MB, running on 1 thread
11. `qdb_blob_get`, with a content of 1MB, running on 2 threads
12. `qdb_blob_get`, with a content of 1MB, running on 4 threads

Each test will run during approximately 4 seconds, and there will be a pause of 1 second between each test.

## Options

#### `--cluster` or `-c`

Sets the cluster's address.

When testing a quasardb cluster, the value must have the form `qdb://ip:port`.

The default value is `qdb://127.0.0.1:2836`.

#### `--pause` or `-p`

Sets the delay between each test, in seconds.

The default value is `1` second.

#### `--duration` or `-d`

Sets the duration of each test, in seconds.

Tests are not bound to a number of iteration, instead they are executed until this timeout is reached.
Some tests have a long running setup; in that case the timeout is used to limit the duration of the setup, and the test is executed according to the number of iteration prepared during the setup.

The default value is `4` seconds.

#### `--threads` or `-t`

Sets the number of threads to run each test.

The default is `1,2,4`, therefore running each test 3 times: a first time with 1 thread, then with 2 and a last time with 4 threads.

#### `--sizes`

Sets the contents sizes for each test.

Most tests (for example `qdb_blob_put`) have different results depending on the size of the input. Some tests, like ``qdb_int_add`, don't depend on the size.

The default value is `1,1K,1M`, therefore running each test 3 times: a first time with an input of 1 byte, then with 1 kilobyte and a last time with 1 megabyte.

#### `--iterations` or `-i`

Sets the number of iterations done for each test.

It silences the (default) duration option.

The default value is `0`, meaning it's not active and the tests will run until the specified duration has elapsed.

#### `--tests`

Sets the names of the tests to run. You can use `*` at the beginning or end of a test name to select multiple tests at once.

To know the list of supported tests, just run `qdb-benchmark --help` or `qdb-benchmark --show-tests`.

The current list of supported tests is:

1. qdb_blob_attach_tag
1. qdb_blob_get
1. qdb_blob_get_noalloc
1. qdb_blob_put
1. qdb_blob_remove
1. qdb_blob_update
1. qdb_deque_pop_back
1. qdb_deque_pop_front
1. qdb_deque_push_back
1. qdb_deque_push_front
1. qdb_int_add
1. qdb_int_get
1. qdb_int_put
1. qdb_int_remove
1. qdb_int_update
1. qdb_stream_write
1. std_atomic
1. std_fread
1. std_fwrite
1. std_mutex

The default is to run all tests, which can be quite long.
