#!/usr/bin/env bash

PROJECT_ROOT="$(git rev-parse --show-toplevel)"
TEST_FILES_PATH="$PROJECT_ROOT/utils/manual-tests/test-files"

PATH="$TEST_FILES_PATH:$PATH"
cd "$TEST_FILES_PATH"



echo "Testing mkfile empty file inside /"

init -f test.mdofs >/dev/null 2>&1

if mkfile -f test.mdofs "/file1" >/dev/null 2>&1;
then
    echo "Passed"
else
    echo "Failed"
fi

echo "Testing if 3 records in / after mkfile empty file"

RECORDS=($(ls -f test.mdofs "/"))

if [[ ${#RECORDS[@]} -eq 3 ]]
then
    echo "Passed"
else
    echo "Failed"
fi

echo "Testing if file1 exists in /"

if [[ "${RECORDS[@]}" =~ "file1" ]]
then
    echo "Passed"
else
    echo "Failed"
fi

for NAME in ${RECORDS[@]} ; do
    echo ${NAME}
done

rm -f test.mdofs

#################################################################

echo "Testing mkfile empty file inside subdirectory"

init -f test.mdofs >/dev/null 2>&1

mkdir -f test.mdofs "/dir1" >/dev/null 2>&1

if mkfile -f test.mdofs "/dir1/file1" >/dev/null 2>&1;
then
    echo "Passed"
else
    echo "Failed"
fi

echo "Testing if 3 records in /dir1 after mkfile empty file"

RECORDS=($(ls -f test.mdofs "/dir1"))

if [[ ${#RECORDS[@]} -eq 3 ]]
then
    echo "Passed"
else
    echo "Failed"
fi

echo "Testing if file1 exists in /dir1"

if [[ "${RECORDS[@]}" =~ "file1" ]]
then
    echo "Passed"
else
    echo "Failed"
fi

for NAME in ${RECORDS[@]} ; do
    echo ${NAME}
done

rm -f test.mdofs

#########################################################################

echo "Testing mkfile 2 empty files inside subdirectory"

init -f test.mdofs >/dev/null 2>&1

mkdir -f test.mdofs "/dir1" >/dev/null 2>&1

mkfile -f test.mdofs "/dir1/file1" >/dev/null 2>&1

if mkfile -f test.mdofs "/dir1/file2" >/dev/null 2>&1;
then
    echo "Passed"
else
    echo "Failed"
fi

echo "Testing if 4 records in /dir1 after mkfile 2 empty files"

RECORDS=($(ls -f test.mdofs "/dir1"))

if [[ ${#RECORDS[@]} -eq 4 ]]
then
    echo "Passed"
else
    echo "Failed"
fi

echo "Testing if file2 exists in /dir1"

if [[ "${RECORDS[@]}" =~ "file1" ]]
then
    echo "Passed"
else
    echo "Failed"
fi

for NAME in ${RECORDS[@]} ; do
    echo ${NAME}
done

echo "Testing after adding another directory"

mkdir -f test.mdofs "/dir1/dir2" >/dev/null 2>&1

echo "Testing if 5 records in /dir1 after mkfile 2 empty files and directory"

RECORDS=($(ls -f test.mdofs "/dir1"))

if [[ ${#RECORDS[@]} -eq 5 ]]
then
    echo "Passed"
else
    echo "Failed"
fi

for NAME in ${RECORDS[@]} ; do
    echo ${NAME}
done

rm -f test.mdofs





