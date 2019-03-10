## HLS IP

This folder contains the source code for HLS IP.
For example, the `resize` IP is built from HLS code; this function uses 
a subset of publicly available xfOpenCV library from Xilinx.

To build all the IP's in this folder, please run:

```
./build_ip.sh
```

To build a specific IP, please run the following:

```
vivado_hls <ip_name>/script.tcl
```

For example, for the `resize` IP, we can run:

```
vivado_hls resize/script.tcl
```

## Licenses

**xfOpenCV** License : [BSD 3-Clause License](https://github.com/Xilinx/xfopencv/blob/master/LICENSE.txt)
