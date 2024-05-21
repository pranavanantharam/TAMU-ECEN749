`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 02/05/2024 08:44:47 PM
// Design Name: 
// Module Name: switch
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module clock_divider(output reg div_clock, input clock, input reset);

    reg [31:0] counter;
    
    parameter freq_count = 40000000;

    always @(posedge clock) begin
        
        if( counter == freq_count ) begin
            div_clock <= 1;
            counter <= 0;
        end
        else begin
            div_clock <= 0;
            counter <= counter + 1;
        end
    end
endmodule

