#include "systemc.h"

SC_MODULE(RF_or_Decoder){
	sc_in <int>  Reg_in,imm_in;
	sc_in <bool> ALU_B_source;
	sc_out<int>  ALU_B;

	void exec(){
		int Reg_in_temp,imm_in_temp;
		Reg_in_temp=Reg_in.read();
		imm_in_temp=imm_in.read();
//if its going to use RS2 or immidiate value
		if (ALU_B_source.read() == false){
			ALU_B.write(Reg_in_temp);
		}else{
			ALU_B.write(imm_in_temp);
		}
	}
	SC_CTOR(RF_or_Decoder){
		SC_METHOD(exec);
		sensitive << Reg_in << imm_in << ALU_B_source << ALU_B;
	}
};