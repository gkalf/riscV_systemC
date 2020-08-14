#ifndef _DATA_MEM_DATA_MEM_H
#define _DATA_MEM_DATA_MEM_H

#include "systemc.h"


template<unsigned SIZE = 32>
SC_MODULE(DATA_MEM) 
{
	//Interface Ports...
	sc_in_clk				clk;
	sc_in<bool> 			reset;
	sc_in<sc_uint<SIZE> >	addr;
	sc_in<sc_uint<SIZE> > 	control_registers;
	sc_in<sc_uint<SIZE> >	PC_from_rs2_data_to_Store;
	sc_out<sc_uint<SIZE> >	ALU_result_to_WB;
	sc_out<sc_uint<SIZE> > 	control_registers_WB;
	sc_out<sc_uint<SIZE> >	jump_address;
	sc_out<bool>  stall_j;
	sc_in<bool>		take_branch;
	sc_in<sc_uint<SIZE> >	PC_MEM;
	sc_in<sc_uint<SIZE> >	immidiate_to_MEM;

	void jump_block();
	void write_or_read_data();

	//Constructor Declaration...
	SC_CTOR(DATA_MEM)
	{
		SC_CTHREAD(write_or_read_data,clk.pos());

		SC_METHOD(jump_block);
		sensitive << addr << control_registers << take_branch <<immidiate_to_MEM << PC_from_rs2_data_to_Store;
	}
};

template<unsigned SIZE>
void DATA_MEM<SIZE>::write_or_read_data() 
{
	while(1){
		if ((reset.read() == 1) | stall_j.read()==1){
			ALU_result_to_WB = 0;
			control_registers_WB = 0;
		}else{
			control_registers_WB.write(control_registers);
			if (control_registers.read()[0] | (control_registers.read()[31])){
				ALU_result_to_WB = PC_from_rs2_data_to_Store.read();
			}else{
				ALU_result_to_WB =addr.read();
			}
		}
		
		wait();
	}	
}

template<unsigned SIZE>
void DATA_MEM<SIZE>::jump_block()
{
	if (control_registers.read()[0]){
		jump_address = immidiate_to_MEM.read() + PC_MEM.read();
		stall_j = 1;
	}else if (control_registers.read()[31]){
		jump_address = addr.read()<<1;
		stall_j = 1;
	}else if (control_registers.read()[4] & take_branch.read() ){
		jump_address = PC_MEM.read() + immidiate_to_MEM.read();
		stall_j = 1;
	}else{
		stall_j = 0;
		jump_address = 0;
	}
}

#endif
