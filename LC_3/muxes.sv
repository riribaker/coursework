//contains all muxes used in slc3

//MDRmux, ADDR1mux, SR2mux
module mux_2to1(
	input select,
   input logic [15:0] D1,
   input logic [15:0] D0,
   output logic [15:0] D_out
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

//SR1mux, DRmux
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

//PCmux
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

//ADDR2mux
module mux_4to1(
	input [1:0] select,
   input logic [15:0] D3, 
   input logic [15:0] D2,
   input logic [15:0] D1,
   input logic [15:0] D0,
   output logic [15:0] D_out
   );

   always_comb
   begin 
    
    // D3 if select = 11, D2 if select = 10, D1 if select = 01, D0 is select = 00
       unique case(select)
           2'b00 : D_out = D0 ;
           2'b01 : D_out = D1 ;
           2'b10 : D_out = D2 ;
           2'b11 : D_out = D3 ;
            
       end case
   end

endmodule

