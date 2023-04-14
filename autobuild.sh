#########################################################################
# File Name: autobuild.sh
# Author: Yuhao Jiang
# mail: 791331908@qq.com
# Created Time: 2023年04月14日 16:48:47
#########################################################################
#!/bin/bash
set -x
rm -rf `pwd`/build/*
cd `pwd`/build &&
    cmake .. &&
    make
