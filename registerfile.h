#pragma once
#include "hex.h"

using namespace std;

class registerfile : public hex 
{

public:
    /**
    * @brief this method jsut resets registers 0-31
    * @parem The method to zero out the vector
    * @note there are 32 regs and reg zero will always be zero 
    * @note will fill all but reg zero with f0 
    */
	void reset();
    /**
    * @brief this method is so exec can put values in the regs
    * @parem will take a reg addr number and but val in for where r is pointing at
    */
	void set(uint32_t r, int32_t val);
    /**
    * @brief this method is so exec can access the regs
    */
	int32_t get(uint32_t r) const;
    /**
    * @brief this method is to dump the values stored in regs
    */
	void dump(const string& hdr) const;

private: 
    /**
    * @brief vector is out 0-31 regs in our RISC-V simulator
    */
	vector<uint32_t> regs;
};