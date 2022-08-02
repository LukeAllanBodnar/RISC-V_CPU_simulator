#include "rv32i_decode.h"
#include "memory.h"

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

using namespace std; 

string rv32i_decode::decode(uint32_t addr, uint32_t insn) {

	uint32_t func3 = get_funct3(insn);
	uint32_t func7 = get_funct7(insn);
	int32_t imm_i = get_imm_i(insn);
	int32_t shamt = get_rs2(insn);
	switch (insn) {
	
	case insn_ecall:			return render_ecall(insn);
	case insn_ebreak:           return render_ebreak(insn);
	
    default:
		switch (get_opcode(insn))
		{
		default: return render_illegal_insn(insn);
		case opcode_lui:		    return render_lui(insn);
		case opcode_auipc:		    return render_auipc(insn);
		case opcode_jal:			return render_jal(addr, insn);
		case opcode_jalr:			return render_jalr(insn);
		case opcode_btype:
			switch (func3)
			{
			default:			    return render_illegal_insn(insn);
			case funct3_beq:		return render_btype(addr, insn, "beq");
			case funct3_bne:		return render_btype(addr, insn, "bne");
			case funct3_blt:		return render_btype(addr, insn, "blt");
			case funct3_bge:		return render_btype(addr, insn, "bge");
			case funct3_bltu:		return render_btype(addr, insn, "bltu");
			case funct3_bgeu:		return render_btype(addr, insn, "bgeu");
			}
		case opcode_load_imm:
			switch (func3)
			{
			default:			    return render_illegal_insn(insn);
			case funct3_lb:			return render_itype_load(insn, "lb");
			case funct3_lh:			return render_itype_load(insn, "lh");
			case funct3_lw:			return render_itype_load(insn, "lw");
			case funct3_lbu:		return render_itype_load(insn, "lbu");
			case funct3_lhu:		return render_itype_load(insn, "lhu");
			}
		case opcode_stype:
			switch (func3)
			{
			default:			    return render_illegal_insn(insn);
			case funct3_sb:			return render_stype(insn, "sb");
			case funct3_sh:			return render_stype(insn, "sh");
			case funct3_sw:			return render_stype(insn, "sw");
			}
		case opcode_alu_imm:

			switch (func3)
			{
			default:				    return render_illegal_insn(insn);
			case funct3_add:			return render_itype_alu(insn, "addi", imm_i);
			case funct3_sll:			return render_itype_alu(insn, "slli", shamt);
			case funct3_slt:			return render_itype_alu(insn, "slti", imm_i);
			case funct3_sltu:			return render_itype_alu(insn, "sltiu", imm_i);
			case funct3_xor:			return render_itype_alu(insn, "xori", imm_i);
			case funct3_or:			    return render_itype_alu(insn, "ori", imm_i);
			case funct3_and:			return render_itype_alu(insn, "andi", imm_i);
			case funct3_srx:
				switch (func7)
				{
				default:				return render_illegal_insn(insn);
				case funct7_srl:		return render_itype_alu(insn, "srli", shamt);
				case funct7_sra:		return render_itype_alu(insn, "srai", shamt);
				}
			}

		case opcode_rtype:

			switch (func3)
			{
			default:			    return render_illegal_insn(insn);
			case funct3_add:
				switch (func7)
				{
				default:				return render_illegal_insn(insn);
				case funct7_add:		return render_rtype(insn, "add");
				case funct7_sub:		return render_rtype(insn, "sub");
				}
			case funct3_sll:			return render_rtype(insn, "sll");
			case funct3_slt:			return render_rtype(insn, "slt");
			case funct3_sltu:			return render_rtype(insn, "sltu");
			case funct3_xor:			return render_rtype(insn, "xor");
			case funct3_srx:
				switch (func7)
				{
				default:				return render_illegal_insn(insn);
				case funct7_srl:		return render_rtype(insn, "srl");
				case funct7_sra:		return render_rtype(insn, "sra");
				}
			case funct3_or:				return render_rtype(insn, "or");
			case funct3_and:			return render_rtype(insn, "and");
			}

		case opcode_system:

			switch (func3)
			{
			default:			    return render_illegal_insn(insn);

			case funct3_csrrw:      return render_csrrx(insn, "csrrw");
			case funct3_csrrs:		return render_csrrx(insn, "csrrs");
			case funct3_csrrc:		return render_csrrx(insn, "csrrw");
			case funct3_csrrwi:		return render_csrrxi(insn, "csrrwi");
			case funct3_csrrsi:		return render_csrrxi(insn, "csrrsi");
			case funct3_csrrci:		return render_csrrxi(insn, "csrrci");
			}
		}
	}
		assert(0 && "unrecognized opcode"); // It should be impossible to ever get here!
}
/// 

