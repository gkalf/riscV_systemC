#ifndef JUMP_BRANCH_BLOCK_H
#define JUMP_BRANCH_BLOCK_H

#include "systemc.h"

template<unsigned SIZE = 32>
SC_MODULE(jump_branch_block) 
{
	//Local Parameters Declaration...
	//Interface Ports...
	sc_in<sc_uint<SIZE> >	PC;
	sc_in<sc_uint<SIZE> >	new_PC;
	sc_in<sc_uint<SIZE> >	J_imm;
	sc_in<sc_uint<SIZE> >	B_imm;
	sc_in<sc_uint<SIZE> >	I_imm;
	sc_out<sc_uint<SIZE> >	next_PC;
	sc_in<bool>	jump;
	sc_in<bool>	jump_r;
	sc_in<bool>	branch;
	sc_in<bool>	take_branch;
	sc_in<sc_uint<SIZE> >   ALU_result;

	//Processes Declaration...
	void always_process_26();

	//Constructor Declaration...
	SC_CTOR(jump_branch_block)
	{
		SC_METHOD(always_process_26);
		sensitive << jump << PC << J_imm << jump_r << I_imm << branch << take_branch << B_imm;
	}
};

template<unsigned SIZE>
void jump_branch_block<SIZE>::always_process_26() 
{
	if (jump.read()){
		next_PC.write(PC.read()+(J_imm.read()<<1));
	}else if(jump_r.read()){
		next_PC.write(ALU_result.read()<<1);
	}else if(branch.read() & take_branch.read()){
		next_PC.write((B_imm.read()+PC.read()));
	}else{
		next_PC.write(new_PC.read());
	}

}


#endif
