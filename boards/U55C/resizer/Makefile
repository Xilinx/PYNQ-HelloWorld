include platform.mk

CUR_DIR = $(shell pwd)

RM = rm -f
RMDIR = rm -rf

TARGET := hw

# update VITIS_PLATFORM based on your system
VITIS_PLATFORM = /proj/xbuilds/2022.1_released/internal_platforms/$(PLATFORM)/$(PLATFORM).xpfm
VPP_OPTS = -t ${TARGET} --platform ${VITIS_PLATFORM} \
	--save-temps --config ${CUR_DIR}/prj_config
XO_BUILD_PATH = ../../ip/vitis_lib/vision/L2/tests/resize/resize_DOWN_BILINEAR_NO_RGB
KERN_NAME = resize_accel

.PHONY: all clean

all: binary_container_1/resizer.xclbin
kernel_xo = binary_container_1/resize_accel.xo

binary_container_1/resize_accel.xo:
	@mkdir -p $(@D)
	-@$(RM) $@
	cd $(XO_BUILD_PATH) && make xo TARGET=$(TARGET) DEVICE=$(VITIS_PLATFORM)
	cp -rf $(XO_BUILD_PATH)/*/resize_accel.xo $@

binary_container_1/resizer.xclbin: $(kernel_xo)
	v++ $(VPP_OPTS) -l --temp_dir binary_container_1 --log_dir binary_container_1/logs --remote_ip_cache binary_container_1/ip_cache -o "$@" $(+)
	cp -f $@ resizer.xclbin

clean:
	cd $(XO_BUILD_PATH) && $(RMDIR) sample* .Xil && make cleanall
	${RM} *.o *.elf *.log *.jou sample*
	${RMDIR} binary_container_1/ .Xil/
