
## Prompt 1


>You are an expert in Linux Software development who will create a Makefile to build a module. The output files needs to be store in a Kbuild folder that the Makefile needs to take care.

>source file name is nxp_simtemp

**Validation Comments:** Result was pasted into `kernel/Makefile.3` unfortunaly it was given the error showed below. **TODO** REVISITING THIS LATER TO MAKE IT WORK PROPERLY. Meanwhile `Makefile.1` works. 

**CLI Execution output:**
```
root@jenifer-Ubuntu:/home/jenifer/Documents/simtemp/kernel# ../scripts/build.sh 
make: Entering directory '/home/jenifer/Documents/simtemp/kernel'
make -C /lib/modules/`uname -r`/build M=/home/jenifer/Documents/simtemp/kernel O=/home/jenifer/Documents/simtemp/kernel/Kbuild modules
make[1]: Entering directory '/usr/src/linux-headers-6.14.0-32-generic'
make[2]: Entering directory '/home/jenifer/Documents/simtemp/kernel'
***
***  ERROR: Kernel configuration is invalid. The following files are missing:
***    - /home/jenifer/Documents/simtemp/kernel/Kbuild/include/generated/autoconf.h
***    - /home/jenifer/Documents/simtemp/kernel/Kbuild/include/generated/rustc_cfg
***    - /home/jenifer/Documents/simtemp/kernel/Kbuild/include/config/auto.conf
***  Run "make oldconfig && make prepare" on kernel source to fix it.
***
/usr/src/linux-headers-6.14.0-32-generic/Makefile:780: /home/jenifer/Documents/simtemp/kernel/Kbuild/include/config/auto.conf: No such file or directory
make[3]: *** [/usr/src/linux-headers-6.14.0-32-generic/Makefile:855: /home/jenifer/Documents/simtemp/kernel/Kbuild/include/config/auto.conf] Error 1
make[2]: *** [/usr/src/linux-headers-6.14.0-32-generic/Makefile:251: __sub-make] Error 2
make[2]: Leaving directory '/home/jenifer/Documents/simtemp/kernel'
make[1]: *** [Makefile:251: __sub-make] Error 2
make[1]: Leaving directory '/usr/src/linux-headers-6.14.0-32-generic'
make: *** [Makefile:22: /home/jenifer/Documents/simtemp/kernel/Kbuild/nxp_simtemp.ko] Error 2
make: Leaving directory '/home/jenifer/Documents/simtemp/kernel'
```