// Copyright (C) 2021 Xilinx, Inc
//
// SPDX-License-Identifier: BSD-3-Clause

#include "hls_stream.h"
#include "common/xf_common.hpp"
#include "common/xf_infra.hpp"
#include "imgproc/xf_resize.hpp"

#define DATA_WIDTH 24
#define NPIX XF_NPPC1

/*  set the height and width  */
#define WIDTH 3840
#define HEIGHT 2160
#define FILTER_SIZE 3
#define TYPE XF_8UC3
#define INTERPOLATION XF_INTERPOLATION_BILINEAR
#define MAXDOWNSCALE 9

typedef ap_axiu<DATA_WIDTH,1,1,1> interface_t;
typedef hls::stream<interface_t> stream_t;


/*
* We use the custom axis2xfMat and xfMat2axis and instead default
* xf::cv::AXIvideo2xfMat and xf::cv::xfMat2AXIvideo
* because the Hello-World uses a regular DMA.
* So, we only need last is only asserted for final pixel of the image.
*/

template <int W, int TYPE, int ROWS, int COLS, int NPPC>
void axis2xfMat (hls::stream<ap_axiu<W, 1, 1, 1> >& AXI_video_strm, xf::cv::Mat<TYPE, ROWS, COLS, NPPC>& img) {
    ap_axiu<W, 1, 1, 1> axi;

    const int m_pix_width = XF_PIXELWIDTH(TYPE, NPPC) * XF_NPIXPERCYCLE(NPPC);

    int rows = img.rows;
    int cols = img.cols >> XF_BITSHIFT(NPPC);

    assert(img.rows <= ROWS);
    assert(img.cols <= COLS);

loop_row_axi2mat:
    for (int i = 0; i < rows; i++) {
    loop_col_zxi2mat:
        for (int j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II=1

            AXI_video_strm.read(axi);
            img.write(i*rows + j, axi.data(m_pix_width - 1, 0));
        }
    }
}

template <int W, int TYPE, int ROWS, int COLS, int NPPC>
void xfMat2axis(xf::cv::Mat<TYPE, ROWS, COLS, NPPC>& img, hls::stream<ap_axiu<W, 1, 1, 1> >& dst) {
    ap_axiu<W, 1, 1, 1> axi;

    int rows = img.rows;
    int cols = img.cols >> XF_BITSHIFT(NPPC);

    assert(img.rows <= ROWS);
    assert(img.cols <= COLS);

    const int m_pix_width = XF_PIXELWIDTH(TYPE, NPPC) * XF_NPIXPERCYCLE(NPPC);

loop_row_mat2axi:
    for (int i = 0; i < rows; i++) {
    loop_col_mat2axi:
        for (int j = 0; j < cols; j++) {
#pragma HLS loop_flatten off
#pragma HLS pipeline II = 1

            /*Assert last only in the last pixel*/
            if ((j == cols-1) && (i == rows-1)) {
                axi.last = 1;
            } else {
                axi.last = 0;
            }

            axi.data = 0;
            axi.data(m_pix_width - 1, 0) = img.read(i*rows + j);
            axi.keep = -1;
            dst.write(axi);
        }
    }
}

// https://xilinx.github.io/Vitis_Libraries/vision/2020.2/api-reference.html#resolution-conversion
void resize_accel(stream_t& src, stream_t& dst,
                  int src_rows, int src_cols,
                  int dst_rows, int dst_cols) {


    #pragma HLS INTERFACE axis register both port=src
    #pragma HLS INTERFACE axis register both port=dst

    #pragma HLS INTERFACE s_axilite port=src_rows              
    #pragma HLS INTERFACE s_axilite port=src_cols              
    #pragma HLS INTERFACE s_axilite port=dst_rows              
    #pragma HLS INTERFACE s_axilite port=dst_cols              
    #pragma HLS INTERFACE s_axilite port=return

    xf::cv::Mat<TYPE, HEIGHT, WIDTH, NPIX> src_mat(src_rows, src_cols);
    xf::cv::Mat<TYPE, HEIGHT, WIDTH, NPIX> dst_mat(dst_rows, dst_cols);

    #pragma HLS DATAFLOW

    // Convert stream to xf::cv::Mat
    axis2xfMat<DATA_WIDTH, TYPE, HEIGHT, WIDTH, NPIX>(src, src_mat);
    // Run xfOpenCV kernel:
    xf::cv::resize<INTERPOLATION, TYPE, HEIGHT, WIDTH, HEIGHT, WIDTH, NPIX, MAXDOWNSCALE>(src_mat, dst_mat);
    // Convert xf::cv::Mat to stream
    xfMat2axis<DATA_WIDTH, TYPE, HEIGHT, WIDTH, NPIX>(dst_mat, dst);

}
