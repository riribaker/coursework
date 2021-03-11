module reg_file(
	input logic Clk, Reset, LD_REG,
	input logic [2:0] DRMUX_OUT,SR2,SR1MUX_OUT
	input logic [15:0] BUS,
	output logic [15:0] SR1_OUT,SR2_OUT
);

//to do