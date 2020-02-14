## HLS IP

This folder helps users rebuild the Vitis Library IP for Vivado HLS flow.
The `vitis_lib` folder is git submodule pointing to the Vitis Libraries.

### Build an HLS IP

For each HLS IP to be rebuilt (e.g., `resize`), 
go to the corresponding folder and run

```
make
```

And the IP will be copied out to the folder one level above the `hls` folder.
To avoid duplicated IPs shown up in Vivado project, you can run

```
make clean
```

This will remove the generated files from the `hls` folder, leaving only the
copied IP in the repository.

### Change supported families

By default, the IP is built targetting a specific family or device. 
To make changes to the supported devices or families, you can
manually locate and edit the following lines in 
`<local_repo>/boards/ip/<ip>/component.xml`:

```
<xilinx:family xilinx:lifeCycle="Pre-Production">zynq</xilinx:family>
<xilinx:family xilinx:lifeCycle="Pre-Production">zynquplus</xilinx:family>
<xilinx:family xilinx:lifeCycle="Pre-Production">zynquplusRFSOC</xilinx:family>
```

After you save it, the supported families are updated.

Users don't have to redo this step if they are using the IP uploaded on this
repository directly. This step is only required if users are rebuilding the IP
from the `hls` folder.

## Licenses

Vitis Libraries License: [Apache License 2.0](https://github.com/Xilinx/Vitis_Libraries/blob/master/LICENSE.txt)

