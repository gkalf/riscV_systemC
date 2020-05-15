#include "systemc.h"

SC_MODULE(Decoder){
	sc_in<unsigned int> instruction;
	sc_out<int> rs1;
	sc_out<int> rs2;
	sc_out<int> rd;
	sc_out<int> imm_extend;
	sc_out<unsigned int> alucode;
	sc_out<bool> wr_to_rf;//write to register file
	sc_out<bool> ALU_B_source; //0->ALU, 1->sign_ext
	sc_out<bool> read_from_mem;
	sc_out<bool> write_to_mem;
	sc_out<bool> jump;
	sc_out<bool> branch;

	void decode(){
		int instr_tmp=0;
		int opcode_tmp=0;
		int rd_tmp=0;
		int func3_tmp=0;
		int rs1_tmp=0;
		int rs2_tmp=0;
		//int func2_tmp;
		int func5_tmp=0;
		int imm20u_tmp;
		int imm12_tmp;
		int imm20_tmp_1bit, imm10_1_tmp,imm11_tmp_1bit;
		int imm19_12_tmp; 
		int branch_12bit, branch_11bit, branch_10_5bit, branch_4_1bit;
		unsigned int imm12_branch = 0;
		int imm20_jump=0;
		int imm11_5_off=0;
		int imm4_0_off=0;
		int store_offset=0;

		instr_tmp  = instruction.read();
		opcode_tmp 	= (instr_tmp & 0x0000007f); 
		rd_tmp   	= (instr_tmp & 0x00000f80) >> 7; 
		func3_tmp   = (instr_tmp & 0x00007000) >> 12;
		rs1_tmp     = (instr_tmp & 0x000f8000) >> 15;
		rs2_tmp   	= (instr_tmp & 0x01f00000) >> 20;
		
		//func2_tmp  	= (instr_tmp & 0x06000000) >> 25; 
 		func5_tmp   = (instr_tmp & 0xf8000000) >> 27; 
		imm20u_tmp  = (instr_tmp & 0xfffff000);//U_type 
		imm12_tmp   = (instr_tmp & 0xfff00000) >> 20;//U_type 
		
		//jump decoding
		imm20_tmp_1bit= (instr_tmp & 0x80000000)>>12; 
		imm19_12_tmp  = (instr_tmp & 0x000FF000)>>1; 
		imm11_tmp_1bit= (instr_tmp & 0x00100000)>>10;
		imm10_1_tmp   = (instr_tmp & 0x7FE00000)>>21;
		
		// sign_extend
		if (imm20_tmp_1bit==524288){
		 	imm20_jump= (0xFFF00000+imm20_tmp_1bit + imm19_12_tmp + 
		 				 imm11_tmp_1bit + imm10_1_tmp) ;
		 }else{
			imm20_jump    = (imm20_tmp_1bit + imm19_12_tmp + 
						 imm11_tmp_1bit + imm10_1_tmp) ;
		}
//decoding branch instruction
		branch_12bit  = (instr_tmp & 0x80000000) >> 20;
		branch_11bit  = (instr_tmp & 0x00000080) << 3;
		branch_10_5bit= (instr_tmp & 0x7E000000) >> 21;
		branch_4_1bit = (instr_tmp & 0x00000F00) >> 8;
//sign_extend		
		if (branch_12bit==2048){
			imm12_branch  = 0xFFFFF000+branch_12bit + branch_11bit +
						branch_10_5bit + branch_4_1bit;
		}else{
			imm12_branch  = branch_12bit + branch_11bit +
						branch_10_5bit + branch_4_1bit;
		}

//decoding store instruction
		imm11_5_off = (instr_tmp & 0xfe000000) >> 18;
		imm4_0_off  = (instr_tmp & 0x00000f80) >> 7;

		store_offset = imm11_5_off + imm4_0_off;

		printf("\n****************\n");
		printf("instruction  %x \n",instr_tmp);
		printf("\n****************\n");
		printf("opcode_tmp   %x \n",opcode_tmp);
		printf("func3_tmp    %x \n",func3_tmp);
		printf("rd_tmp       %x \n",rd_tmp);
		printf("rs1_tmp      %x \n",rs1_tmp);
		printf("rs2_tmp      %x \n",rs2_tmp);
		printf("func5_tmp    %x \n",func5_tmp);
		printf("imm20u_tmp   %x \n",imm20u_tmp);
		printf("imm12_tmp    %x \n",imm12_tmp);
		printf("imm20_jump   %x \n",imm20_jump);
		printf("imm12_branch %x \n",imm12_branch);
		printf("store_offset %x \n",store_offset);

		switch(opcode_tmp){
			case 0x33: //R-type
				rs1.write(rs1_tmp);
				rs2.write(rs2_tmp);
				rd.write(rd_tmp);
				wr_to_rf.write(true);
				ALU_B_source.write(false);
				read_from_mem.write(false);
				write_to_mem.write(false);
				branch.write(false);
				jump.write(false);
				if (func3_tmp==0){ //add
						if (func5_tmp == 0x8){
							alucode.write(8);
						}else{
							alucode.write(0);
						}
				}else if (func3_tmp==1){ //sll
						alucode.write(1);
				}else if (func3_tmp==2){ //slt
						alucode.write(2);
				}else if (func3_tmp==3){ //slt
						alucode.write(3);
				}else if (func3_tmp==4){  //xor
						alucode.write(4);
				}else if (func3_tmp==5){ //slt
						if (func5_tmp == 0x8){//sra
							alucode.write(13);
						}else{//srl
							alucode.write(5);
						}
				}else if (func3_tmp==6){ //or
						alucode.write(6);
				}else if (func3_tmp==7){ //and
						alucode.write(7);
				}	
				break;
			case 0x13://functions immidiate  
				rs1.write(rs1_tmp);
				rs2.write(0);
				imm_extend.write(imm12_tmp);
				rd.write(rd_tmp);
				wr_to_rf.write(true);
				ALU_B_source.write(true);
				read_from_mem.write(false);
				write_to_mem.write(false);
				jump.write(false);
				branch.write(false);

				if (func3_tmp==0){ //add
					alucode.write(0);
				}else if (func3_tmp==2){ //slt
					alucode.write(2);
				}else if (func3_tmp==3){ //sltu
					alucode.write(3);
				}else if (func3_tmp==4){  //xor
					alucode.write(4);
				}else if (func3_tmp==6){ //slt
					alucode.write(6);
				}else if (func3_tmp==7){ //slt
					alucode.write(7);
				}
				break;
			case 0x37: //LUI, 0+20'bx(upper)
				rs1.write(0);
				imm_extend.write(imm20u_tmp);
				rd.write(rd_tmp);
				wr_to_rf.write(true);
				ALU_B_source.write(true);
				read_from_mem.write(false);
				write_to_mem.write(false);
				alucode.write(0);
				jump.write(false);
				branch.write(false);
				break;
			case 0x03: //LW
				rs1.write(rs1_tmp);
				imm_extend.write(imm12_tmp);
				rd.write(rd_tmp);
				wr_to_rf.write(true);
				ALU_B_source.write(true);
				read_from_mem.write(true);
				write_to_mem.write(false);
				alucode.write(0);
				jump.write(false);
				branch.write(false);
				if (func3_tmp){
					//send code for 32bit
				}
				break;
			case 0x23: //SW
				rs1.write(rs1_tmp);
				rs2.write(rs2_tmp);
				imm_extend.write(store_offset);
				rd.write(rd_tmp);
				wr_to_rf.write(false);
				ALU_B_source.write(true);
				read_from_mem.write(false);
				write_to_mem.write(true);
				jump.write(false);
				branch.write(false);
				alucode.write(0);
				if (func3_tmp){
					//send code for 32bit
				}
				break;
			case 0x6F: //jal
				rs1.write(rs1_tmp);
				imm_extend.write(imm20_jump);
				rd.write(rd_tmp);
				wr_to_rf.write(false);
				ALU_B_source.write(true);
				read_from_mem.write(false);
				write_to_mem.write(false);
				jump.write(true);
				branch.write(false);
				alucode.write(0);
				break;
			case 0x63: //branch
				rs1.write(rs1_tmp);
				rs2.write(rs2_tmp);
				imm_extend.write(imm12_branch);
				wr_to_rf.write(false);
				ALU_B_source.write(false);
				read_from_mem.write(false);
				write_to_mem.write(false);
				jump.write(false);
				branch.write(true);
				if (func3_tmp == 0){
					alucode.write(8);

				}
				break;
		}
	}
	SC_CTOR(Decoder){
		SC_METHOD(decode);
		sensitive << instruction;
	}
};
