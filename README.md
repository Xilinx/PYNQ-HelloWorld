## PYNQ-Helloworld

This repository contains a "Hello World" introduction application to the Xilinx PYNQ framework. 

The design illustrates how to run a resizer IP to resize an image on the FPGA. There are two notebooks that illustrate the resize operation. One notebook shows the image resizing done purely in software using Python Image Library. The second notebook shows the resize operation being performed in the programmable logic using a resizer IP from the  Xilinx xfopencv library. 
https://github.com/Xilinx/xfopencv/tree/master/examples/resize   


![](./resizer_notebooks.png)


## Quick Start

Open a terminal on your PYNQ board and run:

```
sudo pip3 install pynq-helloworld
```

Currently this repository is compatible with `pynq` package v2.5.1.

Go to your jupyter home folder (on edge boards, this is 
`/home/xilinx/jupyter_notebooks`), and run the following to deliver the notebooks:

```
pynq get-notebooks pynq-helloworld -p .
```

The `-p` option specifies the target folder location. Then you should be 
able to try the notebooks!

## Supporting Boards

Currently this repository is supporting:

* **Zynq-7000 boards**: Pynq-Z1, Pynq-Z2, etc.
* **Zynq Ultrascale boards**: Ultra96, ZCU104, etc.
* **PCIE Alveo cards**: U200, U250, U280.
* **AWS F1 instance**: VU9P.

For AWS, a few additional steps are required to generate the `*.awsxclbin`
file. For more information, you can check the `README.md` inside 
`boards/VU9P/resizer`.

## License

**PYNQ** License : [BSD 3-Clause License](https://github.com/Xilinx/PYNQ/blob/master/LICENSE)
