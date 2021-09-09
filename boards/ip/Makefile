IP := resize
VERSION := 1.0

all: hls copy
	@echo "IP built successfully."

hls:
	vivado_hls -f build.tcl
	sed -i 's$<xilinx:family xilinx:lifeCycle="Pre-Production">zynq</xilinx:family>$<xilinx:family xilinx:lifeCycle="Pre-Production">zynq</xilinx:family><xilinx:family xilinx:lifeCycle="Pre-Production">zynquplus</xilinx:family><xilinx:family xilinx:lifeCycle="Pre-Production">zynquplusRFSOC</xilinx:family>$g' $(IP)_$(VERSION)/component.xml

copy:
	cp -rf $(IP)/solution1/impl/ip/*.zip ../../
	cd ../../ && rm -rf $(IP)_$(VERSION) && \
	unzip *.zip -d $(IP)_$(VERSION) && rm *.zip

clean:
	rm -rf $(IP) *.log

