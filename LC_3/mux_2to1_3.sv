module mux_2to1_3(
    input select,
    input logic [2:0] D1,
    input logic [2:0] D0,
    output logic [2:0] D_out
    );

    always_comb
    begin 
    
    // D1 if select = 1, D0 is select = 0
        unique case(select)
            1'b0 : D_out = D0 ;
            1'b1 : D_out = D1 ;
        endcase
    end


endmodule
