#!/usr/bin/env bash

source env_vars.sh

BUILD_PATH="$PROJECT_ROOT/cmake-build-debug"

cp -u "$BUILD_PATH/utils/init/app/init" "$BIN_PATH"
cp -u "$BUILD_PATH/utils/mkdir/app/mkdir" "$BIN_PATH"
cp -u "$BUILD_PATH/utils/ls/app/ls" "$BIN_PATH"
#cp -u "$BUILD_PATH/utils/cat/app/cat" "$BIN_PATH"
cp -u "$BUILD_PATH/utils/mkfile/app/mkfile" "$BIN_PATH"
#cp -u "$BUILD_PATH/utils/rm/app/rm" "$BIN_PATH"
