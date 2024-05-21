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

module jackpot(output [3:0] LEDS, input [3:0] SWITCHES, input clock, input reset);

    wire div_clock;

    reg game_finish = 0;
    reg [3:0] led_state = 4'b0001;
    reg [3:0] prev_switch_state = 4'b0000;

    clock_divider(div_clock, clock, reset);

    always @(posedge div_clock) begin

        if( reset ) begin
            led_state = 4'b0001;
            prev_switch_state = 4'b0000;
            game_finish = 0;
        end
        else begin
            if( game_finish != 1 ) begin
                if( (prev_switch_state[0] == 0 && SWITCHES[0] == 1 && led_state == 4'b0001) ||
                        (prev_switch_state[1] == 0 && SWITCHES[1] == 1 && led_state == 4'b0010) ||
                        (prev_switch_state[2] == 0 && SWITCHES[2] == 1 && led_state == 4'b0100) ||
                        (prev_switch_state[3] == 0 && SWITCHES[3] == 1 && led_state == 4'b1000)
                    ) begin
                        led_state <= 4'b1111;
                        game_finish = 1;
                    end
                    
                    if( game_finish != 1 ) begin
                        if( led_state == 4'b1000 ) begin
                            led_state <= 4'b0001;
                        end
                        else begin
                            led_state <= (led_state << 1);
                        end
                    end
                end
            end
        
        // Update previous switch state
        prev_switch_state[3:0] = SWITCHES[3:0];
    end

    assign LEDS[3:0] = led_state[3:0];

endmodule