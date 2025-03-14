#include <verilated.h>
#include <iostream>
#include <cstdlib>   // For rand()
#include <ctime>     // For seeding random numbers
#include "Vlab9_ALU.h" // Verilator-generated ALU header

#define NUM_TESTS 1000  // Number of random tests per operation

int main(int argc, char **argv) {
    // Initialize Verilator
    Verilated::commandArgs(argc, argv);
    
    // Instantiate ALU
    Vlab9_ALU* alu = new Vlab9_ALU;

    // Seed the random number generator
    std::srand(std::time(0));

    std::cout << "Starting ALU verification with " << NUM_TESTS << " tests per operation...\n";

    // Loop through all possible select values (4-bit: 16 operations)
    for (int mode = 0; mode < 2; mode++) {  // mode: 0 = Arithmetic, 1 = Logic
        for (int select = 0; select < 16; select++) {
            uint32_t pass_count = 0;

            // Run 1000 tests per operation
            for (int i = 0; i < NUM_TESTS; i++) {
                // Generate random inputs
                uint16_t in_a = rand() & 0xFFFF;  // Random 16-bit number
                uint16_t in_b = rand() & 0xFFFF;
                uint8_t carry_in = rand() & 0x1; // Random 0 or 1

                // Apply inputs
                alu->in_a = in_a;
                alu->in_b = in_b;
                alu->carry_in = carry_in;
                alu->select = select;
                alu->mode = mode; // Set mode: Arithmetic (0) or Logic (1)

                // Evaluate ALU
                alu->eval();

                // Get outputs
                uint16_t alu_out = alu->alu_out;
                uint8_t carry_out = alu->carry_out;
                uint8_t compare = alu->compare;

                // Compute expected results
                uint16_t expected_out = 0;
                uint8_t expected_carry_out = 0;
                uint8_t expected_compare = (in_a == in_b) ? 1 : 0; // Compare is always set if in_a == in_b

                // Perform expected operation (matching 74181 table)
                if (mode == 0) {  // Arithmetic mode
                    switch (select) {
                        case 0x0: expected_out = in_a; break;
                        case 0x1: expected_out = in_a + in_b; break;
                        case 0x2: expected_out = in_a + ~in_b; break;
                        case 0x3: expected_out = 0xFFFF; break;  // -1 in 16-bit
                        case 0x4: expected_out = in_a + (in_a & ~in_b); break;
                        case 0x5: expected_out = (in_a + in_b) + (in_a & ~in_b); break;
                        case 0x6: expected_out = in_a - in_b - 1; break;
                        case 0x7: expected_out = (in_a & ~in_b) - 1; break;
                        case 0x8: expected_out = in_a + (in_a & in_b); break;
                        case 0x9: expected_out = in_a + in_b; break;
                        case 0xA: expected_out = (in_a + ~in_b) + (in_a & in_b); break;
                        case 0xB: expected_out = (in_a & in_b) - 1; break;
                        case 0xC: expected_out = in_a + in_a; break;
                        case 0xD: expected_out = (in_a + in_b) + in_a; break;
                        case 0xE: expected_out = (in_a + ~in_b) + in_a; break;
                        case 0xF: expected_out = in_a - 1; break;
                    }
                } else {  // Logic mode
                    switch (select) {
                        case 0x0: expected_out = ~in_a; break;
                        case 0x1: expected_out = ~(in_a | in_b); break;
                        case 0x2: expected_out = ~in_a & in_b; break;
                        case 0x3: expected_out = 0; break; // Logical 0
                        case 0x4: expected_out = ~(in_a & in_b); break;
                        case 0x5: expected_out = ~in_b; break;
                        case 0x6: expected_out = in_a ^ in_b; break;
                        case 0x7: expected_out = in_a & ~in_b; break;
                        case 0x8: expected_out = ~in_a | in_b; break;
                        case 0x9: expected_out = ~(in_a ^ in_b); break;
                        case 0xA: expected_out = in_b; break;
                        case 0xB: expected_out = in_a & in_b; break;
                        case 0xC: expected_out = 0xFFFF; break; // Logical 1
                        case 0xD: expected_out = in_a | ~in_b; break;
                        case 0xE: expected_out = in_a | in_b; break;
                        case 0xF: expected_out = in_a; break;
                    }
                }

                // Validate output
                if (alu_out == expected_out && compare == expected_compare) {
                    pass_count++;
                } else {
                    std::cerr << "Test failed: "
                              << "Mode=" << mode << " Select=" << std::hex << (int)select
                              << " A=" << std::hex << in_a << " B=" << std::hex << in_b
                              << " Expected=" << std::hex << expected_out
                              << " Got=" << std::hex << alu_out << std::endl;
                }
            }

            // Print summary for each operation
            std::cout << "Mode: " << mode << " Select: " << std::hex << select
                      << " Passed: " << std::dec << pass_count << "/" << NUM_TESTS << std::endl;
        }
    }

    // Cleanup
    delete alu;
    std::cout << "ALU verification completed.\n";
    return 0;
}
