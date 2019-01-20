#!/usr/bin/env bash

echo "INIT TEST"
bash init_test.sh
echo
echo "MKDIR LS TEST"
bash mkdir_ls_test.sh
echo
echo "MKFILE CAT FSIZE TEST"
bash mkfile_cat_fsize_test.sh
echo
echo "RM TEST"
bash rm_test.sh
echo
echo "MULTIPROCESSING TEST"
bash multiprocessing_test.sh