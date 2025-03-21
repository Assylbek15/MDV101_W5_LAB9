module ArithmeticUnit (
    input wire carry_in, 
    input wire [15:0] in_a,
    input wire [15:0] in_b,
    input wire [3:0] select,
    output reg [15:0] out,
    output reg carry_out
);

    wire [15:0] not_b = ~in_b;  // B' (bitwise NOT of B)
    wire [15:0] add_ab = in_a + in_b;  // A + B
    wire [15:0] add_ab_complement = in_a + not_b; // A + B'
    wire [15:0] add_aab = in_a + (in_a & not_b);  // A + AB'
    wire [15:0] add_aplus_ab = add_ab + (in_a & not_b); // (A + B) + AB'
    wire [15:0] subtract_ab_1 = in_a - in_b - 1; // A - B - 1
    wire [15:0] subtract_ab_prime = (in_a & not_b) - 1; // AB' - 1
    wire [15:0] add_aab_true = in_a + (in_a & in_b); // A + AB
    wire [15:0] add_a_b_prime_ab = (in_a + not_b) + (in_a & in_b); // (A + B') + AB
    wire [15:0] ab_subtract_1 = (in_a & in_b) - 1; // AB - 1
    wire [15:0] add_a_a = in_a + in_a;  // A + A
    wire [15:0] add_a_plus_a_b = (in_a + in_b) + in_a; // (A + B) + A
    wire [15:0] add_a_plus_a_b_prime = (in_a + not_b) + in_a; // (A + B') + A
    wire [15:0] subtract_a_1 = in_a - 1; // A - 1

    always @(*) begin
        case (select)
            4'b0000: out = in_a;                // A
            4'b0001: out = add_ab;              // A + B
            4'b0010: out = add_ab_complement;   // A + B'
            4'b0011: out = 16'b1111_1111_1111_1111; // -1 (Two's complement representation)
            4'b0100: out = add_aab;             // A + AB'
            4'b0101: out = add_aplus_ab;        // (A + B) + AB'
            4'b0110: out = subtract_ab_1;       // A - B - 1
            4'b0111: out = subtract_ab_prime;   // AB' - 1
            4'b1000: out = add_aab_true;        // A + AB
            4'b1001: out = add_ab;              // A + B
            4'b1010: out = add_a_b_prime_ab;    // (A + B') + AB
            4'b1011: out = ab_subtract_1;       // AB - 1
            4'b1100: out = add_a_a;             // A + A
            4'b1101: out = add_a_plus_a_b;      // (A + B) + A
            4'b1110: out = add_a_plus_a_b_prime; // (A + B') + A
            4'b1111: out = subtract_a_1;        // A - 1
            default: out = 16'b0000_0000_0000_0000;
        endcase

        // Carry out is set only for addition cases
        carry_out = (select == 4'b0001 || select == 4'b0010 || 
                     select == 4'b0101 || select == 4'b1001 || 
                     select == 4'b1101 || select == 4'b1110) ? carry_in : 1'b0;
    end

endmodule
