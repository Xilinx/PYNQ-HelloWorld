############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
############################################################
open_project resize
set_top resize_accel

add_files ../vitis_lib/vision/L2/examples/resize/xf_resize_config.h
add_files ../vitis_lib/vision/L2/tests/resize/resize_DOWN_BILINEAR_NO_RGB/xf_config_params.h
add_files xf_axis_config.h
add_files xf_resize_accel.cpp \
-cflags "-I../vitis_lib/vision/L1/include \
	-I../vitis_lib/vision/L2/examples/resize \
	-I../vitis_lib/vision/L2/tests/resize/resize_DOWN_BILINEAR_NO_RGB \
	-D__SDSVHLS__ -std=c++0x"
add_files -tb xf_resize_tb.cpp \
-cflags "-I../vitis_lib/vision/L1/include \
	-I../vitis_lib/vision/L2/examples/resize \
	-I../vitis_lib/vision/L2/tests/resize/resize_DOWN_BILINEAR_NO_RGB \
	-D__SDSVHLS__ -std=c++0x"

open_solution "solution1"
set_part {zynq} -tool vivado
create_clock -period 10 -name default
set_clock_uncertainty 27.0%

csynth_design
export_design -format ip_catalog -description "Image resizing IP" -display_name "resize_accel"
exit
