#ifndef _DECODE_H
#define _DECODE_H

#include "systemc.h"
#include "stdlib.h"

template<unsigned DATA_WIDTH = 32>
SC_MODULE(Decode) 
{
	//Interface Ports...
	sc_in_clk						clk;
	sc_in<bool>						reset;
	sc_in<sc_uint<32> >				instruction;
	sc_in<sc_uint<DATA_WIDTH> > 	PC_dec;
	sc_in<bool>                 	rd_en;
	sc_in <sc_uint<DATA_WIDTH> > 	rd_data;
	sc_in <sc_uint<5> >				rd_address;
	sc_in<bool>						stall_j;
	sc_in<sc_uint<32> >				PC;

	sc_out<sc_uint<32> >		    control_registers;
	sc_out<sc_uint<32> >		    PC_exec;
	sc_out<sc_uint<DATA_WIDTH> > 	immidiate_sign_extended;
	sc_out<sc_uint<32> > 			inputA_reg_file;
	sc_out<sc_uint<32> > 			inputB_reg_file;
	sc_out<sc_uint<32> > 			rs2_store_data;
	sc_out<bool>					stall;
	
	//Internal Signals...
	sc_signal<sc_uint<3> >		funct3;
	sc_signal<sc_uint<7> >		opcode;
	sc_signal<sc_uint<7> >		funct7;
	sc_signal<sc_uint<32> >		I_imm;
	sc_signal<sc_uint<32> >		S_imm;
	sc_signal<sc_uint<32> >		B_imm;
	sc_signal<sc_uint<32> >		U_imm;
	sc_signal<sc_uint<32> >		J_imm;
	sc_signal<sc_uint<32> >		imm_tmp;
	
	sc_signal<bool>			ALU_A_src;
	sc_signal<sc_uint<2> >  ALU_B_src;
	sc_signal<bool>			Wr_to_RF;
	sc_signal<bool>			WB_select;
	sc_signal<bool>			jump;
	sc_signal<bool>			jump_r;
	sc_signal<bool>			MEM_write;
	sc_signal<bool>			Branch;
	sc_signal<bool>			rs1_en;
	sc_signal<bool>			rs2_en;

	sc_signal<sc_uint<5> >	rs1_tmp;
	sc_signal<sc_uint<5> >	rs2_tmp;
	sc_signal<sc_uint<5> >	rd;
	sc_signal<sc_uint<5> >  ALU_code;

	sc_uint<DATA_WIDTH> Reg_file[32];

	//Processes Declaration...
	void stall_unit();
	void instruction_decoder();
	void control_unit();
	void RF();
	void forward_unit();
	//Constructor Declaration...
	SC_CTOR(Decode)
	{
		SC_METHOD(control_unit);
		sensitive << opcode << funct3 << funct7 << I_imm << S_imm << B_imm << U_imm << J_imm;

		SC_METHOD(stall_unit);
		sensitive << rs1_tmp << rs2_tmp << control_registers;

		SC_CTHREAD(RF,clk.pos());

		SC_METHOD(instruction_decoder);
		sensitive << instruction;
  }
};


template<unsigned DATA_WIDTH>
void Decode<DATA_WIDTH>::control_unit() 
{
	
	ALU_code = 0;
	Wr_to_RF = 0;
	jump = 0;
	Branch = 0;
	Wr_to_RF = 0;
	ALU_A_src = 0;
	ALU_B_src = 0;
	rs1_en = 0;
	rs2_en = 0;
	MEM_write = 0;
	WB_select = 0;
	imm_tmp = 0;

	switch( opcode.read() )
	{
		case 0x37 : //lui
			ALU_B_src = 1;
			Wr_to_RF = 1;
			imm_tmp = U_imm;
			break;
		case 0x17 : //auipc
			ALU_A_src = 1;
			ALU_B_src = 1;
			Wr_to_RF = 1;
			imm_tmp = U_imm;
			break;
		case 0x6f : // jump
			jump = 1;
			Wr_to_RF = 1;
			imm_tmp = J_imm;
			break;
		case 0x67 : //jalr
			Wr_to_RF = 1;
			jump_r = 1;
			rs1_en = 1;
			ALU_B_src = 1;
			imm_tmp = I_imm;
			break;
		case 0x63 : //branch
			Branch = 1;
			rs1_en = 1;
			rs2_en = 1;
			imm_tmp = B_imm;
			break;
		case 0x03 : //load immidiate
			ALU_B_src = 1;
			Wr_to_RF = 1;
			rs1_en = 1;
			WB_select = 1;
			imm_tmp = I_imm;
			break;
		case 0x23 : 
			ALU_B_src = 1;
			rs1_en = 1;
			rs2_en = 1;
			MEM_write = 1;
			imm_tmp = S_imm;
			break;
		case 0x13 : 
			ALU_A_src = 0;
			ALU_B_src = 1;
			Wr_to_RF = 1;
			rs1_en = 1;
			imm_tmp = I_imm;
			if( (funct3.read() == 2) | (funct3.read() == 3) ) 
			{
				ALU_code = (sc_uint<1>(1), funct3.read());
			}
			else
			{
				if( funct3.read() == 5 ) 
				{
					ALU_code = (funct7.read()[5], funct3.read());
				}
				else
				{
					ALU_code = (sc_uint<1>(0), funct3.read());
				}
			}
			break;
		case 0x33 : 
			Wr_to_RF = 1;
			rs1_en = 1;
			rs2_en = 1;
			if( (funct3.read() == 2) | (funct3.read() == 3) ) 
			{
				ALU_code = (sc_uint<1>(1), funct3.read());
			}else
			{
				if( (funct3.read() == 5) | (funct3.read() == 0)) 
				{
					ALU_code = (funct7.read()[5], funct3.read());
				}
				else
				{
					ALU_code = (sc_uint<1>(0), funct3.read());
				}
			}
			break;
		case 0x73 :
			Wr_to_RF = 1;
			imm_tmp = I_imm;
			if ( (funct3.read()[2] == 1)){
				ALU_code =(sc_uint<1>(1),sc_uint<1>(0), funct3.read());
				ALU_B_src = 1;
			}else{
				ALU_code = (sc_uint<1>(1),sc_uint<1>(0), funct3.read());
				rs1_en = 1;
				ALU_B_src = 1;
			}
			break;
	}
}
	


