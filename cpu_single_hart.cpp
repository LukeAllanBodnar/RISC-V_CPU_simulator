#include "cpu_single_hart.h"

using namespace std;

void cpu_single_hart::run(uint64_t exec_limit) {

    regs.set(2, mem.get_size());

    while (rv32i_hart::is_halted() == false && (exec_limit == 0 || exec_limit > get_insn_counter()) ) {

        rv32i_hart::tick();

    }

    cout << "Execution terminated. Reason: " << get_halt_reason() << endl;

}