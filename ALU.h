#include "systemc.h"

SC_MODULE(ALU){
	//sc_in<bool> reset;
	sc_in<int> inA,inB;
	sc_in<unsigned int> alucode;
	sc_out<int> Dout;
	sc_out<bool> Z;

	void exec(){
		int inA_tmp,inB_tmp,out_tmp;
		int alucode_tmp;
		inA_tmp=inA.read();
		inB_tmp=inB.read();
		alucode_tmp=alucode.read();
		printf("ALU: inA= %d,inB= %d, alucode= %u",inA_tmp,inB_tmp,alucode_tmp);

		if (alucode_tmp == 0){//ADD
			out_tmp=inA_tmp+inB_tmp;
		}else if (alucode_tmp == 8){//SUB
			out_tmp=inA_tmp-inB_tmp;
		}else if (alucode_tmp == 1){//SLL
			out_tmp=inA_tmp<<inB_tmp;
		}else if (alucode_tmp == 2){//SLT 
			out_tmp=inA_tmp-inB_tmp;
		}else if (alucode_tmp == 3){//SLTU
			out_tmp=inA_tmp-inB_tmp;
		}else if (alucode_tmp == 8){//SLL
			out_tmp=inA_tmp<<inB_tmp;
		}else if (alucode_tmp == 4){//XOR
			out_tmp=inA_tmp ^ inB_tmp;
		}else if (alucode_tmp == 5){//SRL
			out_tmp=inA_tmp>>inB_tmp;
		}else if (alucode_tmp == 13){//SRA
			out_tmp=inA_tmp >> inB_tmp;
		}else if (alucode_tmp == 6){//OR
			out_tmp=inA_tmp | inB_tmp;
		}else if (alucode_tmp == 7){//AND
			out_tmp=inA_tmp & inB_tmp;
		}else{
			
		}

		if (out_tmp == 0){
			Z.write(true);
		}else{
			Z.write(false);
		}
		Dout.write(out_tmp);
		printf("result = %x \n", out_tmp);
	}
	SC_CTOR(ALU){
		SC_METHOD(exec);
		sensitive << inA << inB << alucode;
	}

	

};