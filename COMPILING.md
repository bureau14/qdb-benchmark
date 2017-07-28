Compiling qdb-benchmark
=======================

## Requirement:

* cmake
* a recent version of gcc or clang

## Download the source code

    cd $HOME
    git clone git@github.com:bureau14/qdb-benchmark.git
    
## Optional: download qdb API

Pick the desired version from https://download.quasardb.net/quasardb

    curl <url-of-qdb-api> | tar xv -C $HOME/qdb-benchmark/thirdparty/quasardb

## Optional: compile mongodb client API

    sudo apt-get install scons libboost-regex-dev libboost-system-dev libboost-thread-dev
    cd $HOME
    git clone https://github.com/mongodb/mongo-cxx-driver.git
    cd mongo-cxx-driver
    scons
    scons install --prefix=$HOME/qdb-benchmark/thirdparty/mongodb/

## Building

    git submodule update --init --recursive
    mkdir $HOME/qdb-benchmark/build
    cd $HOME/qdb-benchmark/build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    cmake --build .
