# Rebuild Build PYNQ-HelloWorld

A pre-build version of the PYNQ-HelloWorld for a variety of boards is provided.

The process to rebuild the design is fully automated. Navigate to the board folder and run `make`. Below, you can see an example for edge platforms as well as Alveo acceleration cards.

## Edge Devices

The code snipped below shows an example to rebuild the design for the KV260 ([Kria KV260 Vision AI Starter Kit](https://www.xilinx.com/products/som/kria/kv260-vision-starter-kit.html))

```bash
cd KV260/resizer
make
```

## Alveo Accelerated cards

The code snipped below shows an example to rebuild the design for the Alveo U55C ([Alveo U55C High Performance Compute Card](https://www.xilinx.com/products/boards-and-kits/alveo/u55c.html)).

```bash
git submodule init && git submodule update
cd U55C/resizer
make
```

> **Note:** that for Alveo you need to pull the [Vitis Accelerated Libraries](https://github.com/Xilinx/Vitis_Libraries), which are a submodule of this repository.
