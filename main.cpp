#include "ALU.h"
#include "Testbench.h"
#include "rf.h"
#include "decoder.h"
#include "PC.h"
#include "data_MEM.h"
#include "fetch.h"
#include "ReG_or_Immidiate.h"

int sc_main(int argc, char *argv[]){
	sc_signal<int> rs1_data,rs2_data,ALU_b,imm_extend,Dout;
	sc_signal<int , SC_MANY_WRITERS> rd_data;
	sc_signal<unsigned int > alucode;
	sc_signal<unsigned int > instruction;
	sc_signal<int> rs1_address,rs2_address,rd_address;
	sc_signal<bool> Z,wr_to_rf,ALU_B_source,read_from_mem,write_to_mem,jump,branch;
	sc_signal<bool> reset;
	sc_signal<int> PC_address;
	sc_clock sysclk("clk", 20, SC_NS);

	DATA_MEMORY     data_MEM("DATA_MEMORY");
	ALU             alu("ALU");
	FETCH           fetch("FETCH");
	PC              pc("PC");
	rf              RF("RF");
	Decoder         decoder("decoder");
	Testbench       test1("TestBench");
	RF_or_Decoder   rf_or_decoder("ReG_or_Immidiate");


	sc_trace_file *wf = sc_create_vcd_trace_file("counter");
	// Dump the desired signals
	sc_trace(wf, rs1_data, "A");
	sc_trace(wf, ALU_b, "B");
	sc_trace(wf, alucode, "alucode");
	sc_trace(wf, Z,"ZERO");
	sc_trace(wf, rd_data, "rd_data");
	sc_trace(wf, Z, "ZERO");
	sc_trace(wf, instruction,"instruction");
	sc_trace(wf, rs1_address,"RS1");
	sc_trace(wf, rs2_address,"RS2");
	sc_trace(wf, rs1_data,"rs1_data");
	sc_trace(wf, rs2_data,"rs2_data");
	sc_trace(wf, imm_extend,"imm_extend");
	sc_trace(wf, sysclk,"sysclk");
	sc_trace(wf, reset,"reset");
	sc_trace(wf, PC_address,"PC");
	sc_trace(wf, Dout,"Dout");
	sc_trace(wf, branch,"branch");
	sc_trace(wf, read_from_mem,"read_from_mem");
	sc_trace(wf, write_to_mem,"write_to_mem");
	sc_trace(wf, ALU_B_source,"ALU_source");
//instatiating testbench
	test1.reset(reset);
	test1.clk(sysclk);
	
//instatiating programm counter
	pc.address_from_jump(imm_extend);
	pc.jump(jump);
	pc.branch(branch);
	pc.Zero(Z);
	pc.PC_address(PC_address);
	pc.new_address(PC_address);
	pc.reset(reset);
	pc.clk(sysclk);
//instatiating the data_memory
	data_MEM.address(Dout);
	data_MEM.write_data(rs2_data);
	data_MEM.read_from_mem(read_from_mem);
	data_MEM.write_to_mem(write_to_mem);
	data_MEM.data_to_reg(rd_data);
	data_MEM.reset(reset);
//instatiating alu
	alu.inA(rs1_data);
	alu.inB(ALU_b);
	alu.alucode(alucode);
	alu.Dout(Dout);
	alu.Z(Z);
//instatiating ALU_source
	rf_or_decoder.Reg_in(rs2_data);
	rf_or_decoder.imm_in(imm_extend);
	rf_or_decoder.ALU_B_source(ALU_B_source);
	rf_or_decoder.ALU_B(ALU_b);
//instatiating register file
	RF.rs1_address(rs1_address);
	RF.rs2_address(rs2_address);
	RF.rd_address(rd_address);
	RF.rs1_data(rs1_data);
	RF.rs2_data(rs2_data);
	RF.rd_data(rd_data);
	RF.wr_to_reg(wr_to_rf);
	RF.reset(reset);
//instatiating decoder
	decoder.instruction(instruction);
	decoder.rs1(rs1_address);
	decoder.rs2(rs2_address);
	decoder.rd(rd_address);
	decoder.imm_extend(imm_extend);
	decoder.alucode(alucode);
	decoder.wr_to_rf(wr_to_rf);
	decoder.ALU_B_source(ALU_B_source);
	decoder.read_from_mem(read_from_mem);
	decoder.write_to_mem(write_to_mem);
	decoder.jump(jump);
	decoder.branch(branch);

//instatiating fetch
	fetch.PC_address(PC_address);
	fetch.instruction(instruction);
	fetch.reset(reset);

	sc_start(2900, SC_NS);
    sc_close_vcd_trace_file(wf);

	return 0;
}