#include "rv32i_hart.h"

using namespace std;


void rv32i_hart::exec(uint32_t insn, std::ostream* pos)
{
	if (insn == insn_ecall) {
		exec_ecall(insn, pos);
	}
	else if (insn == insn_ebreak) {
		exec_ebreak(insn, pos);
	} else {

switch (get_opcode(insn))
	{
		default:				exec_illegal_insn(insn, pos); return;
		case opcode_lui:		exec_lui(insn, pos);  return;
		case opcode_auipc:		exec_auipc(insn, pos);  return;
		case opcode_jal:		exec_jal(insn, pos);  return;
		case opcode_jalr:		exec_jalr(insn, pos);  return;
		case opcode_btype:		exec_btype(insn, pos);  return;
	    case opcode_load_imm:	exec_itype_load(insn, pos);  return;
		case opcode_stype:      exec_stype(insn, pos); return;
		case opcode_alu_imm:    exec_itype_alu(insn, pos);  return;
		case opcode_rtype:      exec_rtype(insn, pos); return;
		case opcode_system:     exec_csrr(insn, pos); return;  
	}

	}

}

void rv32i_hart::exec_lui(uint32_t insn, ostream* pos) {

	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn);

	if (pos)
	{
		string s = render_lui(insn);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(imm_u);
	}
	regs.set(rd, imm_u);
	pc += 4;

}

void rv32i_hart::exec_auipc(uint32_t insn, ostream* pos) {

	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn);

	if (pos)
	{
		string s = render_auipc(insn);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(pc) << " + " << hex::to_hex0x32(imm_u) << " = " << hex::to_hex0x32(pc + imm_u);
	}
	regs.set(rd, (imm_u+pc));
	pc += 4;

}

void rv32i_hart::exec_jal(uint32_t insn, ostream* pos) {


	uint32_t rd = get_rd(insn);
	regs.set(rd, (pc + 4));
	int32_t imm_j = get_imm_j(insn);

	if (pos)
	{
		string s = render_jal(pc,insn);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(pc+4) << ",  pc = " << hex::to_hex0x32(pc) << " + " << hex::to_hex0x32(imm_j) << " = " << hex::to_hex0x32(pc + imm_j);
	}

	pc += imm_j;

}

void rv32i_hart::exec_jalr(uint32_t insn, ostream* pos) {

	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	int32_t imm_i = get_imm_i(insn);
	int32_t temp = pc + 4;

	if (pos)
	{
		string s = render_jalr(insn);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(pc + 4) << ",  pc = (" << hex::to_hex0x32(imm_i) << " + " << hex::to_hex0x32(regs.get(rs1)) << ") & 0xfffffffe = " << hex::to_hex0x32(regs.get(rs1) + imm_i);
	}
	pc = ((imm_i+regs.get(rs1))&0xfffffffe);
	regs.set(rd, temp);

}

void rv32i_hart::exec_btype(uint32_t insn, ostream* pos) {

	uint32_t func3 = get_funct3(insn);
	int32_t imm_b = get_imm_b(insn);
	bool add = false;
	uint32_t track = 4;
	string op;
	int32_t rs1 = get_rs1(insn);
	int32_t rs2 = get_rs2(insn);
	uint32_t U_rs1 = (uint32_t)regs.get(rs1);
	uint32_t U_rs2 = (uint32_t)regs.get(rs2);

	switch (func3)
	{
	default:     
		render_illegal_insn(insn);

		break;
	case funct3_beq:	
		
		if (regs.get(rs1) == regs.get(rs2)) {
			add = true;
		}
		op = " == ";
		break;

	case funct3_bne:		
		
		if (regs.get(rs1) != regs.get(rs2)) {
			add = true;
		}
		op = " != ";
		break;

	case funct3_blt:	
		
		if (regs.get(rs1) < regs.get(rs2)) {
			add = true;
		}
		op = " < ";
		break;

	case funct3_bge:		
		
		if (regs.get(rs1) >= regs.get(rs2)) {
			add = true;
		}
		op = " >= ";
		break;

	case funct3_bltu:		
		
		if (U_rs1 < U_rs2) {
			add = true;
		}
		op = " <U ";
		break;

	case funct3_bgeu:		
		
		if (U_rs1 >= U_rs2) {
			add = true;
		}
		op = " >=U ";
		break;

	}

	if (add == true) {
		pc += imm_b;
		track = imm_b;

	} else if (add == false) {
		pc += 4;
	}

	string s;
	if (pos)
	{
		switch (func3)
		{
		default:			    render_illegal_insn(insn);
			break;
		case funct3_beq:		s = render_btype(pc-track, insn, "beq");
			break;
		case funct3_bne:		s = render_btype(pc-track, insn, "bne");
			break;
		case funct3_blt:		s = render_btype(pc-track, insn, "blt");
			break;
		case funct3_bge:		s = render_btype(pc-track, insn, "bge");
			break;
		case funct3_bltu:		s = render_btype(pc-track, insn, "bltu");
			break;
		case funct3_bgeu:		s = render_btype(pc-track, insn, "bgeu");
			break;
		}
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << op << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(pc);
	}
}

