#!/usr/bin/env bash

bash setup_test_env.sh
source env_vars.sh

PATH="$BIN_PATH:$PATH"
cd "$TEST_FILES_PATH" || exit 1

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
