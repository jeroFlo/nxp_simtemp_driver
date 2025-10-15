 #! /bin/bash

 # - builds the kernel module 
 
 makefile_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )/../kernel

 if [ -n "$1" ]; then # check for a user input 
    if [[ "$1" -eq "-c" ]]; then # option to make a clean
       cd $makefile_path
       echo "*** Cleaning ***"
       make clean -C $makefile_path 
       exit 0
    else # asumming a existing path to a makefile
      makefile_path=$1
      echo "*** Overriding Makefile path by the user input ***"
    fi
    
 fi

 cd $makefile_path

 make -C $makefile_path 


 # - user app
