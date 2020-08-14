/******************************************************************
	Mux_4-1_MUX_4_1.h
	This file is generated from Mux_4-1.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#ifndef _MUX_4_1_MUX_4_1_H
#define _MUX_4_1_MUX_4_1_H

#include "systemc.h"

template<unsigned SIZE = 32>
SC_MODULE(MUX_4_1) 
{
	//Interface Ports...
	sc_in<sc_uint<SIZE> >	a;
	sc_in<sc_uint<SIZE> >	b;
	sc_in<sc_uint<SIZE> >	c;
	sc_in<sc_uint<SIZE> >	d;
	sc_in<sc_uint<2> >	choose_wisely;
	sc_out<sc_uint<SIZE> >	out;

	//Processes Declaration...
	void always_process_13();

	//Constructor Declaration...
	SC_CTOR(MUX_4_1)
	{
		SC_METHOD(always_process_13);
		sensitive << choose_wisely << a << b << c << d;
	}
};

template<unsigned SIZE>
void MUX_4_1<SIZE>::always_process_13() 
{
	if( choose_wisely.read() == 0 ) 
	{
		out = a.read();
	}
	else
	{
		if( choose_wisely.read() == 1 ) 
		{
			out = b.read();
		}
		else
		{
			if( choose_wisely.read() == 2 ) 
			{
				out = c.read();
			}
			else
			{
				if( choose_wisely.read() == 3 ) 
				{
					out = d.read();
				}
			}
		}
	}
}

#endif
