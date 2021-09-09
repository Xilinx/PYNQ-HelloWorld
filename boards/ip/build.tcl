############################################################
## Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
############################################################
open_project resize
set_top resize_accel
add_files src/xf_resize_accel.cpp -cflags "-I./vitis_lib/vision/L1/include -I./vitis_lib/vision/L2/examples/resize -I./vitis_lib/vision/L2/tests/resize/resize_DOWN_BILINEAR_NO_RGB -D__SDSVHLS__"
open_solution "resizer"
set_part {zynq}
create_clock -period 10 -name default
set_clock_uncertainty 27.0%
csynth_design
export_design -format ip_catalog -description "Image resizing IP" -display_name "resize_accel"
exit

