## PYNQ-Helloworld

This repository contains an example application built end-to-end using Xilinx design tools and the Xilinx PYNQ framework. 

The design illustrates how to run a resizer IP to resize an image on the FPGA. There are two notebooks that illustrate the resize operation. One notebook shows the image resizing done in purely software using Python Image Library. The second notebook shows the resize operation being performed in the programmable logic using a resizer IP from the  Xilinx xfopencv library. 

![](./resizer_notebooks.png)





## Quick Start

Open a terminal on your PYNQ board and run:

```
sudo pip3 install --upgrade git+https://xilinx.com/pynq-helloworld.git
```

Currently this repository is compatible with [PYNQ image v2.3](http://www.pynq.io/board).



## License

**PYNQ** License : [BSD 3-Clause License](https://github.com/Xilinx/PYNQ/blob/master/LICENSE)
