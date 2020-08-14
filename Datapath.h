#ifndef _DATAPATH_H
#define _DATAPATH_H

#include "systemc.h"

template<unsigned SIZE=32>
SC_MODULE(Datapath){

sc_in_clk 				clk;
sc_in<bool>				reset;
sc_in<sc_uint<SIZE> > 	PC;
sc_in<sc_uint<SIZE> >	ALU_inputA;
sc_in<sc_uint<SIZE> >	ALU_inputB;
sc_in<sc_uint<SIZE> >	rs2_data;
sc_in<sc_uint<SIZE> >	immidiate;
sc_in<sc_uint<SIZE> >	ALU_result;
sc_in<sc_uint<SIZE> >   ALU_result_to_WB;
sc_in<sc_uint<3> >		funct3;
sc_in<sc_uint<5> >		rd;
sc_in<sc_uint<5> >		rs1;
sc_in<sc_uint<5> > 		rs2;
sc_in<bool> 			WB_select;
sc_in<bool> 			Wr_to_RF;
sc_in<bool> 			MEM_write;
sc_in<bool>  			jump;
sc_in<bool> 			branch;
sc_in<bool>				rs1_en;
sc_in<bool>				rs2_en;

sc_out<sc_uint<SIZE> >	inputA_3;
sc_out<sc_uint<SIZE> >	inputB_3;
sc_out<bool>		 	MEM_write_4;
sc_out<bool>			WB_5;
sc_out<sc_uint<3> >		funct3_5;
sc_out<sc_uint<3> >		funct3_4;
sc_out<sc_uint<5> > 	rd5;
sc_out<bool > 			Wr_to_RF_5;
sc_out<sc_uint<SIZE> >	rs2_data_3;


sc_signal<sc_uint<5> >	rd3;
sc_signal<sc_uint<5> >	rd4;
sc_signal<bool>			Wr_to_RF_3;
sc_signal<bool>			WB_3;

void pipeline();
void forward();

	SC_CTOR(Datapath)
	{
		SC_CTHREAD(pipeline,clk.pos());

		SC_METHOD(forward);
		sensitive << clk.pos();
	}
};

template<unsigned SIZE>
void Datapath<SIZE>::forward()
{
	inputA_3 = 	(rs1_en.read() == 1)?(rd3 == rd4 )?ALU_result.read():
			   	(rd3 == rd5 )?ALU_result_to_WB.read():ALU_inputA.read():ALU_inputA.read();

	inputB_3 = 	(rs2_en.read() == 1)?(rd3 == rd4 )?ALU_result.read():
				(rd3 == rd5 )?ALU_result_to_WB.read():ALU_inputB.read():ALU_inputB.read();
}

template<unsigned SIZE>
void Datapath<SIZE>::pipeline()
{
	rd3 = 0;
	rd4 = 0;
	rd5 = 0;
	while(1){
		wait();
		if (reset.read()){
			MEM_write_4 = 0;
			WB_5 =0;
			funct3_4 = 0;
			funct3_5 = 0;
			Wr_to_RF_3 = 0;
			rs2_data_3 = 0;
		}else{
			rd3 	   = rd;
			rd4		   = rd3;
			rd5 	   = rd4;
			Wr_to_RF_3 = Wr_to_RF;
			Wr_to_RF_5 = Wr_to_RF_3;
			WB_3 	   = WB_select;
			WB_5 	   = WB_3;
			MEM_write_4=MEM_write;
			rs2_data_3 = rs2_data;
			funct3_4 = funct3;
			funct3_5 = funct3_4;
		}
	}

}

#endif
