/******************************************************************
	Register_File_Register_File.h
	This file is generated from Register_File.v by V2SC
	(c) Copryight 2007 by Ali Haj Abutalebi & Leila Mahmoudi Ayough
	Mazdak and Alborz Design Automation
	email: info@mazdak-alborz.com
	website: www.mazdak-alborz.com
*******************************************************************/

#ifndef _REGISTER_FILE_REGISTER_FILE_H
#define _REGISTER_FILE_REGISTER_FILE_H

#include "systemc.h"

template<unsigned SIZE = 32, unsigned Number_OF_Registers = 5>
SC_MODULE(Register_File) 
{
	//Interface Ports...
	sc_in<bool>	clk;
	sc_in<bool>	rd_en;
	sc_in<bool>	rs1_en;
	sc_in<bool>	rs2_en;
	sc_in<sc_uint<Number_OF_Registers> >	rd;
	sc_in<sc_uint<Number_OF_Registers> >	rs1;
	sc_in<sc_uint<Number_OF_Registers> >	rs2;
	sc_in<sc_uint<SIZE> >	rd_data;
	sc_out<sc_uint<SIZE> >	rs1_data;
	sc_out<sc_uint<SIZE> >	rs2_data;

	//Internal Signals...
	sc_uint<SIZE> 	RAM[(1 << Number_OF_Registers)];

	//Processes Declaration...
	void initial_process_29();
	void always_process_36();
	void always_process_52();

	//Constructor Declaration...
	SC_CTOR(Register_File)
	{
		SC_METHOD(initial_process_29);

		SC_METHOD(always_process_36);
		sensitive << rs1_en << rs1 << rs2_en << rs2;

		SC_METHOD(always_process_52);
		sensitive << clk.pos();
	}
};

template<unsigned SIZE, unsigned Number_OF_Registers>
void Register_File<SIZE, Number_OF_Registers>::initial_process_29() 
{
	for(unsigned i = 0; i < SIZE; i = i + 1) 
	{
		RAM[i] = 0;
	}
}

template<unsigned SIZE, unsigned Number_OF_Registers>
void Register_File<SIZE, Number_OF_Registers>::always_process_36() 
{
	if( rs1_en.read() ) 
	{
		rs1_data = RAM[rs1.read()];
	}
	else
	{
		rs1_data = 0;
	}
	if( rs2_en.read() ) 
	{
		rs2_data = RAM[rs2.read()];
	}
	else
	{
		rs2_data = 0;
	}
}

template<unsigned SIZE, unsigned Number_OF_Registers>
void Register_File<SIZE, Number_OF_Registers>::always_process_52() 
{
	if( rd_en.read() )
	{

		RAM[rd.read()] = rd_data.read();
		RAM[0] = 0;
		int i = 0;
		for (i=0;i<32;i++){
			int tmp=RAM[i];
			printf("RF[%d] = %x \n",i,tmp);
		}	
	}
	else
	{
		RAM[0] = 0;
	};
	
}

#endif
