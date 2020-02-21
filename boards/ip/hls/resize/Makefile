IP := resize
VERSION := 1.0

all: hls copy
	@echo "IP built successfully."

hls:
	vivado_hls -f script.tcl

copy:
	cp -rf $(IP)/solution1/impl/ip/*.zip ../../
	cd ../../ && rm -rf $(IP)_$(VERSION) && \
	unzip *.zip -d $(IP)_$(VERSION) && rm *.zip

clean:
	rm -rf $(IP) *.log

