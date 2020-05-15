#include "systemc.h"

SC_MODULE(PC){
	sc_in <int> new_address;
	sc_in <int> address_from_jump;
	sc_in <bool> reset;
	sc_in <bool> jump,branch,Zero;
	sc_in_clk clk;
	sc_out <int> PC_address;		

	void pc(){
		int new_instr=new_address.read();
		if (reset.read()){
			PC_address.write(0);
			printf("reset_address = 0\n");
		}else{
			//if (jump instruction next address_from_jump+2)
			if (jump.read()){
				new_instr = (address_from_jump.read() << 1)+new_instr;
			//if (branch instruction & zero then address_fron branch+2)	
			}else if (branch.read() & Zero){
				new_instr = (address_from_jump.read() << 1)+new_instr;
			}else{
			//else new_inst+4
				new_instr = new_instr + 4;	
			}
			printf("current instruction address = %d\n",new_instr);
			PC_address.write(new_instr);
		}	
	}

	SC_CTOR(PC){
		SC_METHOD(pc);
		sensitive << clk.pos();
			


	}

};