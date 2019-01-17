#!/usr/bin/env bash

PROJECT_ROOT="$(git rev-parse --show-toplevel)"
TEST_FILES_PATH="$PROJECT_ROOT/utils/manual-tests/test-files"

PATH="$TEST_FILES_PATH:$PATH"
cd "$TEST_FILES_PATH"

NUM_LS_INVOCATIONS=100

function concurrent_ls() {
    for i in "$(seq ${NUM_LS_INVOCATIONS})"; do
        ls -f "test.mdofs" "/" >/dev/null &
    done

    wait
}

echo "Testing concurrent ls and mkdir"

init -f test.mdofs >/dev/null 2>&1

concurrent_ls &
mkdir -f test.mdofs "/dir" >/dev/null 2>&1 &
MKDIR_PID="$!"

wait "$MKDIR_PID"
MKDIR_EXIT_CODE="$?"

wait

if [[ ${MKDIR_EXIT_CODE} -eq 0 ]]
then
    echo "Passed"
else
    echo "Failed"
fi

rm -f test.mdofs
