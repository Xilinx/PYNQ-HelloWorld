BOARD := U200

CUR_DIR = $(shell pwd)
VIVADO_ROOT := $(XILINX_VIVADO)

RM = rm -f
RMDIR = rm -rf

VIVADO := ${VIVADO_ROOT}/bin/vivado
TARGET := hw

# update SDX_PLATFORM based on your system
SDX_PLATFORM = /proj/xbuilds_released_2019/2019.1/GA/2019.1_0524_1430/internal_platforms/xilinx_u200_xdma_201830_2/xilinx_u200_xdma_201830_2.xpfm
XOCC_OPTS = -t ${TARGET} --platform ${SDX_PLATFORM} \
	--save-temps --config ${CUR_DIR}/prj_config
XO_BUILD_PATH = ../../ip/hls/vitis_lib/vision/L2/tests/resize/resize_DOWN_BILINEAR_NO_RGB
KERN_NAME = resize_accel

.PHONY: all clean

all: binary_container_1/resizer.xclbin
kernel_xo = binary_container_1/resize_accel.xo

binary_container_1/resize_accel.xo:
	@mkdir -p $(@D)
	-@$(RM) $@
	cd $(XO_BUILD_PATH) && make xo TARGET=$(TARGET) DEVICE=$(SDX_PLATFORM)
	cp -rf $(XO_BUILD_PATH)/*/resize_accel.xo $@

binary_container_1/resizer.xclbin: $(kernel_xo)
	v++ $(XOCC_OPTS) -l --temp_dir binary_container_1 --log_dir binary_container_1/logs --remote_ip_cache binary_container_1/ip_cache -o "$@" $(+)
	cp -f $@ resizer.xclbin

clean:
	cd $(XO_BUILD_PATH) && $(RMDIR) sample* .Xil && make cleanall
	${RM} *.o *.elf *.log *.jou sample*
	${RMDIR} binary_container_1/ .Xil/
