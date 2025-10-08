 #! /bin/bash

 # - builds the kernel module 
 
 makefile_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )/../kernel

 if [ -n "$1" ]; then # check for a user input 
    makefile_path=$1
    echo "*** Overriding Makefile path by the user input ***"
 fi

 make -C /lib/modules/`uname -r`/build M=$makefile_path modules


 # - user app
