#pragma once
#include "hex.h"


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

class rv32i_decode : public hex 
{

public:

	///@parm addr The memory address where the insn is stored.
	static std::string decode(uint32_t addr, uint32_t insn);

protected:
	static constexpr int mnemonic_width = 8;

	/**
	* @defgroup opcodes are binary literals repsenting the opcodes of
	* the RISCV CPU instruction set
	*
	* @note this was made to be used with a switch statment
	*
	* @{
	*/
	static constexpr uint32_t opcode_lui = 0b0110111;     ///<  Binary literal for lui
	static constexpr uint32_t opcode_auipc = 0b0010111;	  ///<  Binary literal for auipic
	static constexpr uint32_t opcode_jal = 0b1101111;     ///<  Binary literal for jal 
	static constexpr uint32_t opcode_jalr = 0b1100111;    ///<  Binary literal for jalr
	static constexpr uint32_t opcode_btype = 0b1100011;   ///<  Binary literal for all B type intructions 
	static constexpr uint32_t opcode_load_imm = 0b0000011;///<  Binary literal for all Load I instructions 
	static constexpr uint32_t opcode_stype = 0b0100011;   ///<  Binary literal for S type instructions 
	static constexpr uint32_t opcode_alu_imm = 0b0010011; ///<  Binary literal for all Arithmetic and Logic instructions for type I
	static constexpr uint32_t opcode_rtype = 0b0110011;   ///<  Binary literal for R type Arithmetic and Logic instructions
	static constexpr uint32_t opcode_system = 0b1110011;  ///<  Binary literal for I type shift intstructions  /**@}*/
	/**
	* @defgroup get()'s opcode are binary litarals repsenting the func3's of certain opcodes of
	* the RISCV CPU instruction set
	*
	* @note this was made for the switch statments 
	*
	* @{
	*/
	static constexpr uint32_t funct3_beq = 0b000;		///<  Binary literal for func3 beq
	static constexpr uint32_t funct3_bne = 0b001;		///<  Binary literal for func3 bne
	static constexpr uint32_t funct3_blt = 0b100;		///<  Binary literal for func3 blt
	static constexpr uint32_t funct3_bge = 0b101;		///<  Binary literal for func3 bge
	static constexpr uint32_t funct3_bltu = 0b110;		///<  Binary literal for func3 bltu
	static constexpr uint32_t funct3_bgeu = 0b111;		///<  Binary literal for func3 bgeu
		
	static constexpr uint32_t funct3_lb = 0b000;		///<  Binary literal for func3 lb
	static constexpr uint32_t funct3_lh = 0b001;		///<  Binary literal for func3 lh
	static constexpr uint32_t funct3_lw = 0b010;		///<  Binary literal for func3 lw
	static constexpr uint32_t funct3_lbu = 0b100;		///<  Binary literal for func3 lbu
	static constexpr uint32_t funct3_lhu = 0b101;		///<  Binary literal for func3 lhu

	static constexpr uint32_t funct3_sb = 0b000;		///<  Binary literal for func3 sb
	static constexpr uint32_t funct3_sh = 0b001;		///<  Binary literal for func3 sh
	static constexpr uint32_t funct3_sw = 0b010;		///<  Binary literal for func3 sw

	static constexpr uint32_t funct3_add = 0b000;		///<  Binary literal for func3 add and subtract
	static constexpr uint32_t funct3_sll = 0b001;		///<  Binary literal for func3 sll
	static constexpr uint32_t funct3_slt = 0b010;		///<  Binary literal for func3 slt
	static constexpr uint32_t funct3_sltu = 0b011;		///<  Binary literal for func3 slty
	static constexpr uint32_t funct3_xor = 0b100;		///<  Binary literal for func3 xor
	static constexpr uint32_t funct3_srx = 0b101;		///<  Binary literal for func3 both srl and srx 
	static constexpr uint32_t funct3_or = 0b110;		///<  Binary literal for func3 or
	static constexpr uint32_t funct3_and = 0b111;		///<  Binary literal for func3 and 

	static constexpr uint32_t funct3_csrrw = 0b001;		///<  Binary literal for func3 csrrw
	static constexpr uint32_t funct3_csrrs = 0b010;		///<  Binary literal for func3 csrrs
	static constexpr uint32_t funct3_csrrc = 0b011;		///<  Binary literal for func3 csrrc
	static constexpr uint32_t funct3_csrrwi = 0b101;	///<  Binary literal for func3 csrrwi
	static constexpr uint32_t funct3_csrrsi = 0b110;	///<  Binary literal for func3 csrrsi
	static constexpr uint32_t funct3_csrrci = 0b111;	///<  Binary literal for func3 csrrci /**@}*/
	/**
	* @defgroup get()'s opcode are binary litarals repsenting the func7's of certain opcodes of
	* the RISCV CPU instruction set
	*
	* @note this was made for the switch statments
	*
	* @{
	*/
	static constexpr uint32_t funct7_srl = 0b0000000;		///<  Binary literal for func7 srl
	static constexpr uint32_t funct7_sra = 0b0100000;		///<  Binary literal for func7 sra

