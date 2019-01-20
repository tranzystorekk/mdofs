#!/usr/bin/env bash

bash setup_test_env.sh
source env_vars.sh

PATH="$BIN_PATH:$PATH"
cd "$TEST_FILES_PATH" || exit 1

NUM_LS_INVOCATIONS=10000



function concurrent_ls() {
    for i in $(seq "${NUM_LS_INVOCATIONS}"); do
        ls -f "test.mdofs" "/" >/dev/null &
        pids+=("$!")
    done

    wait
}

echo "Testing concurrent ls and mkdir"

init -f test.mdofs >/dev/null 2>&1

concurrent_ls &


sleep 1
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
    WAS_SUCCESS=0
    WAS_FAIL=0

    pids=()
    for i in $(seq "${NUM_LS_INVOCATIONS}"); do
        cat -f "test.mdofs" "/file1" >/dev/null &
        pids+=("$!")
    done

    for p in ${pids[@]}; do
            if wait "$p"; then
#                    echo "Process $p success"
                    WAS_SUCCESS=1
            else
#                    echo "Process $p fail"
                    WAS_FAIL=1
            fi
    done

    if [[ $WAS_SUCCESS -eq 1 ]] && [[ $WAS_FAIL -eq 1 ]]
    then
        echo "Operation occured during other: TEST OK"
    else
        echo "Operation didn't occur during other: TEST UNDEFINED"
    fi
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

#######################################################################

function concurrent_cat() {
    WAS_SUCCESS=0
    WAS_FAIL=0

    pids=()
    for i in $(seq "${NUM_LS_INVOCATIONS}"); do
        cat -f "test.mdofs" "/file1" >/dev/null &
        pids+=("$!")
    done

    for p in ${pids[@]}; do
            if wait "$p"; then
#                    echo "Process $p success"
                    WAS_SUCCESS=1
            else
#                    echo "Process $p fail"
                    WAS_FAIL=1
            fi
    done

    if [[ $WAS_SUCCESS -eq 1 ]] && [[ $WAS_FAIL -eq 1 ]]
    then
        echo "Operation occured during other: TEST OK"
    else
        echo "Operation didn't occur during other: TEST UNDEFINED"
    fi
}

echo "Testing concurrent cat and mkfile"

init -f test.mdofs >/dev/null 2>&1

concurrent_cat &

sleep 1
mkfile -f test.mdofs "/file1" >/dev/null 2>&1
MKFILE_PID="$!"

wait "$MKFILE_PID"
MKFILE_EXIT_CODE="$?"

wait

if [[ ${MKFILE_EXIT_CODE} -eq 0 ]]
then
    echo "Passed"
else
    echo "Failed"
fi

/usr/bin/rm -f test.mdofs

################################################################




