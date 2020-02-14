// ==============================================================
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2019.2 (64-bit)
// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// ==============================================================
`timescale 1 ns / 1 ps

module resize_accel_udivbkb_div_u
#(parameter
    in0_WIDTH = 32,
    in1_WIDTH = 32,
    out_WIDTH = 32
)
(
    input                       clk,
    input                       reset,
    input                       ce,
    input                       start,
    input       [in0_WIDTH-1:0] dividend,
    input       [in1_WIDTH-1:0] divisor,
    output wire                 done,
    output wire [out_WIDTH-1:0] quot,
    output wire [out_WIDTH-1:0] remd
);

localparam cal_WIDTH = (in0_WIDTH > in1_WIDTH)? in0_WIDTH : in1_WIDTH;

//------------------------Local signal-------------------
reg     [in0_WIDTH-1:0] dividend0;
reg     [in1_WIDTH-1:0] divisor0;
reg     [in0_WIDTH-1:0] dividend_tmp;
reg     [in0_WIDTH-1:0] remd_tmp;
wire    [in0_WIDTH-1:0] dividend_tmp_mux;
wire    [in0_WIDTH-1:0] remd_tmp_mux;
wire    [in0_WIDTH-1:0] comb_tmp;
wire    [cal_WIDTH:0]   cal_tmp;

//------------------------Body---------------------------
assign  quot   = dividend_tmp;
assign  remd   = remd_tmp;

// dividend0, divisor0
always @(posedge clk)
begin
    if (start) begin
        dividend0 <= dividend;
        divisor0  <= divisor;
    end
end

// One-Hot Register
// r_stage[0]=1:accept input; r_stage[in0_WIDTH]=1:done
reg     [in0_WIDTH:0]     r_stage;
assign done = r_stage[in0_WIDTH];
always @(posedge clk)
begin
    if (reset == 1'b1)
        r_stage[in0_WIDTH:0] <= {in0_WIDTH{1'b0}};
    else if (ce)
        r_stage[in0_WIDTH:0] <= {r_stage[in0_WIDTH-1:0], start};
end

// MUXs
assign  dividend_tmp_mux = r_stage[0]? dividend0 : dividend_tmp;
assign  remd_tmp_mux     = r_stage[0]? {in0_WIDTH{1'b0}} : remd_tmp;

if (in0_WIDTH == 1) assign comb_tmp = dividend_tmp_mux[0];
else                assign comb_tmp = {remd_tmp_mux[in0_WIDTH-2:0], dividend_tmp_mux[in0_WIDTH-1]};

assign  cal_tmp  = {1'b0, comb_tmp} - {1'b0, divisor0};

always @(posedge clk)
begin
    if (ce) begin
        if (in0_WIDTH == 1) dividend_tmp <= ~cal_tmp[cal_WIDTH];
        else           dividend_tmp <= {dividend_tmp_mux[in0_WIDTH-2:0], ~cal_tmp[cal_WIDTH]};
        remd_tmp     <= cal_tmp[cal_WIDTH]? comb_tmp : cal_tmp[in0_WIDTH-1:0];
    end
end

endmodule

module resize_accel_udivbkb_div
#(parameter
        in0_WIDTH   = 32,
        in1_WIDTH   = 32,
        out_WIDTH   = 32
)
(
        input                           clk,
        input                           reset,
        input                           ce,
        input                           start,
        output  reg                     done,
        input           [in0_WIDTH-1:0] dividend,
        input           [in1_WIDTH-1:0] divisor,
        output  reg     [out_WIDTH-1:0] quot,
        output  reg     [out_WIDTH-1:0] remd
);
//------------------------Local signal-------------------
reg                       start0 = 'b0;
wire                      done0;
reg     [in0_WIDTH-1:0] dividend0;
reg     [in1_WIDTH-1:0] divisor0;
wire    [in0_WIDTH-1:0] dividend_u;
wire    [in1_WIDTH-1:0] divisor_u;
wire    [out_WIDTH-1:0] quot_u;
wire    [out_WIDTH-1:0] remd_u;
//------------------------Instantiation------------------
resize_accel_udivbkb_div_u #(
    .in0_WIDTH      ( in0_WIDTH ),
    .in1_WIDTH      ( in1_WIDTH ),
    .out_WIDTH      ( out_WIDTH )
) resize_accel_udivbkb_div_u_0 (
    .clk      ( clk ),
    .reset    ( reset ),
    .ce       ( ce ),
    .start    ( start0 ),
    .done     ( done0 ),
    .dividend ( dividend_u ),
    .divisor  ( divisor_u ),
    .quot     ( quot_u ),
    .remd     ( remd_u )
);
//------------------------Body---------------------------
assign dividend_u = dividend0;
assign divisor_u = divisor0;

always @(posedge clk)
begin
    if (ce) begin
        dividend0 <= dividend;
        divisor0  <= divisor;
        start0    <= start;
    end
end

always @(posedge clk)
begin
    done <= done0;
end

always @(posedge clk)
begin
    if (done0) begin
        quot <= quot_u;
        remd <= remd_u;
    end
end

endmodule


`timescale 1 ns / 1 ps
module resize_accel_udivbkb(
    clk,
    reset,
    ce,
    start,
    done,
    din0,
    din1,
    dout);

parameter ID = 32'd1;
parameter NUM_STAGE = 32'd1;
parameter din0_WIDTH = 32'd1;
parameter din1_WIDTH = 32'd1;
parameter dout_WIDTH = 32'd1;
input clk;
input reset;
input ce;
input start;
output done;
input[din0_WIDTH - 1:0] din0;
input[din1_WIDTH - 1:0] din1;
output[dout_WIDTH - 1:0] dout;

wire[dout_WIDTH - 1:0] sig_remd;


resize_accel_udivbkb_div #(
.in0_WIDTH( din0_WIDTH ),
.in1_WIDTH( din1_WIDTH ),
.out_WIDTH( dout_WIDTH ))
resize_accel_udivbkb_div_U(
    .dividend( din0 ),
    .divisor( din1 ),
    .quot( dout ),
    .remd( sig_remd ),
    .clk( clk ),
    .ce( ce ),
    .reset( reset ),
    .start( start ),
    .done( done ));

endmodule

