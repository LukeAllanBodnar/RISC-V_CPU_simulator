#pragma once
#include <iomanip>
#include <string>
#include <iostream>
#include <sstream>        
#include <stdint.h>
#include <string>
#include <vector>
#include <cctype>
#include <iomanip>
#include <fstream> 
#include <cassert>
#include <unistd.h>


class hex {

public:
	/**
	* @defgroup to_hex()'s will read an unsigned interger  
	* and return a string 
	*
	* @parem The unsigned interge is a value from the given memory
	*  and prints it into a string 
	*
	* @return will print out the value 
	*
	* @{
	*/ 

	static std::string to_hex8(uint8_t i);      ///< will print out an 8 bit value 
	static std::string to_hex32(uint32_t i);    ///< will print out a 32 bit value 
	static std::string to_hex0x32(uint32_t i);  ///< will print out a 32 bit value with an Ox in the front /**@}*/

	static std::string to_hex0x20(uint32_t i);
	static std::string to_hex0x12(uint32_t i);

};

