module traffic_light_fsm (
    input clk,
    input reset,                // Asynchronous reset
    input wire [3:0] code_in,   // 4-bit input from keypad
    output reg locked // remain high(1) unless FSM reaches the "Unlocked" state.
);

    // State encoding
    parameter S_LOCKED = 3'b000;   // Initial state -> locked
    parameter S_1 = 3'b001;   // First correct digit(3)
    parameter S_2 = 3'b010;   // Second correct digit(3)
    parameter S_3 = 3'b011;   // (5)
    parameter S_4 = 3'b100;   // (2)
    parameter S_5 = 3'b101;   // (5)
    parameter S_UNLOCKED =  3'b110; // correct sequence entered(6) ->unlock

    reg [2:0] current_state, next_state;

    // Model state register (sequential logic)
    always @(posedge clk or posedge reset) begin
        if (reset) begin
            current_state <= S_LOCKED;
        end else begin
            current_state <= next_state;
        end
    end

    // Model next state decode logic (combinational logic)
    always @(*) begin
        case (current_state)
            S_LOCKED: begin
                next_state = (code_in == 4'd3) ? S_1 : S_LOCKED;
            end
            S_1: begin
                next_state = (code_in == 4'd3) ? S_2 : S_LOCKED;
            end
            S_2: begin
                next_state = (code_in == 4'd5) ? S_3 : S_LOCKED;
            end
            S_3: begin
                next_state = (code_in == 4'd2) ? S_4 : S_LOCKED;
            end
            S_4: begin
                next_state = (code_in == 4'd5) ? S_5 : S_LOCKED;
            end
            S_5: begin
                next_state = (code_in == 4'd6) ? S_UNLOCKED : S_LOCKED;
            end

            default: begin
                next_state = S_LOCKED;
            end
        endcase
    end

    // Model output logic (combinational logic)
    always @(*) begin
        locked = (current_state != S_UNLOCKED); // locked = 1 unless in Unlocked state
    end
endmodule