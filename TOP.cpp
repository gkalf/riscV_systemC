#include "systemc.h"
#include "Fetch_Fetch.h"
#include "testbech_testbench.h"
// #include "Control_Unit_Control_Unit.h"
// #include "Register_File_Register_File.h"
// #include "instruction_decoder_instruction_decoder.h"
// #include "Mux_4-1_MUX_4_1.h"
// #include "MUX_2-1_MUX_2_1.h"
// #include "ALU_ALU.h"
// #include "DATA_mem_DATA_MEM.h"
// #include "immidiate_extend_sign_extend.h"
// #include "jump_branch_block_jump_branch_block.h"
// #include "WB.h"
#include "DUAL_PORT_MEMORY.h"


SC_MODULE(Theseus_CPU) 
{
	//Interface Ports...
	sc_signal<bool>clk;
	sc_signal<bool>	reset;

	//Internal Signals...
	// sc_signal<bool>	carry;
	// sc_signal<bool>	zero;
	// sc_signal<sc_uint<7> >	opcode;
	 sc_signal<sc_uint<3> >	funct3;
	// sc_signal<sc_uint<7> >	funct7;
	// sc_signal<bool>	rs1_en;
	// sc_signal<bool>	rs2_en;
	// sc_signal<sc_uint<5> >	ALU_code;
	// sc_signal<bool>	ALU_src;
	// sc_signal<bool>	Wr_to_RF;
	// sc_signal<bool>	Branch;
	// sc_signal<bool>	jump;
	// sc_signal<sc_uint<2> >	Imm_sel;
	sc_signal<bool>	MEM_write;
	// sc_signal<bool>	WB_select;
	// sc_signal<sc_uint<32> >	r_data;
	// sc_signal<sc_uint<32> >	ALU_result;
	sc_signal<sc_uint<32> >	PC;
	sc_signal<sc_uint<32> >	new_PC;
	sc_signal<sc_uint<32> >	next_PC;
	sc_signal<sc_uint<32> >	instruction;
	sc_signal<bool> take_branch;
	// sc_signal<sc_uint<5> >	rd;
	// sc_signal<sc_uint<5> >	rs1;
	// sc_signal<sc_uint<5> >	rs2;
	// sc_signal<sc_uint<32> >	rs1_data;
	// sc_signal<sc_uint<32> >	rs2_data;
	// sc_signal<sc_uint<32> >	rd_data;
	// sc_signal<sc_uint<32> >	I_imm;
	// sc_signal<sc_uint<32> >	S_imm;
	// sc_signal<sc_uint<32> >	U_imm;
	// sc_signal<sc_uint<32> >	B_imm;
	// sc_signal<sc_uint<32> >	J_imm;
	// sc_signal<bool>	ALU_A_src;
	// sc_signal<sc_uint<2> >	ALU_B_src;
	// sc_signal<bool>	jump_r;
	// sc_signal<sc_uint<32> >	immidiate;
	// sc_signal<sc_uint<32> >	immidiate_sign_extended;
	// sc_signal<sc_uint<32> >	ALU_inputA;
	// sc_signal<sc_uint<32> >	ALU_inputB;
	sc_signal<sc_uint<32> >	data_address;
	sc_signal<sc_uint<32> >	data_from_memory;
	sc_signal<sc_uint<32> > data_to_memory;
	sc_signal<sc_uint<32> >	instr_address;
	sc_signal<sc_uint<32> >	load_instruction;
	

	//Instantiated Modules Object Declaration...
	Fetch<32,15>	Fetch_Inst;
	// jump_branch_block<32>	next_PC_inst;
	// Control_Unit	Control_Unit_inst;
	// Register_File<32,5>	Register_File_inst;
	// instruction_decoder	instruction_decoder_inst;
	// MUX_4_1<32>	MUX_4_1_inst;
	// MUX_2_1<32>	ALU_A_src_inst;
	// MUX_4_1<32>	ALU_B_src_inst;
	// ALU<32>	ALU_inst;
	// DATA_MEM<32,8>	DATA_MEM_inst;
	// WB<32> WB_inst;
	RAM<> RAM_inst;
	testbench test;
	//Local Temporary Signals...

	//Signal Handler...
	

	//Constructor Declaration...
	SC_CTOR(Theseus_CPU) :
		test("testbench"),
		Fetch_Inst("FETCH_INST") ,
		// next_PC_inst("NEXT_PC_INST") ,
		// Control_Unit_inst("CONTROL_UNIT_INST") ,
		// Register_File_inst("REGISTER_FILE_INST") ,
		// instruction_decoder_inst("INSTRUCTION_DECODER_INST") ,
		// MUX_4_1_inst("MUX_4_1_INST") ,
		// ALU_A_src_inst("ALU_A_SRC_INST") ,
		// ALU_B_src_inst("ALU_B_SRC_INST") ,
		// ALU_inst("ALU_INST") ,
		// DATA_MEM_inst("DATA_MEM_INST") ,
		// WB_inst("WB_inst"),
		RAM_inst("RAM_inst")
	{
		test.clk(clk);
		test.reset(reset);

		Fetch_Inst.clk(clk);
		Fetch_Inst.reset(reset);
		Fetch_Inst.take_branch(take_branch);
		Fetch_Inst.next_PC(next_PC);
		Fetch_Inst.new_PC(new_PC);
		Fetch_Inst.index(instr_address);		
		
		// next_PC_inst(PC,new_PC,J_imm,B_imm,I_imm,next_PC,jump,jump_r,Branch,zero,ALU_result);

		// Control_Unit_inst(opcode,funct3,funct7,rs1_en,rs2_en,ALU_code,ALU_A_src,ALU_B_src,Wr_to_RF,jump,Branch,MEM_write,WB_select,Imm_sel,jump_r);

		// Register_File_inst(clk,Wr_to_RF,rs1_en,rs2_en,rd,rs1,rs2,rd_data,rs1_data,rs2_data);

		// instruction_decoder_inst(instruction,opcode,rs1,rs2,rd,funct3,funct7,I_imm,S_imm,B_imm,U_imm,J_imm);

		// MUX_4_1_inst(I_imm,S_imm,B_imm,J_imm,Imm_sel,immidiate);

		// ALU_A_src_inst(U_imm,rs1_data,ALU_A_src,ALU_inputA);

		// ALU_B_src_inst(rs2_data,immidiate,PC,U_imm,ALU_B_src,ALU_inputB);

		// ALU_inst(clk,rs1_data,ALU_inputB,ALU_result,ALU_code,zero);

		// DATA_MEM_inst(clk,rs2_data,ALU_result,r_data,data_from_memory,data_to_memory, data_address,MEM_write);

		// WB_inst(jump,jump_r,WB_select,r_data,new_PC,ALU_result,funct3,rd_data);

		RAM_inst(clk,funct3,instr_address,data_address,load_instruction, data_to_memory  ,data_from_memory, MEM_write);
		
	}
};

SC_MODULE_EXPORT(Theseus_CPU);


