module alu(
	input logic [15:0] B,A,
	input logic [2:0] ALUK,
	output logic [15:0] ALU_OUT
);

always_comb
	begin
		unique case(ALUK)
			2'b00 : ALU_OUT = A + B; //add
			2'b01 : ALU_OUT = A & B; //and
			2'b10 : ALU_OUT = ~A;    //not A
			2'b11 : ALU_OUT = A;     //A
		endcase
	end
	
endmodule
