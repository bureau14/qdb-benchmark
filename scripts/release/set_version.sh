#!/bin/sh
set -eu -o pipefail
IFS=$'\n\t'

if [[ $# -ne 1 ]] ; then
    >&2 echo "Usage: $0 <new_version>"
    exit 1
fi

INPUT_VERSION=$1; shift

MAJOR_VERSION=${INPUT_VERSION%%.*}
WITHOUT_MAJOR_VERSION=${INPUT_VERSION#${MAJOR_VERSION}.}
MINOR_VERSION=${WITHOUT_MAJOR_VERSION%%.*}
WITHOUT_MINOR_VERSION=${INPUT_VERSION#${MAJOR_VERSION}.${MINOR_VERSION}.}
PATCH_VERSION=${WITHOUT_MINOR_VERSION%%.*}

XYZ_VERSION="${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}"

cd $(dirname -- $0)
cd ${PWD}/../..

# set(CPACK_PACKAGE_VERSION "2.0.2")
sed -i -e 's/set(CPACK_PACKAGE_VERSION *"[^"]*")/set(CPACK_PACKAGE_VERSION "'"${XYZ_VERSION}"'")/' CMakeLists.txt