uint32_t rv32i_decode::get_opcode(uint32_t insn) {

	return (insn & 0x0000007f);

}

uint32_t rv32i_decode::get_rd(uint32_t insn) {

	uint32_t rd = (insn & 0x00000f80) >> 7;
	return rd;

}

uint32_t rv32i_decode::get_funct3(uint32_t insn) {

	uint32_t funct3 = ((insn >> 12) & 0x00000007);
	return funct3;

}

uint32_t rv32i_decode::get_rs1(uint32_t insn) {

	uint32_t rs1 = (( insn & 0x000f8000) >> 15 );
	return rs1;

}

uint32_t rv32i_decode::get_rs2(uint32_t insn) {

	uint32_t rs2 = ( (insn & 0x01f00000) >> 20 );
	return rs2;

}

uint32_t rv32i_decode::get_funct7(uint32_t insn) {

	uint32_t funct7 = ((insn & 0xfe000000) >> 25);
	return funct7;

}

int32_t rv32i_decode::get_imm_i(uint32_t insn) {

	int32_t imm_i = (insn >> 20 );
	if (insn & 0x80000000)
	imm_i |= 0xfffff000; // sign-extend the left
	return imm_i;

}

int32_t rv32i_decode::get_imm_u(uint32_t insn) {

	int32_t imm_u = (insn & 0xfffff000);
	return imm_u;

}

int32_t rv32i_decode::get_imm_b(uint32_t insn) {

	int32_t imm_b = ( (insn & 0x7e000000) >> 20);
	imm_b |= ((insn & 0x00000f00) >> 7);
	imm_b |= ((insn & 0x00000080) << 4);
	if (insn & 0x80000000)
	imm_b |= 0xfffff000; // sign-extend the left
	return imm_b;

}

int32_t rv32i_decode::get_imm_s(uint32_t insn) {

	int32_t imm_s = (insn & 0xfe000000) >> (25 - 5);
	imm_s |= (insn & 0x00000f80) >> (7 - 0);
	if (insn & 0x80000000)
	imm_s |= 0xfffff000; // sign-extend the left
	return imm_s;

}

int32_t rv32i_decode::get_imm_j(uint32_t insn) {

	int32_t imm_j = ((insn & 0x7fe00000) >> 20 );
	imm_j |= ( insn & 0x000ff000);
	imm_j |= ((insn & 0x00100000) >> 9);
	if (insn & 0x80000000) {
		imm_j |= 0xfff00000; // sign-extend the left
	}
	return imm_j;

} 

std::string rv32i_decode::render_illegal_insn(uint32_t insn) {
	
	(void)insn;
	return "ERROR: UNIMPLEMENTED INSTRUCTION";

}

std::string rv32i_decode::render_lui(uint32_t insn) {
	
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn);
	std::ostringstream os;
	os << render_mnemonic("lui") << render_reg(rd) << "," << hex::to_hex0x20((imm_u >> 12) & 0x0fffff);
	return os.str();

}

std::string rv32i_decode::render_auipc(uint32_t insn) {

	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn);
	std::ostringstream os;
	os << render_mnemonic("auipc") << render_reg(rd) << "," << hex::to_hex0x20(((imm_u) >> 12) & 0x0fffff);
	return os.str();

}

