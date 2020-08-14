#ifndef _FETCH_FETCH_H
#define _FETCH_FETCH_H

#include "systemc.h"
#include "stdlib.h"

template<unsigned DATA_WIDTH = 32, unsigned ADDR_WIDTH = 15>
SC_MODULE(Fetch) 
{
	//Interface Ports...
	sc_in_clk						clk;
	sc_in<bool>						reset;

	sc_in<sc_uint<DATA_WIDTH> >   	next_PC;
	sc_in<bool>						stall;
	sc_in<bool>						stall_j;

	sc_out<sc_uint<DATA_WIDTH> > 	new_PC;
	sc_out<sc_uint<DATA_WIDTH> >    PC;
	//Internal Signals...
	//Processes Declaration...
	void fetch_to_decode();

	//Constructor Declaration...
	SC_CTOR(Fetch)
	{
		SC_CTHREAD(fetch_to_decode,clk.pos());		
  }
};



template<unsigned DATA_WIDTH, unsigned ADDR_WIDTH>
void Fetch<DATA_WIDTH, ADDR_WIDTH>::fetch_to_decode() 
{
	// unsigned PC;
	int i=0;
	new_PC = -4;
	while(1){
		PC = new_PC.read();
		if (reset.read() == 1){
			new_PC = 0;
		}else if (stall.read()) {
			new_PC = PC.read();
		}else if (stall_j.read()){
			new_PC = next_PC;
		}else{
			new_PC = new_PC.read() + 4 ;
		}
		wait();
	}	
}


#endif
