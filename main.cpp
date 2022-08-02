#include "hex.h"
#include "memory.h"
#include "rv32i_decode.h"
#include "rv32i_hart.h"
#include "cpu_single_hart.h"
#include <unistd.h>
#include <iomanip>
#include <string>
#include <iostream>


//***************************************************************************
//
//   Name: Luke Allan Bodnar
//    ZID: Z1808328
// Course: CSCI 463
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************

/*

g++ -pedantic -Wall -Wextra -g -std=c++14 -c -o main.o main.cpp
g++ -pedantic -Wall -Wextra -g -std=c++14 -c -o memory.o memory.cpp
g++ -pedantic -Wall -Wextra -g -std=c++14 -c -o hex.o hex.cpp
g++ -pedantic -Wall -Wextra -g -std=c++14 -c -o rv32i_decode.o rv32i_decode.cpp
g++ -pedantic -Wall -Wextra -g -std=c++14 -c -o registerfile.o registerfile.cpp
g++ -pedantic -Wall -Wextra -g -std=c++14 -c -o rv32i_hart.o rv32i_hart.cpp
g++ -pedantic -Wall -Wextra -g -std=c++14 -c -o cpu_single_hart.o cpu_single_hart.cpp
g++ -pedantic -Wall -Wextra -g -std=c++14 -o memsim main.o memory.o hex.o rv32i_decode.o registerfile.o rv32i_hart.o cpu_single_hart.o

*/

using std::string;
using std::cerr;
using std::cout;
/**
* @brief this method is so we can take a look at what insns our in memory before we start executing them 
* @parem will take out vector mem and read it 
* @note will read at every byte instead of reading it one at a time
*/
static void disassemble(const memory& mem)
{

	for (uint32_t i = 0; i < mem.get_size(); i+=4) {

		
		cout << hex::to_hex32(i) << ": " << std::setw(10) << std::left << hex::to_hex32(mem.get32(i)) << rv32i_decode::decode(i, mem.get32(i)) << endl;


	}

}
/**
* @brief this method is give what options we have to do with our memory and how to read in and how much memory we want 
* @note will give diffrent options to display information before during and after executing insn's. 
*/
static void usage()
{
	cerr << "Usage: rv32i [-d] [-i] [-r] [-z] [-l exec-limit] [-m hex-mem-size] infile" << endl;
	cerr << "-d show disassembly before program execution" << endl;
	cerr << "-i show instruction printing during execution" << endl;
	cerr << "-l maximum number of instructions to exec" << endl;
	cerr << "-m specify memory size (default = 0x100)" << endl;
	cerr << "-r show register printing during execution" << endl;
	cerr << "-z show a dump of the regs & memory after simulation" << endl;
	exit(1);
}
/**
* @brief out main method that allows to put what options we want and call run to see and exec the insn and information realting to such things
*/
int main(int argc, char** argv)
{
	uint32_t memory_limit = 0x100;	// default memory size is 0x100
	int insn_limit = 0;

	bool d = false;
	bool i = false;
	bool r = false;
	bool z = false;

	int opt;
	while ((opt = getopt(argc, argv, "m:dil:rz")) != -1)
	{
		switch (opt)
		{
		case 'm':
		{
			std::istringstream iss(optarg);
			iss >> std::hex >> memory_limit;
		}
		continue;
		case 'd':
		{
			d = true;
		}
		continue;
		
		case 'i':
		{   
			i = true;
		}
		continue;
		case 'l':
		{
			std::istringstream isz(optarg);
			isz >> insn_limit;
		}
		continue;

		case 'r':
		{
			r = true;
		}
		continue;
		case 'z':
		{
			z = true;
		}
		continue;
		
		default: /* ’?’ */
			usage();
		}
	}

	if (optind >= argc)
		usage();	// missing filename

	memory mem(memory_limit);

	if (!mem.load_file(argv[optind])) {
		usage();
	}

	cpu_single_hart cpu(mem);
	cpu.reset();

	if (i == true) {

		cpu.set_show_instructions(true);

	}
	if (r == true) {

		cpu.set_show_registers(true);

	}
	if (d == true) {

		disassemble(mem);

	}

	cpu.run(insn_limit);
	cout << cpu.get_insn_counter() << " instructions executed" << endl;

	if (z == true) {

		cpu.dump("");
		mem.dump();

	}

	return 0;
}