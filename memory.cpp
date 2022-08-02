#include "memory.h"
#include <cctype>
#include <iomanip>
#include <string>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
using std::ios;
 

//***************************************************************************
//
//   Name: Luke Allan Bodanr
//    ZID: Z1808328
// Course: CSCI 463
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************

memory::memory(uint32_t s) {
	
	s = (s + 15) & 0xfffffff0; // round the length up, mod-16

	mem.resize(s, 0xa5);	
}

memory::~memory()
{

}

bool memory::check_illegal(uint32_t addr) const{
	
	if ( addr >= mem.size()) {
		cerr << "WARNING: Address out of range: " << hex::to_hex0x32(addr) << endl;
		return true;
	} else {
	return false; 
	}
}

uint32_t memory::get_size() const {
	return mem.size();
}

uint8_t memory::get8(uint32_t addr) const {
	
	if (check_illegal(addr) == true) {
		return 0;
	}

	return mem.at(addr);

}

uint16_t memory::get16(uint32_t addr) const{
	uint32_t left;
	uint32_t right;

	right = get8(addr);
	left = get8(addr+1);

	return (left << 8) | (right & 0xff);
}

uint32_t memory::get32(uint32_t addr) const {
	uint32_t left;
	uint32_t right;

	right = get16(addr);
	left = get16(addr + 2);

	return (left << 16) | (right & 0xffff);

}

int32_t memory::get8_sx(uint32_t addr) const {
	uint32_t val = get8(addr);
	return val | (val & 0x00000080 ? 0xffffff00 : 0);
}

int32_t memory::get16_sx(uint32_t addr) const {
	uint32_t val = get16(addr);
	return val | (val & 0x00008000 ? 0xffff0000 : 0);
}

int32_t memory::get32_sx(uint32_t addr) const {
	uint32_t val = get32(addr);
	return val | (val & 0x00000000 ? 0x00000000 : 0);
}

void memory::set8(uint32_t addr, uint8_t val) {

	if (check_illegal(addr) == false) {
		mem.at(addr) = val;
	}
	else {
		mem.at(addr) = 0;
	}
	
} 

void memory::set16(uint32_t addr, uint16_t val) {
	
	uint16_t temp = val;
	
	temp >>= 8;

	uint8_t right = temp;
	uint8_t left = val & 0xff;

	set8(addr + 1,right);
	set8(addr,left);
}

void memory::set32(uint32_t addr, uint32_t val) {
	
	uint32_t temp = val;
	
	temp >>= 16;
	
	uint16_t right = temp;
	uint16_t left = val & 0xffff; 

    set16(addr+2, right);
	set16(addr, left);
}


void memory::dump() const {

	uint32_t L_loop = 0;     // These two intgers are used to make a loop the will count up to a double fullword boundry 
	uint32_t Tracker = 16;   

	for (uint32_t i = 0; i < mem.size(); ++i) {
		
		if (i % 16 == 0 && i > 0) {  // this loope will print out the 8-bitvalues as the ASCCI counter part
			
			cout << " *";
		
			while (L_loop < Tracker) {

				uint8_t ch = get8(L_loop);  

				ch = isprint(ch) ? ch : '.';

				cout << ch;
				
				L_loop++;
			
			}
			cout << "*" << endl;
			Tracker = L_loop + 16; //This will reset the loop and does the next set of 16 8-bit values
		}
		
		if (i % 16 == 0) {
			cout << hex::to_hex32(i) <<":";
		}
		
		if (((i+8)%16) == 0) {
			cout << " ";
		}

		cout << std::setw(3) << to_hex8(mem.at(i));

	}
	cout << " *";


	Tracker = L_loop + 16;

	while (L_loop < Tracker) {

		uint8_t ch = get8(L_loop);

		ch = isprint(ch) ? ch : '.';

		cout << ch;

		L_loop++;
	}
		cout << "*" << endl;
}

bool memory::load_file(const std::string& fname) {
	
	std::ifstream infile(fname, std::ios::in | std::ios::binary);
	

	if (!infile) {
		cerr << "Can’t open file ’testdata’ for reading." << endl;
		return false;
	}
		
	uint8_t i;
	infile >> std::noskipws;
	for (uint32_t addr = 0; infile >> i; ++addr) {
		
		if (check_illegal(addr) == false) {
			mem.at(addr) = i;
		}
		else {
			cerr << "Program too big." << endl;
			cerr << "Usage: rv32i [-m hex-mem-size] infile" << endl;
			cerr << "    -m specify memory size (default = 0x100)" << endl;
			exit(1);
		}

	}

	return true;
}
