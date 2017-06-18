#! /bin/bash

set -e

make clean > log/make_clean_log.log
echo '1'
make all > log/make_all_log.log
echo '2'
sudo make install > log/make_install.log
echo '3'
