#pragma once

#include "memory.h"
#include "registerfile.h"
#include "rv32i_decode.h"


using namespace std;

class rv32i_hart : public rv32i_decode
{
public:

    /**
    * @brief this is a constructor that inheritance all previous mems
    * @parem The vector is made to simulate memory
    * @note will always round up to a Double fullword boundry
    * @note will fill in extra space aith a5
    */
rv32i_hart(memory & m) : mem(m) { }
    /**
    * @brief this is a constructor that inheritance all previous mems
    * @parem The vector is made to simulate memory
    * @note will always round up to a Double fullword boundry
    * @note will fill in extra space aith a5
    */
void set_show_instructions(bool b) { show_instructions = b; }
    /**
    * @brief this function to on/off showing of instructions
    */
void set_show_registers(bool b) { show_registers = b; }
    /**
    * @brief this function to on/off showing of registers
    */
bool is_halted() const { return halt; }
    /**
    * @brief this function shows if the program should halt or not
    */
const std::string & get_halt_reason() const { return halt_reason; }
    /**
    * @brief this function returns the reason it halted
    */
uint64_t get_insn_counter() const { return insn_counter; }
    /**
    * @brief this function returns the total amount of insn's that happend
    */
void set_mhartid(int i) { mhartid = i; }
    /**
    * @brief this function returns mhartid 
    */
void tick(const string & hdr = "");
    /**
    * @brief this is a method that runs a insn at a time that also add to the insn counter
    * @parem takes a header for when it wants to do output
    * @note This will stop if halt ever becomes true 
    */
void dump(const string & hdr = "") const;
    /**
    * @brief this method shows the regs in the current cpu run 
    * @parem takes a hdr if you want to print it with one
    * @note will show it before the insn run's 
    */
void reset();
    /**
    * @brief this method reset the regs pc register, reasons and bool values that control the data being seen
    */

/**
* @defgroup these are exec to simulate RISC-V insn with a ostream to 
* print out what it is doing
*
* @note this was made to be used with a switch statment, tick() and run() in mind
*
* @{
*/

void exec_lui(uint32_t insn, std::ostream* pos);	     ///<  exec for lui
void exec_auipc(uint32_t insn, std::ostream* pos);       ///<  exec for auipc
void exec_jal(uint32_t insn, std::ostream* pos);         ///<  exec for jal  
void exec_jalr(uint32_t insn, std::ostream* pos);        ///<  exec for jalr
void exec_btype(uint32_t insn, std::ostream* pos);       ///<  exec for btype
void exec_itype_load(uint32_t insn, std::ostream* pos);  ///<  exec for itype_load
void exec_stype(uint32_t insn, std::ostream* pos);		 ///<  exec for stype
void exec_itype_alu(uint32_t insn, std::ostream* pos);	 ///<  exec for itype_alu
void exec_rtype(uint32_t insn, std::ostream* pos);	     ///<  exec for rtype	
void exec_ecall(uint32_t insn, std::ostream* pos);       ///<  exec for ecall	
void exec_ebreak(uint32_t insn, std::ostream* pos);      ///<  exec for ebreak
void exec_csrr(uint32_t insn, std::ostream* pos);	     ///<  exec for csrr /**@}*/		

 
bool show_instructions = { false };
   /**
   * @brief  bool method checks to if to show all insn
   */
bool show_registers = { false };
   /**
   * @brief bool method checks to if to show all registers 
   */
private:
static constexpr int instruction_width = 35;
void exec(uint32_t insn, std::ostream*);
void exec_illegal_insn(uint32_t insn, std::ostream*);

/**
* @defgroup these are various variables to simulate a RISC-V CPU
*
* @note this was made in mine to keep track of the 
*
* @{
*/

bool halt = { false };              ///<  bool to choose when to halt
string halt_reason = { " none " };  ///<  string to give reason for the halt

uint64_t insn_counter = { 0 };      ///<  uint64_t to keep track of how many insns paased through run
uint32_t pc = { 0 };                ///<  uint32_t  to represnt a RISC-V general purpose register 
uint32_t mhartid = { 0 };           ///<  uint32_t  to core that's running 

protected:
registerfile regs;                  ///<  call the  registers from regs 0x-0x31 registers in a RISC-V CPU 
memory & mem;                       ///<  call the  mem from memory to repsent or data  
};
