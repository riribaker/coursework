module ben(
	input logic [15:0] BUS, 
	input logic [2:0] IR_3,
	input logic LD_CC, LD_BEN, Clk, Reset,
	output logic BEN
);
logic n,z,p;
logic n_in,z_in,p_in,BEN_in;

//2-always method:
always_ff (@posedge Clk) begin
	if (Reset)
		BEN = 1'b0;
	if (LD_CC)
		n <= n_in;
		z <= z_in;
		p <= p_in;
	if (LD_BEN)
		BEN <= ((IR_3 & {n,z,p}) != 3'b000); //check with lecture to see how to implement differently
	
always_comb 
	begin
		//set default values
		n = 1'bZ;
		z = 1'bZ;
		p = 1'bZ;
		n_in = 1'b0;
		z_in = 1'b0;
		p_in = 1'b0;

		if (BUS == 16'h0000) //zero
			z_in = 1'b0;
		else if (BUS[15] == 1'b0) //positive
			p_in = 1'b1;
		else if (BUS[15]) //negative
			n_in = 1'b1;
			
	end
endmodule


