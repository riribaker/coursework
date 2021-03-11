module reg_16( //16 bit register, no shift needed
    input Clk, Reset, Load,
    input logic [15:0] D,
    output logic [15:0] D_out
    );

//this outline of the 16 bit register was based on lecture notes


//simplified from lab 4 
    always_ff @ (posedge Clk) begin
        
    
        if (Reset) 
            D_out = 1'b0;
        else if (Load) 
            D_out = D;
        
    end



endmodule
