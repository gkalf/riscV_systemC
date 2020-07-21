#include "systemc.h"
#include "Theuseus_Risc_V_top_Theseus_CPU.h"


int sc_main(int ac, char *av[])
{
	//Instantiated Top Module...
	  sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
		sc_report_handler::set_actions( SC_ID_LOGIC_X_TO_BOOL_, SC_LOG);
		sc_report_handler::set_actions( SC_ID_VECTOR_CONTAINS_LOGIC_VALUE_, SC_LOG);
		//sc_report_handler::set_actions( SC_ID_OBJECT_EXISTS_, SC_LOG);

	sc_signal<bool> clock;
	sc_signal<bool> reset;
	
	Theseus_CPU	dut("dut");

	dut.clk(clock);
	dut.reset(reset);

	
	sc_initialize();
// Let the clock run for 10 cycles
	for (int i = 0; i <= 400; i++)
	{
		clock = 1;
		sc_start(10,SC_NS);
		clock = 0;
		sc_start(10,SC_NS);
	}


	// //VCD File...
	// sc_trace_file *tf = sc_create_vcd_trace_file("Theuseus_Risc_V_top");
	// sc_trace(tf, dut.carry, "carry");
	// sc_trace(tf, dut.zero, "zero");
	// sc_trace(tf, dut.opcode, "opcode");
	// sc_trace(tf, dut.funct3, "funct3");
	// sc_trace(tf, dut.funct7, "funct7");
	// sc_trace(tf, dut.rs1_en, "rs1_en");
	// sc_trace(tf, dut.rs2_en, "rs2_en");
	// sc_trace(tf, dut.ALU_code, "ALU_code");
	// sc_trace(tf, dut.ALU_src, "ALU_src");
	// sc_trace(tf, dut.Wr_to_RF, "Wr_to_RF");
	// sc_trace(tf, dut.Branch, "Branch");
	// sc_trace(tf, dut.jump, "jump");
	// sc_trace(tf, dut.Imm_sel, "Imm_sel");
	// sc_trace(tf, dut.MEM_write, "MEM_write");
	// sc_trace(tf, dut.WB_select, "WB_select");
	// sc_trace(tf, dut.r_data, "r_data");
	// sc_trace(tf, dut.ALU_result, "ALU_result");
	// sc_trace(tf, dut.PC, "PC");
	// sc_trace(tf, dut.new_PC, "new_PC");
	// sc_trace(tf, dut.next_PC, "next_PC");
	// sc_trace(tf, dut.instruction, "instruction");
	// sc_trace(tf, dut.rd, "rd");
	// sc_trace(tf, dut.rs1, "rs1");
	// sc_trace(tf, dut.rs2, "rs2");
	// sc_trace(tf, dut.rs1_data, "rs1_data");
	// sc_trace(tf, dut.rs2_data, "rs2_data");
	// sc_trace(tf, dut.rd_data, "rd_data");
	// sc_trace(tf, dut.I_imm, "I_imm");
	// sc_trace(tf, dut.S_imm, "S_imm");
	// sc_trace(tf, dut.U_imm, "U_imm");
	// sc_trace(tf, dut.B_imm, "B_imm");
	// sc_trace(tf, dut.J_imm, "J_imm");
	// sc_trace(tf, dut.ALU_A_src, "ALU_A_src");
	// sc_trace(tf, dut.ALU_B_src, "ALU_B_src");
	// sc_trace(tf, dut.jump_r, "jump_r");
	// sc_trace(tf, dut.immidiate, "immidiate");

	//Start Simulation...
	//sc_start(3000,SC_NS);

	// sc_close_vcd_trace_file(tf);

	return 0;
}
