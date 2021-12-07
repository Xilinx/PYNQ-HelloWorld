# Copyright (C) 2021 Xilinx, Inc
#
# SPDX-License-Identifier: BSD-3-Clause

IP := resize

VITIS_LIB_INCLUDE = $(shell readlink -f ./vitis_lib/vision/L1/include/)

all: hls
	@echo "IP built successfully."

hls:
	vitis_hls -f build.tcl -tclargs $(VITIS_LIB_INCLUDE)
	sed -i 's/<xilinx:family xilinx:lifeCycle="Pre-Production">zynq<\/xilinx:family>/<xilinx:family xilinx:lifeCycle="Pre-Production">zynq<\/xilinx:family><xilinx:family xilinx:lifeCycle="Pre-Production">zynquplus<\/xilinx:family><xilinx:family xilinx:lifeCycle="Pre-Production">zynquplusRFSOC<\/xilinx:family>/g' $(IP)/$(IP)/impl/ip/component.xml


clean:
	rm -rf $(IP) *.log

