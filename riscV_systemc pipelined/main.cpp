// Code your testbench here.
// Uncomment the next line for SystemC modules.
#include <systemc>
#include "design.cpp"

int sc_main(int ac, char *av[])
{
	//Instantiated Top Module...
		  sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);

  
	sc_signal<bool> clock;
	sc_signal<bool> reset;
    sc_signal<sc_uint<32> > a;
  sc_signal<sc_uint<32> > b;
  sc_signal<sc_uint<32> > c;
    sc_signal<bool> start;
	
	STATE_MACHINE	dut("dut");
  
  	dut.clock(clock);
  	dut.reset(reset);
    dut.a(a);
  	dut.b(b);
  	dut.c(c);
    dut.start(start);
	
	sc_initialize();
// Let the clock run for 10 cycles
	for (int i = 0; i <= 2; i++)
	{
		clock = 1;
		sc_start(10,SC_NS);
		clock = 0;
		sc_start(10,SC_NS);
	}
  	a = 500; b = 10000; start =1;
    for (int i = 0; i <= 10; i++)
      {
          clock = 1;
          sc_start(10,SC_NS);
          clock = 0;
          sc_start(10,SC_NS);
      	  start = 0;
      }
  	// //VCD File...
	sc_trace_file *tf = sc_create_vcd_trace_file("Theuseus_Risc_V_top");
	sc_trace(tf, dut.clock, "clock");
	sc_trace(tf, dut.reset, "reset");
	sc_trace(tf, dut.a, "a");
	sc_trace(tf, dut.b, "b");
  sc_trace(tf, dut.c, "c");
  sc_trace(tf, dut.start, "start");
	

	sc_close_vcd_trace_file(tf);
  return 0;
}

