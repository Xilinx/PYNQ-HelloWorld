############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
############################################################
open_project resize
set_top resize_accel
add_files resize/xf_resize_accel.cpp -cflags "-Iresize/include"
add_files resize/xf_resize_config.h
add_files resize/xf_config_params.h

add_files -tb resize/xf_headers.h
add_files -tb resize/xf_resize_tb.cpp -cflags "-Iresize/include"

open_solution "solution1"
set_part {xc7z020clg484-1} -tool vivado
create_clock -period 10 -name default
set_clock_uncertainty 27.0%

csynth_design
export_design -format ip_catalog -description "Image resizer based on HLS" -display_name "Resizer"
exit
