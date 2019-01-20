#!/usr/bin/env bash

bash setup_test_env.sh
source env_vars.sh

PATH="$BIN_PATH:$PATH"
cd "$TEST_FILES_PATH" || exit 1

PASSED_COUNTER=0
FAILED_COUNTER=0

echo "Testing mkfile empty file inside /"

init -f test.mdofs >/dev/null 2>&1


if mkfile -f test.mdofs "/file1" >/dev/null 2>&1;
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

echo "Testing if 3 records in / after mkfile empty file"

mapfile -t RECORDS < <(ls -f test.mdofs "/")

if [[ ${#RECORDS[@]} -eq 3 ]]
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

echo "Testing if file1 exists in /"

if [[ "${RECORDS[@]}" =~ "file1" ]]
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

#for NAME in ${RECORDS[@]} ; do
#    echo ${NAME}
#done

/usr/bin/rm -f test.mdofs

#################################################################

echo "Testing mkfile empty file inside subdirectory"

init -f test.mdofs >/dev/null 2>&1

mkdir -f test.mdofs "/dir1" >/dev/null 2>&1

if mkfile -f test.mdofs "/dir1/file1" >/dev/null 2>&1;
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

echo "Testing if 3 records in /dir1 after mkfile empty file"

mapfile -t RECORDS < <(ls -f test.mdofs "/dir1")

if [[ ${#RECORDS[@]} -eq 3 ]]
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

echo "Testing if file1 exists in /dir1"

if [[ "${RECORDS[@]}" =~ "file1" ]]
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

#for NAME in ${RECORDS[@]} ; do
#    echo ${NAME}
#done

/usr/bin/rm -f test.mdofs

#########################################################################

echo "Testing mkfile 2 empty files inside subdirectory"

init -f test.mdofs >/dev/null 2>&1

mkdir -f test.mdofs "/dir1" >/dev/null 2>&1

mkfile -f test.mdofs "/dir1/file1" >/dev/null 2>&1

if mkfile -f test.mdofs "/dir1/file2" >/dev/null 2>&1;
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

echo "Testing if 4 records in /dir1 after mkfile 2 empty files"

mapfile -t RECORDS < <(ls -f test.mdofs "/dir1")

if [[ ${#RECORDS[@]} -eq 4 ]]
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

echo "Testing if file2 exists in /dir1"

if [[ "${RECORDS[@]}" =~ "file1" ]]
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

#for NAME in ${RECORDS[@]} ; do
#    echo ${NAME}
#done

echo "Testing after adding another directory"

mkdir -f test.mdofs "/dir1/dir2" >/dev/null 2>&1

echo "Testing if 5 records in /dir1 after mkfile 2 empty files and directory"

mapfile -t RECORDS < <(ls -f test.mdofs "/dir1")

if [[ ${#RECORDS[@]} -eq 5 ]]
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

#for NAME in ${RECORDS[@]} ; do
#    echo ${NAME}
#done

/usr/bin/rm -f test.mdofs

###########################################################

echo "Testing mkfile file with data inside /"

init -f test.mdofs >/dev/null 2>&1


if mkfile -f test.mdofs "/file1" -i <<< "TEST" >/dev/null 2>&1;
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

echo "Testing if 3 records in / after mkfile file with data"

mapfile -t RECORDS < <(ls -f test.mdofs "/")

if [[ ${#RECORDS[@]} -eq 3 ]]
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

echo "Testing if file with data exists in /"

if [[ "${RECORDS[@]}" =~ "file1" ]]
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

#for NAME in ${RECORDS[@]} ; do
#    echo ${NAME}
#done

echo "Testing file content"

CONTENT=$(cat -f test.mdofs "/file1")

if [[ ${CONTENT} = "TEST" ]]
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

echo "Testing file size"

SIZE_OUTPUT=$(fsize -f test.mdofs "/file1")
PREFIX="File: \".*\" Size: "

SIZE=$(echo ${SIZE_OUTPUT} | sed -e 's/File: \".*\" Size: //g')

if [[ ${SIZE} = "5" ]]
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

/usr/bin/rm -f test.mdofs

##################################################################

init -f test.mdofs >/dev/null 2>&1

echo "Testing cat on nonempty file created with option -i specified with mkfile and TEST string passed"

mkfile -f test.mdofs "/file1" -i <<< "TEST" >/dev/null 2>&1;

CONTENT=$(cat -f test.mdofs "/file1")

if [[ ${CONTENT} = "TEST" ]]
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

echo "Testing fsize on nonempty file created with option -i specified with mkfile and TEST string passed"

SIZE_OUTPUT=$(fsize -f test.mdofs "/file1")
PREFIX="File: \".*\" Size: "

SIZE=$(echo ${SIZE_OUTPUT} | sed -e 's/File: \".*\" Size: //g')

if [[ ${SIZE} = "5" ]]
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

echo "Testing cat on empty file created with option -i specified with mkfile and empty string passed"

mkfile -f test.mdofs "/file2" -i <<< "" >/dev/null 2>&1;

CONTENT=$(cat -f test.mdofs "/file2")

if [[ ${CONTENT} = "" ]]
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

echo "Testing fsize on empty file created with option -i specified with mkfile and empty string passed"

SIZE_OUTPUT=$(fsize -f test.mdofs "/file2")
PREFIX="File: \".*\" Size: "

SIZE=$(echo ${SIZE_OUTPUT} | sed -e 's/File: \".*\" Size: //g')

if [[ ${SIZE} = "1" ]]
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

echo "Testing cat on empty file created with no option specified with mkfile"

mkfile -f test.mdofs "/file3" >/dev/null 2>&1;

CONTENT=$(cat -f test.mdofs "/file3")

if [[ ${CONTENT} = "" ]]
then
    echo "Passed"
    PASSED_COUNTER=$((PASSED_COUNTER+1))
else
    echo "Failed"
    FAILED_COUNTER=$((FAILED_COUNTER+1))
fi

echo "Testing fsize on empty file created with no option specified with mkfile"

SIZE_OUTPUT=$(fsize -f test.mdofs "/file3")
PREFIX="File: \".*\" Size: "

SIZE=$(echo ${SIZE_OUTPUT} | sed -e 's/File: \".*\" Size: //g')

if [[ ${SIZE} = "0" ]]
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
echo ""
echo "SUMMARY: "
echo "Total:" $((PASSED_COUNTER+FAILED_COUNTER))
echo "Passed:" ${PASSED_COUNTER}
echo "Failed:" ${FAILED_COUNTER}