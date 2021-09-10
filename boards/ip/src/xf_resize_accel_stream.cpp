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

typedef xf::cv::ap_axiu<DATA_WIDTH,1,1,1> interface_t;
typedef hls::stream<interface_t> stream_t;


// https://xilinx.github.io/Vitis_Libraries/vision/2020.2/api-reference.html#resolution-conversion
void resize_accel(stream_t& src, stream_t& dst,
             int src_rows, int src_cols, int dst_rows, int dst_cols) {


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

    // Convert stream in to xf::cv::Mat
    xf::cv::AXIvideo2xfMat<DATA_WIDTH, TYPE, HEIGHT, WIDTH, NPIX>(src, src_mat);
    // Run xfOpenCV kernel:
    xf::cv::resize<INTERPOLATION, TYPE, HEIGHT, WIDTH, HEIGHT, WIDTH, NPIX, MAXDOWNSCALE>(src_mat, dst_mat);
    // Convert xf::cv::Mat to stream
    xf::cv::xfMat2AXIvideo<DATA_WIDTH, TYPE, HEIGHT, WIDTH, NPIX>(dst_mat, dst);
}

