#pragma once
#include "hex.h"
#include <iomanip>
#include <string>
#include <iostream>
using namespace std;

class memory : public hex 
{

public:

	/**
	* @brief The vector to hold all our 8 bit data
	* @parem The vector is made to simulate memory  
	* @note will alwyas round up to a Double fullword boundry  
	* @note will fill in extra space aith a5
	*/ 
	memory(uint32_t s);
	

	/**
	* @brief
	* @parem Will delete the vector of memory 
	*/
	~memory();


	/**
	* @brief checkes to see if the address is legal
    * @parem Checks to see if the given adress it our of range of the vector
	* @return true if addr if with the boundries
    */
	bool check_illegal(uint32_t addr) const;

	/**
	* @brief reshapes the vector to fit the file
    * @parem resizes the vector to be a certian size
    * @note will alwayas round up to a Doubel fullword boundry
    * @note will fill in extra space aith a5
    */
	uint32_t get_size() const;

	/**
	* @defgroup get()'s will show a value at the address given  
	* read and return a value give at the address 
	* 
	* @parem addr is the address which lowest simulated memeory 
	*  address from it reads the least significant bit byte. 
    *
	* @return the value or values in little endian style 
	* 
	* @note if addr is out of boundry will show an error message 
	* 
	* @{
	*/
	uint8_t get8(uint32_t addr) const;     ///< returns a unsigned 8 bit value of memory 
	uint16_t get16(uint32_t addr) const;   ///< returns a unsigned 16 bit value of memory 
	uint32_t get32(uint32_t addr) const;   ///< returns a unsigned 32 bit value of memory 

	int32_t get8_sx(uint32_t addr) const;  ///< returns a signed 8 bit value of memory 
	int32_t get16_sx(uint32_t addr) const; ///< returns a signed 16 bit value of memory 
	int32_t get32_sx(uint32_t addr) const; ///< returns a signed 32 bit value of memory /**@}*/


    /**
	* @defgroup set()'s will set a value at the address given
	* read the address and replacce whatever value there with the value given 
	*
	* @parem addr is the address which lowest simulated memeory
	*  address from it reads the least significant bit byte.
	*
	* @note if addr is out of boundry will show an error message
	*
	* @{
	*/
    void set8(uint32_t addr, uint8_t val);   ///< set a 8 bit value of memory 
	void set16(uint32_t addr, uint16_t val); ///< set a 16 bit value of memory 
	void set32(uint32_t addr, uint32_t val); ///< set a 32 bit value of memory /**@}*/


	/**
	* @brief this method will dump the contens of the simulated memory
	* @parem It will go through a loop to read and print each hex 
	* @note I made it with two loops one for the hex address and one to print the charaters the those values represent  
	*/
	void dump() const;

	/**
    * @brief this method will load the file and load it into the vector 
    * @parem it checks each address to see if its too big for the vector 
    */
	bool load_file(const std::string & fname);


	/**
	* @brief this is the vector used to simulate memory 
	*/
 private:
	vector<uint8_t> mem;
 };
