# HLS function for resize

This folder contains the source code for resize function in HLS. This function is a subset of pubicly available xfOpenCV library from Xilinx.

There are two folders in this directory:
include - This directory consists of files pertaining to the library infrastructure and source with core functionality
resize - This folder consists of C-testbench for unit test and a tcl script to run the resize kernel through C-simulation, C-synthesis, C/RTL cosimulation, and exporting the RTL as an IP. 

To test the functionality, please run the following:

```
$ vivado_hls resize/hls_script.tcl
```


The HLS top function is present in xf_resize_accel.cpp. 



## Licenses

**xfOpenCV** License : [BSD 3-Clause License](https://github.com/Xilinx/xfopencv/blob/master/LICENSE.txt)
