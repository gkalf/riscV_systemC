#ifndef _DUAL_PORT_MEMORY_H
#define _DUAL_PORT_MEMORY_H

#include "systemc.h"
#include "stdlib.h"

template<unsigned DATA_WIDTH = 32, unsigned ADDR_WIDTH = 18>
SC_MODULE(RAM) 
{
	sc_in_clk clk;
	sc_in<sc_uint<3> > funct3;
	sc_in<sc_uint<DATA_WIDTH> >	address1;
	sc_in<sc_uint<DATA_WIDTH> >	address2;
	sc_out<sc_uint<DATA_WIDTH> > data1;
	sc_in<sc_uint<DATA_WIDTH> > data2;
	sc_out<sc_uint<DATA_WIDTH> > data2_out;
	sc_in<bool> MEM_write;


	sc_uint<DATA_WIDTH> 	Dual_RAM[1<<ADDR_WIDTH];
	
		//Internal Signals...
	sc_signal<bool>	tx_byte;
	sc_signal<bool>	tx_half;
	sc_signal<bool>	tx_word;
	sc_signal<bool>	byte_at_00;
	sc_signal<bool>	byte_at_01;
	sc_signal<bool>	byte_at_10;
	sc_signal<bool>	byte_at_11;
	sc_signal<bool>	half_at_00;
	sc_signal<bool>	half_at_10;
	sc_signal<bool>	word_at_00;
	sc_signal<bool>	byte0;
	sc_signal<bool>	byte1;
	sc_signal<bool>	byte2;
	sc_signal<bool>	byte3;
	sc_signal<sc_uint<DATA_WIDTH> > previous_address;
	sc_signal<sc_uint<DATA_WIDTH> > previous_data;

	//Processes Declaration...
	void assign_process_tx_byte_22();
	void assign_process_tx_half_23();
	void assign_process_tx_word_24();
	void assign_process_byte_at_00_26();
	void assign_process_byte_at_01_27();
	void assign_process_byte_at_10_28();
	void assign_process_byte_at_11_29();
	void assign_process_half_at_00_31();
	void assign_process_half_at_10_32();
	void assign_process_word_at_00_34();
	void assign_process_byte0_36();
	void assign_process_byte1_37();
	void assign_process_byte2_38();
	void assign_process_byte3_39();
	void always_process_43();
	void retrieve_instruction();
	void retrieve_data();
	void store_data();
	
	int	i;
	FILE*	in;
	int	statusI;
	sc_uint<32> 	din;
	sc_uint<32> 	addr;
	
	SC_CTOR(RAM){
		SC_METHOD(assign_process_tx_byte_22);
		sensitive << funct3;

		SC_METHOD(assign_process_tx_half_23);
		sensitive << funct3;

		SC_METHOD(assign_process_tx_word_24);
		sensitive << funct3;

		SC_METHOD(assign_process_byte_at_00_26);
		sensitive << tx_byte << address2;

		SC_METHOD(assign_process_byte_at_01_27);
		sensitive << tx_byte << address2;

		SC_METHOD(assign_process_byte_at_10_28);
		sensitive << tx_byte << address2;

		SC_METHOD(assign_process_byte_at_11_29);
		sensitive << tx_byte << address2;

		SC_METHOD(assign_process_half_at_00_31);
		sensitive << tx_half << address2;

		SC_METHOD(assign_process_half_at_10_32);
		sensitive << tx_half << address2;

		SC_METHOD(assign_process_word_at_00_34);
		sensitive << tx_word;

		SC_METHOD(assign_process_byte0_36);
		sensitive << word_at_00 << half_at_00 << byte_at_00;

		SC_METHOD(assign_process_byte1_37);
		sensitive << word_at_00 << half_at_00 << byte_at_01;

		SC_METHOD(assign_process_byte2_38);
		sensitive << word_at_00 << half_at_10 << byte_at_10;

		SC_METHOD(assign_process_byte3_39);
		sensitive << word_at_00 << half_at_10 << byte_at_11;

		SC_CTHREAD(always_process_43,clk.pos());
		//sensitive << clk.pos();

		
		// SC_METHOD(retrieve_instruction);
		// sensitive << clk.pos();
		
		// SC_METHOD(retrieve_data);
		// sensitive << address2;
		
		FILE *fp = fopen("I-ADD-01.hex","r");
		unsigned size=0;
		unsigned mem_word;

		printf("** ALERT ** BIOS: initialize BIOS\n");
		for (size = 0; size < (1<<ADDR_WIDTH); size++) {	// initialize bad data
			Dual_RAM[size] = 0;
		}

		size = 0;
		while (fscanf(fp,"%x\n", &mem_word) != EOF) {
			printf("DATA[%d]=%x\n",size,mem_word);
			Dual_RAM[size] = mem_word;
			size++;
		}
	}
};

// template<unsigned DATA_WIDTH, unsigned ADDR_WIDTH>
// void RAM<DATA_WIDTH, ADDR_WIDTH>::retrieve_instruction() 
// {
// 	data1 = Dual_RAM[address1.read().range(ADDR_WIDTH,2)];
// 	}

// template<unsigned DATA_WIDTH, unsigned ADDR_WIDTH>
// void RAM<DATA_WIDTH, ADDR_WIDTH>::retrieve_data() 
// {
// 	data2_out = Dual_RAM[address2.read().range(ADDR_WIDTH,2)];
	
// }

template<unsigned DATA_WIDTH, unsigned ADDR_WIDTH>
void RAM<DATA_WIDTH, ADDR_WIDTH>::assign_process_tx_byte_22() 
{
	tx_byte = !(funct3.read()[1]) & !(funct3.read()[0]);
}

