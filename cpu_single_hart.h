#pragma once
#include "rv32i_hart.h"
#include "registerfile.h"


using namespace std;

class cpu_single_hart : public rv32i_hart {

public: 
	/**
    * @brief this is a constructor that inheritance all previous mems 
    * @parem The vector is made to simulate memory
    * @note will always round up to a Double fullword boundry
    * @note will fill in extra space aith a5
    */
	cpu_single_hart(memory& mem) : rv32i_hart(mem) {}
    /**
    * @brief this is a method that runs the simulated memory, to run RISC-V instructions 
    * @parem int that holds how many insn's  
    * @note will stop if it halt is true 
    */
	void run(uint64_t exec_limit);

};





