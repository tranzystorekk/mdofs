#!/usr/bin/env bash

PROJECT_ROOT="$(git rev-parse --show-toplevel)"
TEST_FILES_PATH="$PROJECT_ROOT/utils/manual-tests/test-files"

BUILD_PATH="$PROJECT_ROOT/cmake-build-debug"

rm -f "${TEST_FILES_PATH}"/*

cp "$BUILD_PATH/utils/init/app/init" "$TEST_FILES_PATH"
cp "$BUILD_PATH/utils/mkdir/app/mkdir" "$TEST_FILES_PATH"
cp "$BUILD_PATH/utils/ls/app/ls" "$TEST_FILES_PATH"
#cp "$BUILD_PATH/utils/cat/app/cat" "$TEST_FILES_PATH"
#cp "$BUILD_PATH/utils/mkfile/app/mkfile" "$TEST_FILES_PATH"
#cp "$BUILD_PATH/utils/rm/app/rm" "$TEST_FILES_PATH"
