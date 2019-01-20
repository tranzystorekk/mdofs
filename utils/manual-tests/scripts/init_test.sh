#!/usr/bin/env bash

bash setup_test_env.sh
source env_vars.sh

PASSED_COUNTER=0
FAILED_COUNTER=0

cd "$TEST_FILES_PATH"

echo "Testing making an instance of file system"

init -f test.mdofs >/dev/null 2>&1

if [[ -f test.mdofs ]]
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

/usr/bin/rm -f test.mdofs

echo "Testing default size bigger than 1MB"

init -f test.mdofs >/dev/null 2>&1

BYTES=$(wc -c test.mdofs | cut -f1 -d ' ')

if [[ $BYTES -gt 1048576 ]]
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

/usr/bin/rm -f test.mdofs

echo "Testing multiple instances of the same name"

init -f test.mdofs >/dev/null 2>&1

if init -f test.mdofs >/dev/null 2>&1;
then
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
else
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
fi

/usr/bin/rm -f test.mdofs

echo "Testing size > 4MB"

init --size 4M -f test4mb.mdofs >/dev/null 2>&1

BYTES=$(wc -c test4mb.mdofs | cut -f1 -d ' ')

if [[ $BYTES -gt 4194304 ]]
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

/usr/bin/rm -f test4mb.mdofs

echo "Testing size = 0"

if init --size 0 -f test0.mdofs >/dev/null 2>&1;
then
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
else
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
fi

echo "Testing size bigger than 4GB"

if init --size 5G -f test5GB.mdofs >/dev/null 2>&1;
then
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
else
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
fi

#######################################################################

# SUMMARY
echo ""
echo "SUMMARY: "
echo "Total:" $((PASSED_COUNTER+FAILED_COUNTER))
echo "Passed:" ${PASSED_COUNTER}
echo "Failed:" ${FAILED_COUNTER}
