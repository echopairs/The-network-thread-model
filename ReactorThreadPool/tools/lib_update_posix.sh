#!/usr/bin/env bash


set -e
script=$(readlink -f "$0")
route=$(dirname "$script")

if [ -d "$route/../third_parties" ]; then
    sudo rm -r $route/../third_parties
fi

mkdir $route/../third_parties
cd $route/../third_parties

# with git
## 1. init google test
git clone https://github.com/google/googletest.git
cd googletest || echo { "cd googletest faile"}
mkdir build
cmake ../
make -j 4
sudo make install

