module datapath(
	input logic Clk, Reset,
	input logic LD_MAR, LD_MDR, LD_IR, LD_PC,LD_REG,LD_CC,LD_BEN,
	input logic GatePC, GateMDR, GateALU, GateMARMUX,
	input logic MIO_EN, BEN,
	input logic [1:0] PCMUX, ADDR2MUX,ALUK,
	input logic ADDR1MUX,SR1MUX,SR2MUX,DRMUX,
	input logic [15:0] MDR_In, //Data_to_CPU from MEM2IO
	output logic [15:0] MAR, MDR, IR, PC
	);
	
	//internal logic
	logic [15:0] PCMUX_OUT, ALU_OUT,PC_OUT,MDR_OUT,IR_OUT, MAR_OUT;
	logic [15:0] BUS;
	logic [15:0] SR1_OUT,SR2_OUT,ADDR1OUT,ADDR2OUT,SR2MUX_OUT;
	logic [2:0]  SR1MUX_OUT,DRMUX_OUT;
	//muxes logic
	logic [15:0] PC_INC;
	assign PC_INC = PC_OUT + 1'b1; //always_ff???
	logic [15:0] MDRMUX_OUT;
	
	//muxes
	mux_2to1 MDRmux(.select(MIO_EN),.D1(MDR_In),.D0(BUS),.D_out(MDRMUX_OUT)); //if OE = 1 then will read in from MEM2IO
	
	mux_3to1 PCmux(.select(PCMUX),.D2(ADDR_OUT),.D1(BUS),.D0(PC_INC),.D_out(PCMUX_OUT)); //D0 is incremented PC
	
	mux_2to1 ADDR1mux(.select(ADDR1MUX),.D1(SR1_OUT),.D0(PC_OUT),.D_out(ADDR1OUT));
	
	mux_4to1 ADDR2mux(.select(ADDR2MUX),
							.D3({5{IR_OUT[10]},IR_OUT[10:0]}),.D2({7{IR_OUT[8]},IR_OUT[8:0]}), //sign extension by 10 bits can be written as 10{ },... etc
							.D1({10{IR_OUT[5]},IR_OUT[5:0]}),.D0(4'h0000),.D_out(ADDR2OUT)
							);
	
	mux_2to1 SR2mux(.select(SR2MUX),.D1({11{IR_OUT[4]},IR_OUT[4:0]}),.D0(SR2_OUT),.D_out(SR2MUX_OUT)); //16 bit 2to1mux
	
	mux_2to1_3 SR1mux(.select(SR1MUX),.D1(IR_OUT[8:6]),.D0(IR_OUT[11:9]),.D_out(SR1MUX_OUT)); //3 bit 2to1 mux
	
	mux_2to1_3 DRmux(.select(DRMUX),.D1(3'b111),.D0(IR_OUT[11:9]),.D_out(DRMUX_OUT)); 
	
	//ALU
	alu ALU(.B(SR2MUX_OUT),.A(SR1_OUT),.ALUK(ALUK),.ALU_OUT(ALU_OUT));
	
	//BEN unit
	ben BENunit(.Clk(Clk),.Reset(Reset),.IR_3(IR[11:9]),.BUS(BUS),.LD_CC(LD_CC),.LD_BEN(LD_BEN),.BEN(BEN));
	
	//registers
	reg_16 PCreg (.Clk(Clk),.Reset(Reset),.Load(LD_PC),.D(PCMUX_OUT),.D_out(PC_OUT));
	
	reg_16 IRreg (.Clk(Clk),.Reset(Reset),.Load(LD_IR),.D(BUS),.D_out(IR_OUT));
	
	reg_16 MDRreg (.Clk(Clk),.Reset(Reset),.Load(LD_MDR),.D(MDRMUX_OUT),.D_out(MDR_OUT));
	
	reg_16 MARreg (.Clk(Clk),.Reset(Reset),.Load(LD_MAR),.D(BUS),.D_out(MAR_OUT));
	
	//register file
	reg_file regfile(.Clk(Clk),.Reset(Reset),.LD_REG(LD_REG),.DRMUX_OUT(DRMUX_OUT),
							.SR2(SR2),.SR1MUX_OUT(SR1MUX_OUT),.BUS(BUS),
							.SR1_OUT(SR1_OUT),.SR2_OUT(.SR2_OUT)
							);
	
	//for week 1 we set ALU_OUT & ADDR_OUT to 4'h0000
	//assign ALU_OUT = 4'h0000;
	assign ADDR_OUT = ADDR1OUT + ADDR2OUT;
	
	always_comb
		begin
		MAR = MAR_OUT;
		MDR = MDR_OUT;
		IR = IR_OUT;
		PC = PC_OUT;
		end
	
	bus BUSmux(
	.Dout0(GateMARMUX), .Dout1(GatePC), .Dout2(GateALU), .Dout3(GateMDR),
	.MARMUX(ADDR_OUT),.PC(PC_OUT),.ALU(ALU_OUT),.MDR(MDR_OUT),
	.OUT(BUS)
	);
	
	endmodule
	