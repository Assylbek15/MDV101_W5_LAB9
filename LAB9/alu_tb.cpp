#include "Valu.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdint>

#define M 65536

int main() {
    srand(time(NULL));
    Valu* dut = new Valu();
    dut->mode = 0;

    for (int sel = 0; sel < 16; sel++) {
        dut->select = sel;
        for (int i = 0; i < 10000; i++) {
            for (uint16_t car = 0; car < 2; car++) {
                uint16_t a = std::rand() % M;
                uint16_t b = std::rand() % M;
                uint32_t sum = 0;

                dut->in_a = a;
                dut->in_b = b;
                dut->carry_in = car;
                dut->eval();

                uint16_t expout = 0; // output
                bool expcar = 0; // carry out
                
                 //your code here
                 switch (sel) {
                    case 0x0: expout = a; break;
                    case 0x1: expout = a + b; expcar = car; break;
                    case 0x2: expout = a + (~b); expcar = car; break;
                    case 0x3: expout = 0xFFFF; break;
                    case 0x4: expout = a + (a & ~b); break;
                    case 0x5: expout = (a + b) + (a & ~b); expcar = car; break;
                    case 0x6: expout = a - b - 1; break;
                    case 0x7: expout = (a & ~b) - 1; break;
                    case 0x8: expout = a + (a & b); break;
                    case 0x9: expout = a + b; expcar = car; break;
                    case 0xA: expout = (a + (~b)) + (a & b); break;
                    case 0xB: expout = (a & b) - 1; break;
                    case 0xC: expout = a + a; break;
                    case 0xD: expout = (a + b) + a; expcar = car; break;
                    case 0xE: expout = (a + (~b)) + a; expcar = car; break;
                    case 0xF: expout = a - 1; break;
                }
                
                 
                if (expout != dut->alu_out)
                    printf("Arithmetic: Wrong Output sel:%d exp:%d dut:%d a:%d b:%d cin:%d\n", sel, expout, dut->alu_out, a, b, car);

                if (expcar != dut->carry_out)
                    printf("Arithmetic: Wrong Carry sel:%d exp:%d dut:%d a:%d b:%d cin:%d\n", sel, expcar, dut->carry_out, a, b, car);

                if ((a == b) != dut->compare)
                    printf("Arithmetic: Comparison error a:%d b:%d\n", a, b);
            }
        }
    }

    std::cout << "Arithmetic module is tested.\n";

    dut->mode = 1;
    for (int sel = 0; sel < 15; sel++) {
        dut->select = sel;
        for (int i = 0; i < 10000; i++) {
            uint16_t a = std::rand() % M;
            uint16_t b = std::rand() % M;
            dut->in_a = a;
            dut->in_b = b;
            dut->eval();

            uint16_t exp = 0; // output
            
            //your code here
            switch (sel) {
                case 0x0: exp = ~a; break;
                case 0x1: exp = ~(a | b); break;
                case 0x2: exp = (~a) & b; break;
                case 0x3: exp = 0; break;
                case 0x4: exp = ~(a & b); break;
                case 0x5: exp = ~b; break;
                case 0x6: exp = a ^ b; break;
                case 0x7: exp = a & ~b; break;
                case 0x8: exp = ~a | b; break;
                case 0x9: exp = ~(a ^ b); break;
                case 0xA: exp = b; break;
                case 0xB: exp = a & b; break;
                case 0xC: exp = 0xFFFF; break;
                case 0xD: exp = a | ~b; break;
                case 0xE: exp = a | b; break;
                case 0xF: exp = a; break;
            }
            

            if (exp != dut->alu_out)
                printf("Logic: WA sel:%d exp:%d dut:%d a:%d b:%d\n", sel, exp, dut->alu_out, a, b);
        }
    }

    std::cout << "Logic module is tested\n";
    delete dut;
    return 0;
}