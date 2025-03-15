#include <verilated.h>
#include <iostream>
#include <vector>
#include "Vlab10_FSM.h"

#define CLK_CYCLES 10  // Number of clock cycles per input

void tick(Vlab10_FSM* fsm) {
    fsm->clk = 0;
    fsm->eval();
    fsm->clk = 1;
    fsm->eval();
}

void reset_fsm(Vlab10_FSM* fsm) {
    fsm->reset = 1;
    tick(fsm);
    fsm->eval();
    fsm->reset = 0;
    tick(fsm);
    fsm->eval();
}

bool test_sequence(Vlab10_FSM* fsm, std::vector<int> inputs, bool expected_locked) {
    reset_fsm(fsm);

    std::cout << "Testing Sequence: ";
    for (int code : inputs) std::cout << code << " ";
    std::cout << "\n";

    for (size_t i = 0; i < inputs.size(); i++) {
        fsm->code_in = inputs[i];
        tick(fsm);
        std::cout << "  Step " << (i + 1) << " | Input: " << inputs[i] 
                  << " | FSM Locked: " << (int)fsm->locked << "\n";
    }

    bool result = (fsm->locked == expected_locked);

    if (!result) {
        std::cerr << "❌ Test FAILED: Expected Locked = " << expected_locked 
                  << ", Got Locked = " << (int)fsm->locked << "\n";
    } else {
        std::cout << "Test PASSED: Expected Locked = " << expected_locked 
                  << ", Got Locked = " << (int)fsm->locked << "\n";
    }

    return result;
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    Vlab10_FSM* fsm = new Vlab10_FSM;
    std::cout << "Starting FSM Testbench...\n";

    int pass_count = 0, test_count = 0;

    // Test cases
    pass_count += test_sequence(fsm, {3, 3, 5, 2, 5, 6}, 0); test_count++; // Correct sequence
    pass_count += test_sequence(fsm, {3, 3, 5, 9}, 1); test_count++;  // Incorrect input resets FSM
    pass_count += test_sequence(fsm, {3, 3, 5, 2}, 1); test_count++;  // Partial correct sequence
    pass_count += test_sequence(fsm, {1, 8, 6, 2, 3}, 1); test_count++; // Random incorrect input

    std::cout << "\nFSM Testbench Completed: " << pass_count << "/" << test_count << " tests passed.\n";

    delete fsm;
    return 0;
}
