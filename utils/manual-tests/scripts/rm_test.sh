#!/usr/bin/env bash

bash setup_test_env.sh
source env_vars.sh

PATH="$BIN_PATH:$PATH"
cd "$TEST_FILES_PATH" || exit 1

PASSED_COUNTER=0
FAILED_COUNTER=0

#######################################################################

init -f test.mdofs >/dev/null 2>&1

echo "Testing rm on /"

if rm -f test.mdofs "/" >/dev/null 2>&1;
then
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
else
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
fi

echo "Testing rm on /."

if rm -f test.mdofs "/." >/dev/null 2>&1;
then
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
else
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
fi

echo "Testing rm on /.."

if rm -f test.mdofs "/.." >/dev/null 2>&1;
then
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
else
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
fi

echo "Testing rm on /dir1"

mkdir -f test.mdofs "/dir1" >/dev/null 2>&1;

if rm -f test.mdofs "/dir1" >/dev/null 2>&1;
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

echo "Testing if 2 directory entries after rm . and .."

mapfile -t RECORDS < <(ls -f test.mdofs "/")

if [[ ${#RECORDS[@]} -eq 2 ]]
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

echo "Testing if dir1 does not exist in /"

if [[ ! "${RECORDS[@]}" =~ "dir1" ]]
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

echo "Testing rm on /dir1/dir2"

mkdir -f test.mdofs "/dir1" >/dev/null 2>&1;
mkdir -f test.mdofs "/dir1/dir2" >/dev/null 2>&1;

if rm -f test.mdofs "/dir1/dir2" >/dev/null 2>&1;
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

echo "Testing rm on /dir1/."

if rm -f test.mdofs "/dir1/." >/dev/null 2>&1;
then
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
else
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
fi

/usr/bin/rm -f test.mdofs

init -f test.mdofs >/dev/null 2>&1

echo "Testing rm on /dir1 when it has entry created inside"

mkdir -f test.mdofs "/dir1" >/dev/null 2>&1;
mkdir -f test.mdofs "/dir1/dir2" >/dev/null 2>&1;

if rm -f test.mdofs "/dir1" >/dev/null 2>&1;
then
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
else
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
fi

/usr/bin/rm -f test.mdofs

init -f test.mdofs >/dev/null 2>&1

echo "Testing rm on /dir1 when it has entry created inside using .."

mkdir -f test.mdofs "/dir1" >/dev/null 2>&1;
mkdir -f test.mdofs "/dir1/dir2" >/dev/null 2>&1;

if rm -f test.mdofs "/dir1/dir2/.." >/dev/null 2>&1;
then
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
else
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
fi

/usr/bin/rm -f test.mdofs

init -f test.mdofs >/dev/null 2>&1

echo "Testing rm on file1"

mkfile -f test.mdofs "/file1" >/dev/null 2>&1;

if rm -f test.mdofs "/file1" >/dev/null 2>&1;
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

/usr/bin/rm -f test.mdofs

#######################################################################

# SUMMARY
echo
echo "SUMMARY:"
echo "Total: $((PASSED_COUNTER+FAILED_COUNTER))"
echo "Passed: ${PASSED_COUNTER}"
echo "Failed: ${FAILED_COUNTER}"