void rv32i_hart::exec_itype_load(uint32_t insn, ostream* pos) {

	uint32_t func3 = get_funct3(insn);
	int32_t imm_i = get_imm_i(insn);
	int32_t rd = get_rd(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t temp = 0;

	string op;

	switch (func3)
	{
	default:
		render_illegal_insn(insn);

		break;
	case funct3_lb:

		temp = mem.get8_sx(regs.get(rs1) + imm_i);
		op = "sx(m8(";

		break;

	case funct3_lh:

		temp = mem.get16_sx(regs.get(rs1) + imm_i);
		op = "sx(m16(";

		break;

	case funct3_lw:

		temp = mem.get32_sx(regs.get(rs1) + imm_i);
		op = "sx(m32(";

		break;

	case funct3_lbu:

		temp = mem.get8(regs.get(rs1) + imm_i);
		op = "zx(m8(";

		break;

	case funct3_lhu:

		temp = mem.get16(regs.get(rs1) + imm_i);
		op = "zx(m16(";
		
		break;

	}

	string s;
	if (pos)
	{
		switch (func3)
		{
		default:			  s = render_illegal_insn(insn);
			break;
		case funct3_lb:		  s = render_itype_load(insn, "lb");
			break;
		case funct3_lh:	      s = render_itype_load(insn, "lh");
			break;
		case funct3_lw:		  s = render_itype_load(insn, "lw");
			break;
		case funct3_lbu:	  s = render_itype_load(insn, "lbu");
			break;
		case funct3_lhu:	  s = render_itype_load(insn, "lhu");
			break;
		}
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << op << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(temp);
	}

	regs.set(rd, temp);

	pc += 4;

}

void rv32i_hart::exec_stype(uint32_t insn, ostream* pos) {

	uint32_t func3 = get_funct3(insn);
	int32_t imm_s = get_imm_s(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t rs2 = get_rs2(insn);
	int32_t temp = 0;
	uint32_t addr = 0;

	string op;

	switch (func3)
	{
	default:
		render_illegal_insn(insn);

		break;
	case funct3_sb:

		addr = regs.get(rs1) + imm_s;
		temp = 0x000000ff&regs.get(rs2);
		op = "m8(";

		break;

	case funct3_sh:

		addr = regs.get(rs1) + imm_s;
		temp = 0x0000ffff&regs.get(rs2);
		op = "m16(";

		break;

	case funct3_sw:

		addr = regs.get(rs1) + imm_s;
		temp = regs.get(rs2);
		op = "m32(";

		break;

	}

	string s;
	if (pos)
	{
		switch (func3)
		{
		default:			    s = render_illegal_insn(insn);
			break;
		case funct3_sb:			s = render_stype(insn, "sb");
			break;
		case funct3_sh:			s = render_stype(insn, "sh");
			break;
		case funct3_sw:			s = render_stype(insn, "sw");
			break;
		}
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << op << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_s) << ") = " << hex::to_hex0x32(temp);
	}

	switch (func3)
	{
	default:			    mem.set8(0 , 0);
		break;
	case funct3_sb:			mem.set8(addr, temp);
		break;
	case funct3_sh:			mem.set16(addr, temp);
		break;
	case funct3_sw:			mem.set32(addr, temp);
		break;
	}

	pc += 4;

}


void rv32i_hart::exec_itype_alu(uint32_t insn, ostream* pos) {

	uint32_t func3 = get_funct3(insn);
	uint32_t func7 = get_funct7(insn);
	int32_t imm_i = get_imm_i(insn);
	int32_t shamt = get_rs2(insn);
	int32_t rd = get_rd(insn);
	int32_t rs1 = get_rs1(insn);
	uint32_t U_rs1 = (uint32_t)regs.get(rs1);
	uint32_t U_imm_i = (uint32_t)get_imm_i(insn);
	string op;
	int32_t temp = 0;

	switch (func3)
	{
	default:				     render_illegal_insn(insn);
		break;
	case funct3_add:			

		temp = regs.get(rs1) + imm_i;
		op = " + ";

		break;
	case funct3_sll:			

		temp = regs.get(rs1) << shamt;
		op = " << ";

		break;
	case funct3_slt:		

		if (regs.get(rs1) < imm_i) {
			temp = 1;
		}
		else {
			temp = 0;
		}
		op = " < ";

		break;
	case funct3_sltu:			

		if (U_rs1 < U_imm_i) {
			temp = 1;
		}
		else {
			temp = 0;
		}
		op = " <U ";

		break;
	case funct3_xor:			

		temp = regs.get(rs1) ^ imm_i;
		op = " ^ ";

		break;
	case funct3_or:			     

		temp = regs.get(rs1) | imm_i;
		op = " | ";

		break;
	case funct3_and:			

		temp = regs.get(rs1) & imm_i;
		op = " & ";

		break;
	case funct3_srx:
		op = " >> ";

		switch (func7)
		{
		default:				 render_illegal_insn(insn);
			break;
		case funct7_srl:		 

			if (regs.get(rs1) & 0x80000000) {
				temp = (((0xffffffff << shamt) ^ 0xffffffff) << (32 - shamt));
				temp = (regs.get(rs1) >> shamt) ^ temp;
			}
			else {
				temp = regs.get(rs1) >> shamt;
			}

			break;
		case funct7_sra:		 

			temp = regs.get(rs1) >> shamt;

			break;
		}
		break;
	}

	string s;
	if (pos)
	{
		switch (func3)
		{
		default:				     s = render_illegal_insn(insn);
			break;
		case funct3_add:			 s = render_itype_alu(insn, "addi", imm_i);
			break;
		case funct3_sll:			 s = render_itype_alu(insn, "slli", shamt);
			break;
		case funct3_slt:			 s = render_itype_alu(insn, "slti", imm_i);
			break;
		case funct3_sltu:			 s = render_itype_alu(insn, "sltiu", imm_i);
			break;
		case funct3_xor:			 s = render_itype_alu(insn, "xori", imm_i);
			break;
		case funct3_or:			     s = render_itype_alu(insn, "ori", imm_i);
			break;
		case funct3_and:			 s = render_itype_alu(insn, "andi", imm_i);
			break;
		case funct3_srx:
			switch (func7)
			{
			default:				 s = render_illegal_insn(insn);
				break;
			case funct7_srl:		 s = render_itype_alu(insn, "srli", shamt);
				break;
			case funct7_sra:		 s = render_itype_alu(insn, "srai", shamt);
				break;
			}
			break;
		}
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		if ((func3 == funct3_slt) || (func3 == funct3_sltu)) {
			*pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << op << dec << imm_i << ") ? 1 : 0 = " << hex::to_hex0x32(temp);
		}
		else if ((func3 == funct3_srx) || (func3 == funct3_sll)) {
			*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << op << dec << shamt << " = " << hex::to_hex0x32(temp);
		} 
		else {
			*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << op << hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(temp);
		}
	}

	regs.set(rd, temp);

	pc += 4;

}

void rv32i_hart::exec_rtype(uint32_t insn, ostream* pos) {

	uint32_t func3 = get_funct3(insn);
	uint32_t func7 = get_funct7(insn);
	int32_t rd = get_rd(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t rs2 = get_rs2(insn);
	uint32_t U_rs1 = (uint32_t)regs.get(rs1);
	uint32_t U_rs2 = (uint32_t)regs.get(rs2);
	string op;
	int32_t temp = 0;

	switch (func3)
	{
	default:				     render_illegal_insn(insn);
		break;
	case funct3_add:

		switch (func7)
		{
		default:				temp = 0;;
			break;
		case funct7_add:		

			temp = regs.get(rs1) + regs.get(rs2);
			op = " + ";

			break;
		case funct7_sub:		

			temp = regs.get(rs1) - regs.get(rs2);
			op = " - ";

			break;
		}
		break;
	case funct3_sll:

		temp = regs.get(rs1) << (regs.get(rs2) & 0x0000001f);
		op = " << ";

		break;
	case funct3_slt:

		if (regs.get(rs1) < regs.get(rs2)) {
			temp = 1;
		}
		else {
			temp = 0;
		}
		op = " < ";

		break;
	case funct3_sltu:

		if (U_rs1 < U_rs2) {
			temp = 1;
		}
		else {
			temp = 0;
		}
		op = " <U ";

		break;
	case funct3_xor:

		temp = regs.get(rs1) ^ regs.get(rs2);
		op = " ^ ";

		break;
	case funct3_or:

		temp = regs.get(rs1) | regs.get(rs2);
		op = " | ";

		break;
	case funct3_and:

		temp = regs.get(rs1) & regs.get(rs2);
		op = " & ";

		break;
	case funct3_srx:
		op = " >> ";

		switch (func7)
		{
		default:				 render_illegal_insn(insn);
			break;
		case funct7_srl:


			if (regs.get(rs1) & 0x80000000) {
				temp = (((0xffffffff << (regs.get(rs2) & 0x0000001f)) ^ 0xffffffff) << (32 - (regs.get(rs2) & 0x0000001f)));
				temp = (regs.get(rs1) >> (regs.get(rs2) & 0x0000001f)) ^ temp;
			}
			else {
				temp = regs.get(rs1) >> (regs.get(rs2) & 0x0000001f);
			}
			break;
		case funct7_sra:

			temp = regs.get(rs1) >> (regs.get(rs2) & 0x0000001f);

			break;
		}
		break;
	}

	string s;
	if (pos)
	{
		switch (func3)
		{
		default:			    
			halt = true;
			halt_reason = "Illegal instruction";
			break;
		case funct3_add:
			switch (func7)
			{
			default:				s = render_illegal_insn(insn);
				break;
			case funct7_add:		s = render_rtype(insn, "add");
				break;
			case funct7_sub:		s = render_rtype(insn, "sub");
				break;
			}
			break;
		case funct3_sll:			s = render_rtype(insn, "sll");
			break;
		case funct3_slt:			s = render_rtype(insn, "slt");
			break;
		case funct3_sltu:			s = render_rtype(insn, "sltu");
			break;
		case funct3_xor:			s = render_rtype(insn, "xor");
			break;
		case funct3_srx:

			switch (func7)
			{
			default:				s = render_illegal_insn(insn);
				break;
			case funct7_srl:		s = render_rtype(insn, "srl");
				break;
			case funct7_sra:		s = render_rtype(insn, "sra");
				break;
			}
			break;
		case funct3_or:				s = render_rtype(insn, "or");
			break;
		case funct3_and:			s = render_rtype(insn, "and");
			break;
		}
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		if ((func3 == funct3_slt) || (func3 == funct3_sltu)) {
			*pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << op << hex::to_hex0x32(regs.get(rs2)) << ") ? 1 : 0 = " << hex::to_hex0x32(temp);
		}
		else if ((func3 == funct3_srx) || (func3 == funct3_sll)) {
			*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << op << dec << (regs.get(rs2) & 0x0000001f) << " = " << hex::to_hex0x32(temp);
		}
		else {
			*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << op << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(temp);
		}
	}

	regs.set(rd, temp);

	pc += 4;

}

void rv32i_hart::exec_ecall(uint32_t insn, ostream* pos) {

	halt = true;
	halt_reason = "ECALL instruction";
	string s;
	s = render_ecall(insn);
	*pos << s;
}

void rv32i_hart::exec_ebreak(uint32_t insn, ostream* pos) {

	halt = true;
	halt_reason = "EBREAK instruction";
	string s;
	s = render_ebreak(insn);
	*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
	*pos << "// " << "HALT" << endl;
}

void rv32i_hart::exec_csrr(uint32_t insn, ostream* pos) {

	uint32_t func3 = get_funct3(insn);
	int32_t rd = get_rd(insn);
	string s;

	switch (func3)
	{
	default:			   
	
		halt = true;
		halt_reason = "Illegal CSR in CRRSS instruction";

		break;

	case funct3_csrrs:		
		s = render_csrrx(insn, "csrrs");
		if (pos)
		{
			*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
			*pos << "// " << render_reg(rd) << " = " << "0" << endl;
		}
		regs.set(rd, mhartid);
		break;
	}
	pc += 4;
}

void rv32i_hart::tick(const string& hdr)
{
	++insn_counter;
	int32_t insn = mem.get32(pc);


	if (show_registers == true) {

		rv32i_hart::dump(hdr);

	}

	if (show_instructions == true) {

		cout << hex::to_hex32(pc) << ": " << std::setw(10) << std::left << hex::to_hex32(mem.get32(pc));
		exec(insn, &cout );
		cout << endl;

	} else {

		exec(insn, nullptr);

	}

	if (halt == true) {
		return;
	}

}

void rv32i_hart::dump(const string& hdr) const {

	regs.dump(hdr);
	cout << endl;
	cout << hdr << " pc " << hex::to_hex32(pc) << endl;

}

void rv32i_hart::reset() {
	
	regs.reset();
	pc = 0;
	insn_counter = 0;
	halt = false;
	halt_reason = "none";

}

void rv32i_hart::exec_illegal_insn(uint32_t insn, ostream* pos) {

	(void)insn;
	if (pos) 
	* pos << render_illegal_insn(insn);
	halt = true;
    halt_reason = " Illegal instruction ";

}