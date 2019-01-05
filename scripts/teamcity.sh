#!/bin/bash -eu
#
# Invocation ./teamcity.sh qdbd_args -- qdb-benchmark_args

set -eu

declare -a QDBD_ARGS
declare -a BENCH_ARGS

QDBD_ARGS+=("--security=false")
QDBD_ARGS+=("--log-flush-interval=1")
QDBD_ARGS+=("--store-history=false")

while [[ $# > 0 ]] ; do
  [ "$1" == "--" ] && break
  QDBD_ARGS+=("$1")
  shift
done

shift # it should be '--'

while [[ $# > 0 ]] ; do
  BENCH_ARGS+=("$1")
  shift
done

ROOT=$(dirname $0)/..

exec $ROOT/thirdparty/quasardb/bin/qdbd --storage-engine=rocksdb --rocksdb-root=$ROOT/db "${QDBD_ARGS[@]}" &
sleep 3
PID=$!

function kill_qdbd {
    kill $PID && wait $PID || true
    rm -rf $ROOT/db
}
trap kill_qdbd EXIT

if [[ "$OSTYPE" == "msys" ]]; then
    exec $ROOT/build/Release/qdb-benchmark "${BENCH_ARGS[@]}"
else
    exec $ROOT/build/qdb-benchmark "${BENCH_ARGS[@]}"
fi
