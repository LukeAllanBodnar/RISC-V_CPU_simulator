#include "hex.h"
#include <iomanip>
#include <string>
#include <iostream>


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

using std::string;

string  hex::to_hex0x20(uint32_t i) {

	std::ostringstream os;
	os << std::setfill('0') << std::setw(5) << std::hex << i;
	return std::string("0x")+os.str();

}

string  hex::to_hex0x12(uint32_t i) {

	std::ostringstream os;
	os << std::setfill('0') << std::setw(3) << std::hex << (i & 0xfff);
	return std::string("0x")+os.str();

}

string hex::to_hex8(uint8_t i) {

		std::ostringstream os;
		os << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint16_t>(i);
		return os.str();

}

string hex::to_hex32(uint32_t i) {

	std::ostringstream os;
	os << std::setfill('0') << std::setw(8) << std::hex << i;
	return os.str();

}

string hex::to_hex0x32(uint32_t i) {

	return std::string("0x")+to_hex32(i);

}