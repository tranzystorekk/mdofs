#!/usr/bin/env bash

containsElement () {
  local e match="$1"
  shift
  for e; do [[ "$e" == "$match" ]] && return 0; done
  return 1
}

PROJECT_ROOT="$(git rev-parse --show-toplevel)"
TEST_FILES_PATH="$PROJECT_ROOT/utils/manual-tests/test-files"

PATH="$TEST_FILES_PATH:$PATH"
cd "$TEST_FILES_PATH"

echo "Testing ls on /"

init -f test.mdofs >/dev/null 2>&1

RECORDS=($(ls -f test.mdofs "/"))

if [[ ${#RECORDS[@]} -eq 2 ]]
then
    echo "Passed"
else
    echo "Failed"
fi

echo "Testing / contains . and .."

if [[ $(containsElement '.' "${RECORDS[@]}") -eq 0 && $(containsElement '..' "${RECORDS[@]}") -eq 0 ]]
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

RECORDS=($(ls -f test.mdofs "/"))

for NAME in ${RECORDS[@]} ; do
    echo ${NAME}
done


rm -f test.mdofs

echo "Testing after adding new folder, 3 records in root"

init -f test.mdofs >/dev/null 2>&1
mkdir -f test.mdofs "/folder" >/dev/null 2>&1

RECORDS=($(ls -f test.mdofs "/"))

if [[ ${#RECORDS[@]} -eq 3 ]]
then
    echo "Passed"
else
    echo "Failed"
fi

echo "Testing if folder is in root directory"

if [[ $(containsElement "folder" "${RECORDS[@]}") -eq 0 ]]
then
    echo "Passed"
else
    echo "Failed"
fi

echo "Testing if in folder there are only two records"

RECORDS_FOLDER=($(ls -f test.mdofs "/folder"))

if [[ ${#RECORDS_FOLDER[@]} -eq 2 ]]
then
    echo "Passed"
else
    echo "Failed"
fi

echo "Testing if in folder there is . and .."


if [[ $(containsElement '.' "${RECORDS_FOLDER[@]}") -eq 0 && $(containsElement '..' "${RECORDS_FOLDER[@]}") -eq 0 ]]
then
    echo "Passed"
else
    echo "Failed"
fi

rm -f test.mdofs

#echo "Testing dir2 inside dir1"
#init -f test.mdofs >/dev/null 2>&1
#mkdir -f test.mdofs "/dir1/"
#mkdir -f test.mdofs "/dir1/dir2"

#RECORDS_FOLDER=($(ls -f test.mdofs "/))