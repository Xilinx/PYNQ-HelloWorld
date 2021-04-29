/***************************************************************************
Copyright (c) 2021, Xilinx, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors 
may be used to endorse or promote products derived from this software 
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***************************************************************************/
#include "xf_resize_config.h"

extern "C" {
void resizer(ap_uint<INPUT_PTR_WIDTH>* src, ap_uint<OUTPUT_PTR_WIDTH>* dst,
             int src_rows, int src_cols, int dst_rows, int dst_cols) {
    #pragma HLS INTERFACE m_axi     port=src  offset=slave bundle=gmem1
    #pragma HLS INTERFACE m_axi     port=dst  offset=slave bundle=gmem2
    #pragma HLS INTERFACE s_axilite port=src_rows              
    #pragma HLS INTERFACE s_axilite port=src_cols              
    #pragma HLS INTERFACE s_axilite port=dst_rows              
    #pragma HLS INTERFACE s_axilite port=dst_cols              
    #pragma HLS INTERFACE s_axilite port=return
    xf::cv::Mat<TYPE, HEIGHT, WIDTH, NPC_T> src_mat(src_rows, src_cols);
    #pragma HLS stream variable=src_mat.data depth=2
    xf::cv::Mat<TYPE, NEWHEIGHT, NEWWIDTH, NPC_T> dst_mat(dst_rows, dst_cols);
    #pragma HLS stream variable=dst_mat.data depth=2
    #pragma HLS DATAFLOW
    xf::cv::Array2xfMat<INPUT_PTR_WIDTH, TYPE, HEIGHT, WIDTH, NPC_T>(src, src_mat);
    xf::cv::resize<INTERPOLATION, TYPE, HEIGHT, WIDTH, NEWHEIGHT, NEWWIDTH, NPC_T, MAXDOWNSCALE>(src_mat, dst_mat);
    xf::cv::xfMat2Array<OUTPUT_PTR_WIDTH, TYPE, NEWHEIGHT, NEWWIDTH, NPC_T>(dst_mat, dst);
}
}
