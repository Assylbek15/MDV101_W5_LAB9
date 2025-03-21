module alu (
    input wire carry_in, 
    input wire [15:0] in_a,
    input wire [15:0] in_b,
    input wire [3:0]  select,
    input wire        mode,
    output wire       carry_out,
    output wire       compare,
    output wire [15:0] alu_out
);

// Internal signals for arithmetic and logic outputs
wire [15:0] arithmetic_out, logic_out;
wire arithmetic_carry_out;

// Arithmetic Unit
// Arithmetic Unit
ArithmeticUnit arithmetic_unit (
    .carry_in(carry_in),
    .in_a(in_a),
    .in_b(in_b),
    .select(select),
    .out(arithmetic_out),
    .carry_out(arithmetic_carry_out)
);

// Logic Unit
LogicUnit logic_unit(
    .in_a(in_a),
    .in_b(in_b),
    .select(select),
    .out(logic_out)
);

// Select output based on mode (0 = arithmetic, 1 = logic)
assign alu_out = (mode) ? logic_out : arithmetic_out;

// Carry out is valid only in arithmetic mode
assign carry_out = (mode == 1'b0) ? arithmetic_carry_out : 1'bx;

// Compare should always be set if inputs are equal (regardless of mode)
assign compare = (in_a == in_b) ? 1'b1 : 1'b0;

endmodule
