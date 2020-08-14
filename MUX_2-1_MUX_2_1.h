/******************************************************************
	MUX_2-1_MUX_2_1.h
	This file is generated from MUX_2-1.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#ifndef _MUX_2_1_MUX_2_1_H
#define _MUX_2_1_MUX_2_1_H

#include "systemc.h"

template<unsigned SIZE = 32>
SC_MODULE(MUX_2_1) 
{
	//Interface Ports...
	sc_in<sc_uint<SIZE> >	a;
	sc_in<sc_uint<SIZE> >	b;
	sc_in<bool>	choose_wisely;
	sc_out<sc_uint<SIZE> >	c;

	//Processes Declaration...
	void always_process_15();

	//Constructor Declaration...
	SC_CTOR(MUX_2_1)
	{
		SC_METHOD(always_process_15);
		sensitive << choose_wisely << a << b;
	}
};

template<unsigned SIZE>
void MUX_2_1<SIZE>::always_process_15() 
{
	if( choose_wisely.read() ) 
	{
		c = a.read();
	}
	else
	{
		c = b.read();
	}
}

#endif
