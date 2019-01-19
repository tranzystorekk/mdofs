#!/usr/bin/env bash

bash setup_test_env.sh
source env_vars.sh

PATH="$BIN_PATH:$PATH"
cd "$TEST_FILES_PATH" || exit 1

echo "Testing ls on /"

init -f test.mdofs >/dev/null 2>&1

mapfile -t RECORDS < <(ls -f test.mdofs "/")

if [[ ${#RECORDS[@]} -eq 2 ]]
then
    echo "Passed"
else
    echo "Failed"
fi

echo "Testing / contains . and .."

if [[ "${RECORDS[@]}" =~ "." && "${RECORDS[@]}" =~ ".." ]]
then
    echo "Passed"
else
    echo "Failed"
fi

echo "Testing invalid path to folder"

if ls -f test.mdofs "" >/dev/null 2>&1;
then
    echo "Failed"
else
    echo "Passed"
fi

echo "Testing mkdir folder"

if mkdir -f test.mdofs "/folder" >/dev/null 2>&1;
then
    echo "Passed"
else
    echo "Failed"
fi

rm -f test.mdofs

echo "Testing mkdir /"

init -f test.mdofs >/dev/null 2>&1
if mkdir -f test.mdofs "/" >/dev/null 2>&1;
then
    echo "Failed"
else
    echo "Passed"
fi

echo "Testing mkdir /."

if mkdir -f test.mdofs "/." >/dev/null 2>&1;
then
    echo "Failed"
else
    echo "Passed"
fi

echo "Testing mkdir /.."

if mkdir -f test.mdofs "/.." >/dev/null 2>&1;
then
    echo "Failed"
else
    echo "Passed"
fi

mapfile -t RECORDS < <(ls -f test.mdofs "/")

for NAME in "${RECORDS[@]}" ; do
    echo "$NAME"
done


rm -f test.mdofs

echo "Testing after adding new folder, 3 records in root"

init -f test.mdofs >/dev/null 2>&1
mkdir -f test.mdofs "/folder" >/dev/null 2>&1

mapfile -t RECORDS < <(ls -f test.mdofs "/")

if [[ ${#RECORDS[@]} -eq 3 ]]
then
    echo "Passed"
else
    echo "Failed"
fi

echo "Testing if folder is in root directory"

if [[ "${RECORDS[@]}" =~ "folder" ]]
then
    echo "Passed"
else
    echo "Failed"
fi

echo "Testing if in folder there are only two records"

mapfile -t RECORDS_FOLDER < <(ls -f test.mdofs "/folder")

if [[ ${#RECORDS_FOLDER[@]} -eq 2 ]]
then
    echo "Passed"
else
    echo "Failed"
fi

echo "Testing if in folder there is . and .."


if [[ "${RECORDS_FOLDER[@]}" =~ "." && "${RECORDS_FOLDER[@]}" =~ ".." ]]
then
    echo "Passed"
else
    echo "Failed"
fi

rm -f test.mdofs

########################################################

echo "Testing dir2 inside dir1 - three records in dir1"

init -f test.mdofs >/dev/null 2>&1
mkdir -f test.mdofs "/dir1" >/dev/null 2>&1
mkdir -f test.mdofs "/dir1/dir2" >/dev/null 2>&1

mapfile -t RECORDS < <(ls -f test.mdofs "/")

echo "Testing if in / there are still 3 directories"

if [[ ${#RECORDS[@]} -eq 3 ]]
then
    echo "Passed"
else
    echo "Failed"
fi

echo "Testing if in / there is not dir2"

if [[ ! "${RECORDS[@]}" =~ "dir2" ]]
then
    echo "Passed"
else
    echo "Failed"
fi

mapfile -t RECORDS_DIR1 < <(ls -f test.mdofs "/dir1")

if [[ ${#RECORDS_DIR1[@]} -eq 3 ]]
then
    echo "Passed"
else
    echo "Failed"
fi

echo "Testing if in dir1 there is dir2"

if [[ "${RECORDS_DIR1[@]}" =~ "dir2" ]]
then
    echo "Passed"
else
    echo "Failed"
fi

mapfile -t RECORDS_DIR2 < <(ls -f test.mdofs "/dir1/dir2")

echo "Testing dir2 - two records in dir2"

if [[ ${#RECORDS_DIR2[@]} -eq 2 ]]
then
    echo "Passed"
else
    echo "Failed"
fi

echo "Testing if in dir2 there is . and .."

if [[ "${RECORDS_DIR2[@]}" =~ "." && "${RECORDS_DIR2[@]}" =~ ".." ]]
then
    echo "Passed"
else
    echo "Failed"
fi

rm -f test.mdofs

#########################################################

echo "Testing if .. inside path works"

init -f test.mdofs >/dev/null 2>&1
mkdir -f test.mdofs "/dir1" >/dev/null 2>&1
mkdir -f test.mdofs "/dir1/dir2" >/dev/null 2>&1
mkdir -f test.mdofs "/dir1/dir2/dir3" >/dev/null 2>&1

mapfile -t RECORDS < <(ls -f test.mdofs "/dir1/dir2/../dir2")

if [[ "${RECORDS[@]}" =~ "dir3" ]]
then
    echo "Passed"
else
    echo "Failed"
fi

echo "Testing if . inside path works"

mapfile -t RECORDS < <(ls -f test.mdofs "/dir1/./dir2")

if [[ "${RECORDS[@]}" =~ "dir3" ]]
then
    echo "Passed"
else
    echo "Failed"
fi

rm -f test.mdofs