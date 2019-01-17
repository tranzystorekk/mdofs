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

echo "Test ls on /"

init -f test.mdofs >/dev/null 2>&1

RECORDS=($(ls -f test.mdofs "/"))

if [[ ${#RECORDS[@]} -eq 2 ]]
then
    echo "Passed"
else
    echo "Failed"
fi

echo "Test / contains . and .."

if [[ $(containsElement '.' "${RECORDS[@]}") -eq 0 && $(containsElement '..' "${RECORDS[@]}") -eq 0 ]]
then
    echo "Passed"
else
    echo "Failed"
fi

rm -f test.mdofs
