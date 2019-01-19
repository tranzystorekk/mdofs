#!/usr/bin/env bash

PROJECT_ROOT="$(git rev-parse --show-toplevel)"
BIN_PATH="$PROJECT_ROOT/utils/manual-tests/bin"
TEST_FILES_PATH="$PROJECT_ROOT/utils/manual-tests/test-files"
PATH="$BIN_PATH:$PATH"
