module bus(
    input logic Dout0,Dout1,Dout2,Dout3,
	input logic [15:0] MARMUX, PC, ALU, MDR,
    output logic [15:0] OUT
);

logic [3:0] SELECT;
assign SELECT = {Dout3,Dout2,Dout1,Dout0};

always_comb
	begin
		unique case (SELECT)
	 	    4'b0001   : OUT = MARMUX;
		    4'b0010   : OUT = PC;
		    4'b0100   : OUT = ALU;
		    4'b1000   : OUT = MDR;
	  	 endcase
	end

endmodule
