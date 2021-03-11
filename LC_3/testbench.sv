module testbench();

timeunit 10ns;
timeprecision 1ns;

//signals from toplevel
logic Clk = 0;
logic Run,Continue;
logic [9:0] SW;
logic [9:0] LED;
logic [6:0] HEX0, HEX1, HEX2, HEX3;

//to compare the value of the MAR,PC,MDR, IR
//logic [15:0] MAR_ans,PC_ans,MDR_ans,IR_ans;

//counter to check number of errors 
//integer errCnt = 0;

//add internal logic signals to debug
logic [15:0] MARsig,MDRsig,IRsig,PCsig;
logic LD_PCsig;
logic [15:0] BUSsig;

slc3_testtop testing(.*);

assign MARsig = testing.slc.MAR;
assign MDRsig = testing.slc.MDR;
assign IRsig = testing.slc.IR;
assign PCsig = testing.slc.PC;
assign LD_PCsig = testing.slc.LD_PC;
assign BUSsig = testing.slc.d0.BUS;

always begin: CLOCK_GENERATION
#1 Clk = ~Clk;
end

initial begin:CLOCK_INITIALIZATION
Clk = 0;
end

initial begin: TEST_VECTORS
//reset:
#2 Run = 0;
#2 Continue = 0;
#2 Continue = 1;

#2 Run = 1;
#2 Run = 0;
#2 Run = 1;

#2 Continue =  1;
#2 Continue = 0;
#2 Continue = 1;


end

endmodule
