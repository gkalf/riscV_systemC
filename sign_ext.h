#include "systemc.h"

SC_MODULE(SIGN_extend){
	sc_in<int>  imm_12;
	sc_out<int> ALUfromIMM;

	void sign_extend(){

	}	
	
	SC_CTOR(SIGN_extend){
		SC_METHOD(sign_extend);
		sensitive << imm_12;
	}
};
