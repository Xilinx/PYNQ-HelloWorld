## HLS IP

This folder helps users rebuild the Vitis Library IP for Vivado HLS flow.
The `vitis_lib` folder is git submodule pointing to the Vitis Libraries.

### Build an HLS IP

To rebuild the HLS IP move in this folder on a terminal session and type

```
make
```

### Change supported families

By default, the IP is built targeting a specific family or device. 
To make changes to the supported devices or families, you can
manually locate and edit the following lines in 
`<local_repo>/boards/ip/<ip>/component.xml`:

```
<xilinx:family xilinx:lifeCycle="Pre-Production">zynq</xilinx:family>
<xilinx:family xilinx:lifeCycle="Pre-Production">zynquplus</xilinx:family>
<xilinx:family xilinx:lifeCycle="Pre-Production">zynquplusRFSOC</xilinx:family>
```

After you save it, the supported families are updated.

## Licenses

Vitis Libraries License: [Apache License 2.0](https://github.com/Xilinx/Vitis_Libraries/blob/master/LICENSE.txt)

