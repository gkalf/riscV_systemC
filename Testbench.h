#include "systemc.h"

SC_MODULE(Testbench){
	sc_out<bool> reset;
	sc_in_clk clk;


	void testprocess(){
		reset.write(1);
		wait();
		reset.write(0);
	}
	SC_CTOR(Testbench){
		SC_CTHREAD(testprocess,clk.pos());
	}
};

