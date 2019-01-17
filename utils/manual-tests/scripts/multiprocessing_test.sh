#!/usr/bin/env bash

PROJECT_ROOT="$(git rev-parse --show-toplevel)"
TEST_FILES_PATH="$PROJECT_ROOT/utils/manual-tests/test-files"

PATH="$TEST_FILES_PATH:$PATH"
cd "$TEST_FILES_PATH"

function concurrent_ls() {
    for i in $(seq 10); do
    ls -f test.mdofs "/" >/dev/null &
    done
}

echo "Testing concurrent ls and mkdir"

init -f test.mdofs >/dev/null 2>&1
concurrent_ls &
if mkdir -f test.mdofs "/dir";
then
    echo "Passed"
else
    echo "Failed"
fi

ls -f test.mdofs "/"

rm -f test.mdofs