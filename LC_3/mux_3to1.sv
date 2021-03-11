module mux_3to1(
    input [1:0] select,
    input logic [15:0] D2,
    input logic [15:0] D1,
    input logic [15:0] D0,
    output logic [15:0] D_out
    );

    always_comb
    begin 
    
    // D2 if select = 10, D1 if select = 01, D0 is select = 00
        unique case(select)
            2'b00 : D_out = D0 ;
            2'b01 : D_out = D1 ;
            2'b10 : D_out = D2 ;
            
        endcase
    end


endmodule
