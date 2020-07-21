#ifndef _WB_H
#define _WB_H

#include "systemc.h"
#include "Mux_4-1_MUX_4_1.h"
#include "MUX_2-1_MUX_2_1.h"
#include "immidiate_extend_sign_extend.h"

template<unsigned SIZE = 32>
SC_MODULE(WB)
{

	sc_in<bool> WB_select;
	sc_in<sc_uint<SIZE> >  	r_data;
	sc_in<sc_uint<SIZE> > 	ALU_result;
	sc_in<sc_uint<3> >	   	funct3;
	sc_out<sc_uint<SIZE> > 	rd_data;
	
	sc_signal<sc_uint<32> >	Decode_to_Sign_Extend_byte;
	sc_signal<sc_uint<32> >	Decode_to_Sign_Extend_Half_Word;
	sc_signal<sc_uint<32> >	sign_extended_byte;
	sc_signal<sc_uint<32> >	sign_extended_half_word;
	sc_signal<sc_uint<32> >	data_to_WB;
	sc_signal<sc_uint<32> >	ALU_or_Imm;
	sc_signal<sc_uint<32> > B_16_0_r_data_31_16__concat;
	sc_signal<sc_uint<32> > B_16_0_r_data_15_0__concat;
	sc_signal<bool> ALU_result_1;
	sc_signal<bool> funct3_un_neg_exp;
	sc_signal<sc_uint<32> > B_24_0_r_data_7_0__concat;
	sc_signal<sc_uint<32> > B_24_0_r_data_15_8__concat;
	sc_signal<sc_uint<32> > B_24_0_r_data_23_16__concat;
	sc_signal<sc_uint<32> > B_24_0_r_data_31_24__concat;
	sc_signal<sc_uint<2> > ALU_result_1_0;
	sc_signal<sc_uint<2> > funct3_1_0;
	sc_signal<sc_uint<32> > DECIMAL_0;

	MUX_2_1<32>	Load_Half_Word;
	sign_extend<32,16>	sign_extend_Half_Word;
	MUX_4_1<32>	Load_Byte;
	sign_extend<32,8>	sign_extend_byte;
	MUX_4_1<32>	MUX_4_1_select_Data_to_WB;
	MUX_2_1<32>	ALU_or_Imm_SEL_Write_Back;


	void signal_handler()
	{
		DECIMAL_0 = 0;
		B_16_0_r_data_31_16__concat = (sc_uint<16>(0), r_data.read().range(31,16));
		B_16_0_r_data_15_0__concat = (sc_uint<16>(0), r_data.read().range(15,0));
		ALU_result_1 = ALU_result.read()[1];
		funct3_un_neg_exp = !(funct3.read()[2]);
		B_24_0_r_data_7_0__concat = (sc_uint<24>(0), r_data.read().range(7,0));
		B_24_0_r_data_15_8__concat = (sc_uint<24>(0), r_data.read().range(15,8));
		B_24_0_r_data_23_16__concat = (sc_uint<24>(0), r_data.read().range(23,16));
		B_24_0_r_data_31_24__concat = (sc_uint<24>(0), r_data.read().range(31,24));
		ALU_result_1_0 = ALU_result.read().range(1,0);
		funct3_un_neg_exp = !(funct3.read()[2]);
		funct3_1_0 = funct3.read().range(1,0);
	}

	SC_CTOR(WB) :
		Load_Half_Word("LOAD_HALF_WORD") ,
		sign_extend_Half_Word("SIGN_EXTEND_HALF_WORD") ,
		Load_Byte("LOAD_BYTE") ,
		sign_extend_byte("SIGN_EXTEND_BYTE") ,
		MUX_4_1_select_Data_to_WB("MUX_4_1_SELECT_DATA_TO_WB") ,
		ALU_or_Imm_SEL_Write_Back("ALU_OR_IMM_SEL_WRITE_BACK")

	{
		Load_Half_Word(B_16_0_r_data_31_16__concat,B_16_0_r_data_15_0__concat,ALU_result_1,Decode_to_Sign_Extend_Half_Word);

		sign_extend_Half_Word(Decode_to_Sign_Extend_Half_Word,funct3_un_neg_exp,sign_extended_half_word);

		Load_Byte(B_24_0_r_data_7_0__concat,B_24_0_r_data_15_8__concat,B_24_0_r_data_23_16__concat,B_24_0_r_data_31_24__concat,ALU_result_1_0,Decode_to_Sign_Extend_byte);

		sign_extend_byte(Decode_to_Sign_Extend_byte,funct3_un_neg_exp,sign_extended_byte);

		MUX_4_1_select_Data_to_WB(sign_extended_byte,sign_extended_half_word,r_data,DECIMAL_0,funct3_1_0,data_to_WB);

		ALU_or_Imm_SEL_Write_Back(data_to_WB,ALU_result,WB_select,rd_data);

		SC_METHOD(signal_handler);
		sensitive << ALU_result << funct3 << r_data;
	}
};

#endif	
