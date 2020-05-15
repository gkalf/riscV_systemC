#include "systemc.h"

SC_MODULE(Adder2Bits){
	sc_in<sc_int<2> > A, B;
	sc_in<sc_int<1> > Cin;
	sc_out<sc_int<3> > sumAndCarry;

	void add(){

		sumAndCarry.write(A.read() + B.read()+Cin.read());
	}

	SC_CTOR(Adder2Bits){
		SC_METHOD(add);
		sensitive << A << B << Cin;
	}
};