std::string rv32i_decode::render_jal(uint32_t addr, uint32_t insn) {

	uint32_t rd = get_rd(insn);
	int32_t imm_j = get_imm_j(insn);
	std::ostringstream os;
	os << render_mnemonic("jal") << render_reg(rd) << "," << hex::to_hex0x32((imm_j + addr));
	return os.str();

}

std::string rv32i_decode::render_jalr(uint32_t insn) {

	uint32_t rd = get_rd(insn);
	int32_t imm_i = get_imm_i(insn);
	int32_t rs1 = get_rs1(insn);
	std::ostringstream os;
	os << render_mnemonic("jalr") << render_reg(rd) << "," << render_base_disp(rs1, imm_i);
	return os.str();

}

std::string rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const char* mnemonic) {

		uint32_t rs1 = get_rs1(insn);
		uint32_t rs2 = get_rs2(insn);
		int32_t imm_b = get_imm_b(insn);
		
		std::ostringstream os;
		os << render_mnemonic(mnemonic) << render_reg(rs1) << "," << render_reg(rs2) << "," << hex::to_hex0x32(addr+imm_b);
		return os.str();

}

std::string rv32i_decode::render_itype_load(uint32_t insn, const char* mnemonic) {

	uint32_t rd = get_rd(insn);
	int32_t imm_i = get_imm_i(insn);
	uint32_t rs1 = get_rs1(insn);
	std::ostringstream os;

	os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_base_disp( rs1 , imm_i) ;
	return os.str();

}

std::string rv32i_decode::render_stype(uint32_t insn, const char* mnemonic) {

	uint32_t rs1 = get_rs1(insn);
	uint32_t imm_s = get_imm_s(insn);
	uint32_t rs2 = get_rs2(insn);
	std::ostringstream os;

	os << render_mnemonic(mnemonic) << render_reg(rs2) << "," << render_base_disp( rs1, imm_s);
	return os.str();

}

std::string rv32i_decode::render_itype_alu(uint32_t insn, const char* mnemonic, int32_t imm_i) {

	uint32_t rs1 = get_rs1(insn);
	uint32_t rd = get_rd(insn);

	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1) << "," << std::dec << imm_i;
	return os.str();

}

std::string rv32i_decode::render_rtype(uint32_t insn, const char* mnemonic) {

	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);

	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1) << "," << render_reg(rs2);
	return os.str();

}

std::string rv32i_decode::render_ecall(uint32_t insn) {

	if (insn == insn_ecall) {
		std::ostringstream os;
		os << "ecall";
		return os.str();
	}
	else return "broken";
}

std::string rv32i_decode::render_ebreak(uint32_t insn) {

	if (insn == insn_ebreak) {
		std::ostringstream os;
		os << "ebreak";
		return os.str();
	}
	else return "broken";

}

std::string rv32i_decode::render_csrrx(uint32_t insn, const char* mnemonic) {

	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t csr = get_imm_i(insn);

	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << "," << hex::to_hex0x12(csr) << "," << render_reg(rs1);
	return os.str();

}

std::string rv32i_decode::render_csrrxi(uint32_t insn, const char* mnemonic) {

	uint32_t rd = get_rd(insn);
	uint32_t zimm = get_rs1(insn);
	uint32_t csr = get_imm_i(insn);

	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << "," << hex::to_hex0x12(csr) << "," << std::dec << zimm;
	return os.str();


}

std::string rv32i_decode::render_reg(int r) {

	std::ostringstream os;
	os << "x" << r;
	return os.str();

}

std::string rv32i_decode::render_base_disp(uint32_t base, int32_t disp) {

	std::ostringstream os;
	os << std::dec << disp << "(" << render_reg(base) << ")";
	return os.str();

}

std::string rv32i_decode::render_mnemonic(const std::string& m) {

	std::ostringstream os;
	os << setw(8) << left << m;
	return os.str();

}

