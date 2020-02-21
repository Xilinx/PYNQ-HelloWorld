/***************************************************************************
Copyright (c) 2016, Xilinx, Inc.
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
#include "xf_axis_config.h"
#include "xf_resize_config.h"

void axis2xfMat (axis_t *src, 
		 xf::cv::Mat<TYPE, HEIGHT, WIDTH, NPC_T> &_src, 
		 int src_rows, int src_cols) {
#pragma HLS inline off

	for (int i=0; i<src_rows; i++) {
		for (int j=0; j<src_cols; j++) {
#pragma HLS pipeline
#pragma HLS loop_flatten off
			_src.data[i*src_cols+j] = src[i*src_cols+j].data;
		}	
	}

}

void xfMat2axis (xf::cv::Mat<TYPE, NEWHEIGHT, NEWWIDTH, NPC_T> &_dst,
		 axis_t *dst,
		 int dst_rows, int dst_cols) {
#pragma HLS inline off

	for (int i=0; i<dst_rows; i++) {
		for (int j=0; j<dst_cols; j++) {
#pragma HLS pipeline
#pragma HLS loop_flatten off
			ap_uint<1> tmp = 0;
			if ((i==dst_rows-1) && (j== dst_cols-1)) {
				tmp = 1;
			}
			dst[i*dst_cols+j].last = tmp;
			dst[i*dst_cols+j].data = _dst.data[i*dst_cols+j];
		}
	}
}

extern "C" {
void resize_accel (axis_t *src, axis_t *dst, int src_rows, int src_cols, int dst_rows, int dst_cols) {
	
#pragma HLS INTERFACE axis port=src //depth=384*288 // Added depth for C/RTL cosimulation
#pragma HLS INTERFACE axis port=dst //depth=192*144 // Added depth for C/RTL cosimulation
#pragma HLS INTERFACE s_axilite port=src_rows
#pragma HLS INTERFACE s_axilite port=src_cols
#pragma HLS INTERFACE s_axilite port=dst_rows
#pragma HLS INTERFACE s_axilite port=dst_cols
#pragma HLS INTERFACE s_axilite port=return

	xf::cv::Mat<TYPE, HEIGHT, WIDTH, NPC_T> src_mat(src_rows, src_cols);
	xf::cv::Mat<TYPE, NEWHEIGHT, NEWWIDTH, NPC_T> dst_mat(dst_rows, dst_cols);
#pragma HLS stream variable=src_mat.data depth=150
#pragma HLS stream variable=dst_mat.data depth=150

#pragma HLS dataflow
	
	axis2xfMat(src, src_mat, src_rows, src_cols);	

	xf::cv::resize<INTERPOLATION, TYPE, HEIGHT, WIDTH, NEWHEIGHT, NEWWIDTH, NPC_T, MAXDOWNSCALE>(src_mat, dst_mat);

	xfMat2axis(dst_mat, dst, dst_rows, dst_cols);	

}
}

