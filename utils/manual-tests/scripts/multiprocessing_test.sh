#!/usr/bin/env bash

bash setup_test_env.sh
source env_vars.sh

PATH="$BIN_PATH:$PATH"
cd "$TEST_FILES_PATH" || exit 1

NUM_LS_INVOCATIONS=10000

function concurrent_ls() {
    for i in $(seq "${NUM_LS_INVOCATIONS}"); do
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

/usr/bin/rm -f test.mdofs

###################################################################

function concurrent_cat() {
    pids=()
    for i in $(seq "${NUM_LS_INVOCATIONS}"); do
        cat -f "test.mdofs" "/file1" >/dev/null &
        pids+=("$!")
    done

#    wait
    echo "${pids[*]}"
    for p in ${pids[@]}; do
            if wait "$p"; then
                    echo "Process $p success"
            else
                    echo "Process $p fail"
            fi
    done
}

echo "Testing concurrent cat and rm"

init -f test.mdofs >/dev/null 2>&1

mkfile -f test.mdofs "/file1" >/dev/null 2>&1

concurrent_cat &

sleep 1
rm -f test.mdofs "/file1" >/dev/null 2>&1 &
RM_PID="$!"

wait "$RM_PID"
RM_EXIT_CODE="$?"

wait

if [[ ${RM_EXIT_CODE} -eq 0 ]]
then
    echo "Passed"
else
    echo "Failed"
fi

/usr/bin/rm -f test.mdofs

