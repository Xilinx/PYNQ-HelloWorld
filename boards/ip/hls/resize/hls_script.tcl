############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
############################################################
open_project hls_proj
set_top resize_accel
add_files xf_resize_accel.cpp -cflags "-I../include -std=c++0x"
add_files xf_resize_config.h
add_files xf_config_params.h

add_files -tb xf_headers.h
add_files -tb xf_resize_tb.cpp -cflags "-I../include -std=c++0x"
add_files -tb /proj/xsjhdstaff4/varuns/DATA/img_l.png

open_solution "solution1"
set_part {xc7z020clg484-1} -tool vivado
create_clock -period 10 -name default
set_clock_uncertainty 27.0%

csim_design -argv {img_l.png} -clean -compiler gcc
csynth_design
cosim_design -argv {img_l.png}
export_design -format ip_catalog
