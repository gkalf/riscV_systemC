#include "systemc.h"

template<unsigned DATA_WIDTH = 32>
SC_MODULE(BTB)
{
	sc_in_clk				clk;
	sc_in<bool>				reset;
	sc_in<sc_uint<32> >		new_PC;
	sc_in<sc_uint<32> >		PC;

	sc_out<sc_uint<32> > 	instr_addr;
	
	sc_signal<sc_uint<32> > TBH[1024];

	void branch_prediction();

	SC_CTOR(BTB){
		SC_CTHREAD(branch_prediction,clk.pos());
	}
};


template<unsigned DATA_WIDTH>
void BTB<DATA_WIDTH>::branch_prediction(){

	while(1){
		for (int i=0; i<1024; i++){
			if (reset){
				TBH[i]=0;
			}else{
				
			}
		}

		wait();
	}
}