template<unsigned DATA_WIDTH>
void Decode<DATA_WIDTH>::RF() 
{
	while(1){
			if( rd_en.read() ){
				Reg_file[rd_address.read()] = rd_data.read();
				Reg_file[0] = 0;
			}else{
					Reg_file[0] = 0;
			}


		if ((stall.read() == 1) | stall_j.read()== 1){
			inputA_reg_file = 0;
			inputB_reg_file = 0;
			immidiate_sign_extended = 0;
			rs2_store_data = 0;
			PC_exec = 0;
			control_registers = 0;
		 }else{
		 	
			inputA_reg_file = (rs1_en == 1)?Reg_file[rs1_tmp.read()]:(sc_uint<32>(0));
			inputB_reg_file = (rs2_en == 1)?Reg_file[rs2_tmp.read()]:(sc_uint<32>(0));

			
			immidiate_sign_extended = imm_tmp;
			// rs1_data = (rs1_en == 1)?Reg_file[rs1_tmp.read()]:(sc_uint<32>(0));
			if (jump.read() | jump_r.read() ){
				rs2_store_data = PC_dec;
			}else{
				rs2_store_data = (rs2_en == 1)?Reg_file[rs2_tmp.read()]:(sc_uint<32>(0));
			}

			PC_exec = PC;
			control_registers = (jump_r.read(), ALU_B_src.read() ,ALU_A_src.read() ,rs1_tmp.read(),rs2_tmp.read(),ALU_code.read(),funct3.read(),rd.read(),
								Branch.read(),Wr_to_RF.read(),MEM_write.read(),WB_select.read(),jump.read());
		}	/*
jump_r |B_src | A_src |	rs1 | rs2 |	ALU_code | funct3 | rd | Branch | Wr_to_RF | MEM_write | WB_select | jump 
   1   |  2   |   1   |  5  |  5  |    5     |    3   |  5 |    1   |     1    |     1     |      1    |   1       																										

		}	*/
		wait();
	}
}

template<unsigned DATA_WIDTH>
void Decode<DATA_WIDTH>::stall_unit()
{

	sc_uint<5> rd3 		= control_registers.read().range(9,5);
	bool       rd3_en 	= control_registers.read()[3];
	bool       WB_select3=control_registers.read()[1];
	bool       MEM_write3=control_registers.read()[2];
	//LOAD r2,0
	//add  r2,0
	if ((WB_select3) & ((rs2_tmp.read() == rd3) | (rs1_tmp.read() == rd3)) & ((rs1_tmp.read() !=0) & (rs2_tmp.read() != 0)) & rd3_en){
		stall = 1;
	}else{
		stall = 0;
	}

}
template<unsigned DATA_WIDTH>
void Decode<DATA_WIDTH>::instruction_decoder()
{
	opcode  = instruction.read().range(6,0);
	rs1_tmp = instruction.read().range(19,15);
	rs2_tmp = instruction.read().range(24,20);
	rd 		= instruction.read().range(11,7);
	funct7 	= instruction.read().range(31,25);
	funct3 	= instruction.read().range(14,12);
	I_imm 	= ((instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31]), instruction.read().range(31,20));
	S_imm 	= ((instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31]), instruction.read().range(31,25), instruction.read().range(11,7));
	B_imm 	= ((instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31]), instruction.read()[31], instruction.read()[7], instruction.read().range(30,25), instruction.read().range(11,8),sc_uint<1>(0));
	U_imm 	= (instruction.read().range(31,12), sc_uint<12>(0));
	J_imm 	= ((instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31], instruction.read()[31]), instruction.read()[31], instruction.read().range(19,12), instruction.read()[20], instruction.read().range(30,21),sc_uint<1>(0));	

}
#endif
