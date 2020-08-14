#include "systemc.h"
#include "Fetch_Fetch.h"
#include "decode.h"
#include "ALU.h"
// #include "Mux_4-1_MUX_4_1.h"
// #include "MUX_2-1_MUX_2_1.h"
#include "DATA_mem_DATA_MEM.h"
#include "WB.h"
#include "DUAL_PORT_MEMORY.h"
//#include "jump_branch_block"
// #include "Datapath.h"


SC_MODULE(Theseus_CPU) 
{
	//Interface Ports...
	sc_in <bool> clk;
	sc_in    <bool>	reset;


void printa()
{
	while(1){
		wait();
		
	}
}


	//Internal Signals...
	sc_signal<sc_uint<3> >	funct3;
	sc_signal<bool>	rs1_en;
	sc_signal<bool>	rs2_en;
	sc_signal<sc_uint<5> >	ALU_code;
	sc_signal<bool>	Wr_to_RF;
	sc_signal<bool>	Branch;
	sc_signal<bool>	jump;
	sc_signal<bool>	MEM_write;
	sc_signal<bool>	WB_select;
	// sc_signal<sc_uint<32> >	r_data;
	sc_signal<sc_uint<32> >	ALU_result;
	sc_signal<sc_uint<32> >	PC;
	sc_signal<sc_uint<32> >	new_PC;
	sc_signal<sc_uint<32> >	next_PC;
	sc_signal<sc_uint<32> >	PC_exec;
	sc_signal<sc_uint<32> >	instruction;
	sc_signal<bool> take_branch;
	sc_signal<sc_uint<5> >	rd;
	sc_signal<sc_uint<5> >	rs1;
	sc_signal<sc_uint<5> >	rs2;
	sc_signal<sc_uint<32> > rd_data;
	sc_signal<sc_uint<32> > immidiate;
	sc_signal<sc_uint<32> > control_registers;
	sc_signal<sc_uint<32> > control_registers_MEM;
	sc_signal<sc_uint<32> > control_registers_WB;
	//sc_signal<sc_uint<32> >	immidiate_sign_extended;
	sc_signal<sc_uint<32> >	ALU_inputA;
	sc_signal<sc_uint<32> >	ALU_inputB;
	sc_signal<sc_uint<32> >	data_address;
	sc_signal<sc_uint<32> >	data_from_memory;
	sc_signal<sc_uint<32> > data_to_memory;
	sc_signal<sc_uint<32> >	instr_address;
	sc_signal<sc_uint<32> >	load_instruction;
	sc_signal<sc_uint<32> > rs2_store_data;
	sc_signal<sc_uint<32> > rs2_store_data_MEM;
	sc_signal<sc_uint<32> > ALU_result_to_WB;
	sc_signal<sc_uint<32> > PC_MEM;
	// sc_signal<bool>			rd_en;

	sc_signal<sc_uint<32> >	inputA_3;
	sc_signal<sc_uint<32> >	inputB_3;
	sc_signal<bool>			MEM_write_4;
	sc_signal<bool>			WB_5;
	sc_signal<sc_uint<3> > 	funct3_4;
	sc_signal<sc_uint<3> > 	funct3_5;
	sc_signal<sc_uint<5> > 	rd5;
	sc_signal<bool> 		Wr_to_RF_5;
	sc_signal<bool>			stall;
	sc_signal<sc_uint<32> >	rs2_data_3;
	sc_signal<sc_uint<32> > jump_address;
	sc_signal<sc_uint<32> > immidiate_to_MEM;
	sc_signal<bool>			stall_j;

	
	
	//Instantiated Modules Object Declaration...
	Fetch<32,15>	Fetch_Inst;
	Decode<32>		Decode_dut;
	exec<32>		exec_inst;
	//jump_branch_block<32>	next_PC_inst;
	// Control_Unit	Control_Unit_inst;
	// Register_File<32,5>	Register_File_inst;
	// MUX_4_1<32>	MUX_4_1_inst;
	// MUX_2_1<32>	ALU_A_src_inst;
	// MUX_4_1<32>	ALU_B_src_inst;
	DATA_MEM<32>	DATA_MEM_inst;
	WB<32> WB_inst;
	//Datapath<32>	Datapath_inst;
	RAM<32,18> 		RAM_inst;
	
	//Local Temporary Signals...

	//Signal Handler...
	void control_registers_deliver(){
		MEM_write_4 = control_registers_MEM.read()[2];
		funct3_4  = control_registers_MEM.read().range(12,10);
		WB_5      = control_registers_WB.read()[1];
		funct3_5  = control_registers_WB.read().range(12,10);
		rd5		  = control_registers_WB.read().range(9,5);
		Wr_to_RF_5= control_registers_WB.read()[3];
		if (stall_j.read()){
			instruction=0;
		}else{
			instruction = load_instruction;
		} 
	}
	
	//Constructor Declaration...
	SC_CTOR(Theseus_CPU) :
		Fetch_Inst("FETCH_INST") ,
		Decode_dut("Decode_dut"),
		exec_inst("exec_inst"),
		DATA_MEM_inst("DATA_MEM_INST") ,
		WB_inst("WB_inst"),
		RAM_inst("RAM_inst")
	{
		Fetch_Inst.clk(clk);
		Fetch_Inst.reset(reset);
		Fetch_Inst.next_PC(jump_address);
		Fetch_Inst.new_PC(new_PC);
		Fetch_Inst.PC(PC);
		Fetch_Inst.stall(stall);
		Fetch_Inst.stall_j(stall_j);

		Decode_dut.clk(clk);
		Decode_dut.reset(reset);
		Decode_dut.instruction(instruction);
		Decode_dut.PC(PC);
		Decode_dut.PC_dec(new_PC);
		Decode_dut.rd_en(Wr_to_RF_5);
		Decode_dut.rd_data(rd_data);
		Decode_dut.rd_address(rd5);
		Decode_dut.control_registers(control_registers);
		Decode_dut.PC_exec(PC_exec);
		Decode_dut.immidiate_sign_extended(immidiate);
		Decode_dut.inputA_reg_file(ALU_inputA);
		Decode_dut.inputB_reg_file(ALU_inputB);
		Decode_dut.rs2_store_data(rs2_store_data);
		Decode_dut.stall(stall);
		Decode_dut.stall_j(stall_j);

		exec_inst.clk(clk);
		exec_inst.reset(reset);
		exec_inst.control_registers(control_registers);
		exec_inst.inputA_reg_file(ALU_inputA);
		exec_inst.inputB_reg_file(ALU_inputB);
		exec_inst.immidiate(immidiate);
		exec_inst.PC_exec(PC_exec);
		exec_inst.result(ALU_result);
		exec_inst.take_branch(take_branch);
		exec_inst.control_registers_MEM(control_registers_MEM);
		exec_inst.rs2_store_data(rs2_store_data);
		exec_inst.rs2_store_data_MEM(rs2_store_data_MEM);
		exec_inst.control_registers_from_MEM(control_registers_MEM);
		exec_inst.control_registers_WB(control_registers_WB);
		exec_inst.ALU_result_to_WB(ALU_result_to_WB);
		exec_inst.ALU_result_to_MEM(ALU_result);
		exec_inst.stall(stall_j);
		exec_inst.PC_MEM(PC_MEM);
		exec_inst.immidiate_to_MEM(immidiate_to_MEM);

		DATA_MEM_inst.clk(clk);
		DATA_MEM_inst.reset(reset);
		DATA_MEM_inst.addr(ALU_result);
		DATA_MEM_inst.ALU_result_to_WB(ALU_result_to_WB);
		DATA_MEM_inst.PC_from_rs2_data_to_Store(rs2_store_data_MEM);
		DATA_MEM_inst.control_registers(control_registers_MEM);
		DATA_MEM_inst.control_registers_WB(control_registers_WB);
		DATA_MEM_inst.jump_address(jump_address);
		DATA_MEM_inst.stall_j(stall_j);
		DATA_MEM_inst.take_branch(take_branch);
		DATA_MEM_inst.PC_MEM(PC_MEM);
		DATA_MEM_inst.immidiate_to_MEM(immidiate_to_MEM);

		WB_inst.WB_select(WB_5);
		WB_inst.funct3(funct3_5);
		WB_inst.r_data(data_from_memory);
		WB_inst.ALU_result(ALU_result_to_WB);
		WB_inst.rd_data(rd_data);

		RAM_inst.clk(clk);
		RAM_inst.funct3(funct3_4);
		RAM_inst.address1(new_PC);
		RAM_inst.address2(ALU_result);
		RAM_inst.data1(load_instruction);
		RAM_inst.data2(rs2_store_data_MEM);
		RAM_inst.data2_out(data_from_memory);
		RAM_inst.MEM_write(MEM_write_4);
		
		SC_THREAD(printa);
		sensitive << clk.pos();

		SC_METHOD(control_registers_deliver);
		sensitive << load_instruction << stall_j << control_registers << control_registers_MEM << control_registers_WB;
	}
};



