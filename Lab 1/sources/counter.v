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

module counter(output [3:0] LEDS, input [1:0] BUTTONS, input clock, input reset);

    reg [3:0] counter_val;
    reg dir;
    
    wire div_clock;
    
    clock_divider(div_clock, clock, reset);
    
    always @(posedge div_clock) begin
        if(reset) begin
            counter_val <= 0;
        end
        else begin
            if( BUTTONS[0] == 1 ) begin
                dir = 1;
            end
            else if( BUTTONS[1] == 1 )begin
                dir = 0;
            end

            if( dir == 1 ) begin
                counter_val <= counter_val + 1;
            end
            else if( dir == 0 ) begin
                counter_val <= counter_val - 1;
            end

        end
    end
    
    assign LEDS[3:0] = counter_val[3:0];
    
endmodule