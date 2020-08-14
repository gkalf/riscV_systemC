#include "systemc.h"
#include "WB.h"


int sc_main(int ac, char *av[])
{
	//Instantiated Top Module...
	  sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
		sc_report_handler::set_actions( SC_ID_LOGIC_X_TO_BOOL_, SC_LOG);
		sc_report_handler::set_actions( SC_ID_VECTOR_CONTAINS_LOGIC_VALUE_, SC_LOG);
		sc_report_handler::set_actions( SC_ID_OBJECT_EXISTS_, SC_LOG);

	sc_signal<bool> WB_select;
	sc_signal<sc_uint<32> > r_data;
	sc_signal<sc_uint<32> > ALU_result;
	sc_signal<sc_uint<3> > funct3;
	sc_signal<sc_uint<32> > rd_data;
	
	WB dut("dut");

	dut.WB_select(WB_select);
	dut.r_data(r_data);
	dut.ALU_result(ALU_result);
	dut.funct3(funct3);
	dut.rd_data(rd_data);

	
	sc_initialize();
// Let the clock run for 10 cycles
//	for (int i = 0; i <= 400; i++)
//	{
//		clock = 1;
//		sc_start(10,SC_NS);
//		clock = 0;
//		sc_start(10,SC_NS);
//	}
//


	return 0;
}