template<unsigned DATA_WIDTH, unsigned ADDR_WIDTH>
void RAM<DATA_WIDTH, ADDR_WIDTH>::assign_process_tx_half_23() 
{
	tx_half = !(funct3.read()[1]) & funct3.read()[0];
}

template<unsigned DATA_WIDTH, unsigned ADDR_WIDTH>
void RAM<DATA_WIDTH, ADDR_WIDTH>::assign_process_tx_word_24() 
{
	tx_word = funct3.read()[1];
}

template<unsigned DATA_WIDTH, unsigned ADDR_WIDTH>
void RAM<DATA_WIDTH, ADDR_WIDTH>::assign_process_byte_at_00_26() 
{
	byte_at_00 = tx_byte.read() & !(address2.read()[1]) & !(address2.read()[0]);
}

template<unsigned DATA_WIDTH, unsigned ADDR_WIDTH>
void RAM<DATA_WIDTH, ADDR_WIDTH>::assign_process_byte_at_01_27() 
{
	byte_at_01 = tx_byte.read() & !(address2.read()[1]) & address2.read()[0];
}

template<unsigned DATA_WIDTH, unsigned ADDR_WIDTH>
void RAM<DATA_WIDTH, ADDR_WIDTH>::assign_process_byte_at_10_28() 
{
	byte_at_10 = tx_byte.read() & address2.read()[1] & !(address2.read()[0]);
}

template<unsigned DATA_WIDTH, unsigned ADDR_WIDTH>
void RAM<DATA_WIDTH, ADDR_WIDTH>::assign_process_byte_at_11_29() 
{
	byte_at_11 = tx_byte.read() & address2.read()[1] & address2.read()[0];
}

template<unsigned DATA_WIDTH, unsigned ADDR_WIDTH>
void RAM<DATA_WIDTH, ADDR_WIDTH>::assign_process_half_at_00_31() 
{
	half_at_00 = tx_half.read() & !(address2.read()[1]);
}

template<unsigned DATA_WIDTH, unsigned ADDR_WIDTH>
void RAM<DATA_WIDTH, ADDR_WIDTH>::assign_process_half_at_10_32() 
{
	half_at_10 = tx_half.read() & address2.read()[1];
}

template<unsigned DATA_WIDTH, unsigned ADDR_WIDTH>
void RAM<DATA_WIDTH, ADDR_WIDTH>::assign_process_word_at_00_34() 
{
	word_at_00 = tx_word.read();
}

template<unsigned DATA_WIDTH, unsigned ADDR_WIDTH>
void RAM<DATA_WIDTH, ADDR_WIDTH>::assign_process_byte0_36() 
{
	byte0 = word_at_00.read() | half_at_00.read() | byte_at_00.read();
}

template<unsigned DATA_WIDTH, unsigned ADDR_WIDTH>
void RAM<DATA_WIDTH, ADDR_WIDTH>::assign_process_byte1_37() 
{
	byte1 = word_at_00.read() | half_at_00.read() | byte_at_01.read();
}

template<unsigned DATA_WIDTH, unsigned ADDR_WIDTH>
void RAM<DATA_WIDTH, ADDR_WIDTH>::assign_process_byte2_38() 
{
	byte2 = word_at_00.read() | half_at_10.read() | byte_at_10.read();
}

template<unsigned DATA_WIDTH, unsigned ADDR_WIDTH>
void RAM<DATA_WIDTH, ADDR_WIDTH>::assign_process_byte3_39() 
{
	byte3 = word_at_00.read() | half_at_10.read() | byte_at_11.read();
}

template<unsigned DATA_WIDTH, unsigned ADDR_WIDTH>
void RAM<DATA_WIDTH, ADDR_WIDTH>::always_process_43() 
{
	previous_address = 0;
	while(1){

	data1 = Dual_RAM[address1.read().range(ADDR_WIDTH,2)];
	if (previous_address.read() == address2.read()){
		data2_out = previous_data;
	}else{
		data2_out = Dual_RAM[address2.read().range(ADDR_WIDTH,2)];
		previous_address == address2.read();
	}
	
	if( MEM_write.read() ) 
	{
		previous_data = data2.read().range(31,0);
		if( word_at_00.read() ) 
		{
			Dual_RAM[address2.read().range(ADDR_WIDTH,2)].range(31,0) = data2.read().range(31,0);
		}
		if( tx_half.read() ) 
		{
			if( half_at_00.read() ) 
			{
				Dual_RAM[address2.read().range(ADDR_WIDTH,2)].range(15,0) = data2.read().range(15,0);
			}
			if( half_at_10.read() ) 
			{
				Dual_RAM[address2.read().range(ADDR_WIDTH,2)].range(31,16) = data2.read().range(15,0);
			}
		}
		if( tx_byte.read() ) 
		{
			if( byte0.read() ) 
			{
				Dual_RAM[address2.read().range(ADDR_WIDTH,2)].range(7,0) = data2.read().range(7,0);
			}
			if( byte1.read() ) 
			{
				Dual_RAM[address2.read().range(ADDR_WIDTH,2)].range(15,8) = data2.read().range(7,0);
			}
			if( byte2.read() ) 
			{
				Dual_RAM[address2.read().range(ADDR_WIDTH,2)].range(23,16) = data2.read().range(7,0);
			}
			if( byte3.read() ) 
			{
				Dual_RAM[address2.read().range(ADDR_WIDTH,2)].range(31,24) = data2.read().range(7,0);
			}
		}
	}
	wait();
	}
}

#endif

