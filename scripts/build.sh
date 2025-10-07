 #! /bin/bash

 # - builds the kernel module 
 
 path=$PWD/../kernel # asumming user will run it from its root

 if [ -n "$1" ]; then # check for a user input 
    path=$1
 fi

 make -C /lib/modules/`uname -r`/build M=$path modules

 # - user app