	static constexpr uint32_t funct7_add = 0b0000000;		///<  Binary literal for func7 add
	static constexpr uint32_t funct7_sub = 0b0100000;		///<  Binary literal for func7 sub 

	static constexpr uint32_t insn_ecall = 0x00000073;		///<  Binary literal for func7 ecall
	static constexpr uint32_t insn_ebreak = 0x00100073;		///<  Binary literal for func7   /**@}*/

	/**
	* @defgroup get()'s will are funtions that take and shift certian pieces of a 32-bit value instruction called insn so 
	* it can be used with junction of opcode's and func's to give the correct instruction
	* 
	* @parem insn is 32-bit value from memory at a certian adress 
	*  these values come form the document you read in. 
    *
	* @return certian pieces from the uint32_t insn
	* 
	* @{
	*/

	static uint32_t get_opcode(uint32_t insn);		///<  returns the opcode from insn
	static uint32_t get_rd(uint32_t insn);			///<  returns RD register 
	static uint32_t get_funct3(uint32_t insn);		///<  returns the funct3 in the insn
	static uint32_t get_rs1(uint32_t insn);			///<  returns rs1 register 
	static uint32_t get_rs2(uint32_t insn);			///<  returns rs2 register
	static uint32_t get_funct7(uint32_t insn);		///<  returns the func7 in the insn
	static int32_t get_imm_i(uint32_t insn);		///<  grabs the imm value for I type insn
	static int32_t get_imm_u(uint32_t insn);		///<  grabs the imm value for U type insn
	static int32_t get_imm_b(uint32_t insn);		///<  grabs the imm value for B type insn
	static int32_t get_imm_s(uint32_t insn);		///<  grabs the imm value for S type insn
	static int32_t get_imm_j(uint32_t insn);		///<  grabs the imm value for J type insn  /**@}*/

	static constexpr uint32_t XLEN = 32;
	/**
	* @defgroup render()'s will foramt the Data the same way the RISC-V CPU would do it. 
	*
	* @parem addr is the address which of the current insn
	*
	* @parem insn is the what is at the current 
	* 
	* @return the string that smilutes the style of RISC-V 
	* 
	* @note if addr is out of boundry will show an error message
	*
	* @{
	*/

	static std::string render_illegal_insn(uint32_t insn);										///<  returns the string for 32-value with insn with no purpose
	static std::string render_lui(uint32_t insn);												///<  returns the string for the lui insn
	static std::string render_auipc(uint32_t insn);												///<  returns the string for the auipc insn
	static std::string render_jal(uint32_t addr, uint32_t insn);								///<  returns the string for the jal insn
	static std::string render_jalr(uint32_t insn);												///<  returns the string for the jalr insn
	static std::string render_btype(uint32_t addr, uint32_t insn, const char* mnemonic);		///<  returns the string for all B type insn
	static std::string render_itype_load(uint32_t insn, const char* mnemonic);					///<  returns the string for all I type load insn
	static std::string render_stype(uint32_t insn, const char* mnemonic);						///<  returns the string for all S insn
	static std::string render_itype_alu(uint32_t insn, const char* mnemonic, int32_t imm_i);	///<  returns the string for all I type ALU insn
	static std::string render_rtype(uint32_t insn, const char* mnemonic);						///<  returns the string for all R type insn
	static std::string render_ecall(uint32_t insn);												///<  returns the string for ecall
	static std::string render_ebreak(uint32_t insn);											///<  returns the string for ebreak
	static std::string render_csrrx(uint32_t insn, const char* mnemonic);						///<  returns the string for crr type insn
	static std::string render_csrrxi(uint32_t insn, const char* mnemonic);						///<  returns the string for csrr_i type insn   

	static std::string render_reg(int r);														///<  used by the other render functions to format registers to match RISC-V
	static std::string render_base_disp(uint32_t base, int32_t disp);							///<  used by the other render functions to format base dispostions to match RISC-V
	static std::string render_mnemonic(const std::string& m);									///<  used by the other render functions to format mnemoic to match RISC-V  /**@}*/
};