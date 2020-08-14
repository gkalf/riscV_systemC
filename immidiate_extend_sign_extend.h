/******************************************************************
	immidiate_extend_sign_extend.h
	This file is generated from immidiate_extend.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#ifndef _IMMIDIATE_EXTEND_SIGN_EXTEND_H
#define _IMMIDIATE_EXTEND_SIGN_EXTEND_H

#include "systemc.h"

template<unsigned SIZE = 32, unsigned Position_of_sign = 12>
SC_MODULE(sign_extend) 
{
	//Interface Ports...
	sc_in<sc_uint<SIZE> >	number_to_extend;
	sc_in<bool>	enable;
	sc_out<sc_uint<SIZE> >	sign_extended;

	//Internal Signals...
	//sc_signal<sc_uint<32> >	temp;

	//Processes Declaration...
	// void assign_process_temp_15();
	void always_process_18();

	//Constructor Declaration...
	SC_CTOR(sign_extend)
	{
		// SC_METHOD(assign_process_temp_15);
		// sensitive << number_to_extend;

		SC_METHOD(always_process_18);
		sensitive << enable << number_to_extend;
		printf("immidiate_extend init\n");
	}
};

// template<unsigned SIZE, unsigned Position_of_sign>
// void sign_extend<SIZE, Position_of_sign>::assign_process_temp_15() 
// {
// 	int temp = number_to_extend.read();
// 	//temp = (sign_extend.read()[Position_of_sign-1], sign_extend.read().range(Position_of_sign - 1,0));
// }

template<unsigned SIZE, unsigned Position_of_sign>
void sign_extend<SIZE, Position_of_sign>::always_process_18() 
{
	//int tmp = number_to_extend.read().range(Position_of_sign-1,0);
	//uint tmp_uint= number_to_extend.read();

	if( enable.read() ) 
	{
		if (number_to_extend.read()[Position_of_sign-1])
			sign_extended = (sc_uint<(SIZE-Position_of_sign)>(0xFFFFFF),number_to_extend.read().range(Position_of_sign-1,0));
		else
			sign_extended = number_to_extend.read();
	}
	else
	{
		sign_extended = number_to_extend.read();
	}
}

#endif
