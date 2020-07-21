#ifndef _ALU_H
#define _ALU_H

#include "systemc.h"

template<unsigned SIZE = 32>
SC_MODULE(exec) 
{
	//Interface Ports...
	sc_in_clk clk;
	sc_in<bool>				reset;
	sc_in<sc_uint<SIZE> >	control_registers;
	sc_in<sc_uint<SIZE> >	inputA_reg_file;
	sc_in<sc_uint<SIZE> >	inputB_reg_file;
	sc_in<sc_uint<SIZE> > 	immidiate;
	sc_in<sc_uint<SIZE> >	PC_exec;
	sc_out<sc_uint<SIZE> >	PC_MEM;
	sc_out<sc_uint<SIZE> >  result;
	sc_out<bool>	        take_branch;
	sc_out<sc_uint<SIZE> >	control_registers_MEM;
	sc_in<sc_uint<SIZE> >	rs2_store_data;
	sc_out<sc_uint<SIZE> >	rs2_store_data_MEM;
	sc_out<sc_uint<SIZE> > 	immidiate_to_MEM;

	sc_in<sc_uint<32> >				control_registers_from_MEM;
	sc_in<sc_uint<32> >				control_registers_WB;
	sc_in<sc_uint<32> >				ALU_result_to_WB;
	sc_in<sc_uint<32> >				ALU_result_to_MEM;
	sc_in<bool>			stall;

	void entry();
	void forward_unit();
	void ALU_source();

	sc_signal<sc_uint<32> >	 inputA;
	sc_signal<sc_uint<32> >	 inputB;
	sc_signal<sc_uint<32> >	 inputA_reg;
	sc_signal<sc_uint<32> >	 inputB_reg;
	sc_signal<sc_uint<32> >	 inputB_to_store;
	sc_signal<sc_uint<32> >  control_registers_tmp;
	sc_signal<sc_uint<5> >   ALU_code;
	sc_signal<sc_uint<2> >	 ALU_B_src;
	sc_signal<bool>			 ALU_A_src;
	sc_signal<sc_uint<32> >  csr_val;

	sc_uint<SIZE> 	csr[4096];
	//Constructor Declaration...
	SC_CTOR(exec)
	{
		SC_CTHREAD(entry,clk.pos());

		SC_METHOD(forward_unit);
		sensitive << immidiate << control_registers << control_registers_from_MEM << control_registers_WB
		<< ALU_result_to_MEM << ALU_result_to_WB << inputA_reg_file << inputB_reg_file; 
	
		SC_METHOD(ALU_source);
		sensitive << control_registers << immidiate <<PC_exec << inputA_reg << inputB_reg;

		printf("ALU init\n");
		for (int i= 0; i<4096; i++){
			csr[i]=0;
		}
	}
};


template<unsigned DATA_WIDTH>
void exec<DATA_WIDTH>::forward_unit() 
{	
	sc_uint<5> rs1_tmp 	= control_registers.read().range(27,23);
	sc_uint<5> rs2_tmp 	= control_registers.read().range(22,18);
	sc_uint<5> rd5 		= control_registers_WB.read().range(9,5);
	sc_uint<5> rd4 		= control_registers_from_MEM.read().range(9,5);
	sc_uint<5> rd3 		= control_registers.read().range(9,5);
	bool       rd3_en 	= control_registers.read()[3];
	bool       rd4_en 	= control_registers_from_MEM.read()[3];
	bool       rd5_en 	= control_registers_WB.read()[3];
	bool       MEM_write= control_registers_MEM.read()[2];

	
		//forward data to rs2_data 3rd stage
		//add r2,rx,rx
		//sw  r2,0
		if ((rd4_en | rd5_en)){
			if ((rs2_tmp == rd4) & (rs2_tmp != 0) & rd4_en){
				inputB_to_store = ALU_result_to_MEM;
			}else if ((rs2_tmp == rd5) & (rs2_tmp != 0)  & rd5_en){
				inputB_to_store = ALU_result_to_WB;
			}else{
				inputB_to_store = rs2_store_data;
			}
		}else{
			inputB_to_store = rs2_store_data;
		}
		//forward from 4th and 5th stage
		if ((rd4_en | rd5_en)){
			if ((rs1_tmp == rd4) & (rs1_tmp != 0)  & rd4_en){
				inputA_reg = ALU_result_to_MEM;
			}else if ((rs1_tmp == rd5) & (rs1_tmp != 0) & rd5_en){
				inputA_reg = ALU_result_to_WB;
			}else{
				inputA_reg = inputA_reg_file;
			}

			if ((rs2_tmp == rd4) & (rs2_tmp != 0) & rd4_en){
				inputB_reg = ALU_result_to_MEM;
			}else if ((rs2_tmp == rd5) & (rs2_tmp != 0)  & rd5_en){
				inputB_reg = ALU_result_to_WB;
			}else{
				inputB_reg = inputB_reg_file;
			}
		}else{
			inputA_reg = inputA_reg_file;
			inputB_reg = inputB_reg_file;
		}
	
}


template<unsigned DATA_WIDTH>
void exec<DATA_WIDTH>::ALU_source()
{

	ALU_A_src = control_registers.read()[28];
	//ALU_B_src = control_registers.read().range(30,29);

	if (ALU_A_src.read() == 0){
		inputA = inputA_reg.read();
	}else if (ALU_A_src.read() == 1){
		inputA = PC_exec.read();
	}

	if (control_registers.read().range(30,29) == 0){
		inputB = inputB_reg.read();
	}else if (control_registers.read().range(30,29) == 1){
		inputB = immidiate.read();
	}else if (control_registers.read().range(30,29) == 2){
		inputB = PC_exec.read();
	}
}



template<unsigned SIZE>
void exec<SIZE>::entry() 
{	
	unsigned tmp_result;
	while(1){
		
		if (reset){
			take_branch = 0;
			result = 0;
		}else{
			int A = inputA.read();
			int B = inputB.read();
			unsigned rs1_tmp = control_registers.read().range(27,23);
			unsigned A_unint= inputA.read();
			unsigned B_uint = inputB.read();
			int inputB2 = inputB.read();
			bool ALU_code_3 = control_registers.read()[16];
			sc_uint<3> ALU_code_2_0 = control_registers.read().range(15,13); 
			bool ALU_code_4 = control_registers.read()[17];
			unsigned csr_val;
			
			if( ALU_code_3 == 1 )
			{
				inputB2 = ~(inputB.read());
			}
			else
			{
				inputB2 = inputB.read();
			};

			
			int sum = A + inputB2 + ALU_code_3;


			//printf("ina = %x, inb = %x , ALU_code = %x \n",A,B,ALU_code_2_0);


			if ((ALU_code_4,ALU_code_2_0) == 15){//CSRRCI
					csr_val = csr[inputB.read()];
					tmp_result=csr_val;
					csr[B_uint]= ~rs1_tmp & (csr_val);
					cout << "i'm in 15\n";
			}else if ((ALU_code_4,ALU_code_2_0) == 14){// CSRRSI
				csr_val = csr[inputB.read()];
					tmp_result=csr_val;
					csr[B_uint]= rs1_tmp | csr_val;
					cout << "i'm in 14\n";
			}else if ((ALU_code_4,ALU_code_2_0) == 13){	//CSRRWI
				csr_val = csr[inputB.read()];
					tmp_result= csr_val;
					csr[B_uint]= rs1_tmp;
					cout << "i'm in 13\n";
			}else if ((ALU_code_4,ALU_code_2_0) == 11){ //CSRRC
				csr_val = csr[inputB.read()];
					tmp_result = csr_val;
					csr[B_uint]= ~inputA.read() & (csr_val);
					cout << hex << "i'm in 11 " <<"\n";
			}else if ((ALU_code_4,ALU_code_2_0) == 10){	//CSRRS
				csr_val = csr[inputB.read()];
					tmp_result = csr_val;
					csr[B_uint]= inputA.read() | csr_val;
					cout << "i'm in 10\n";
			}else if ((ALU_code_4,ALU_code_2_0) == 9){	//CSRRW
				csr_val = csr[inputB.read()];
					tmp_result = csr_val;
					csr[B_uint]= inputA.read();
					cout << "i'm in 9\n";
			}else if ((ALU_code_4,ALU_code_2_0) == 7){ 
					tmp_result = A & B;
					cout << "i'm in 7\n";
			}else if ((ALU_code_4,ALU_code_2_0) == 6){
				
					tmp_result = A | B;
					cout << "i'm in 6\n";
			}else if ((ALU_code_4,ALU_code_2_0) == 5){ 
					if (ALU_code_3)
						tmp_result = A >> B; //add sra
					else
						tmp_result = A_unint >> B;
					
					cout << "i'm in 5\n";
			}else if ((ALU_code_4,ALU_code_2_0) == 4){ 
					tmp_result = A ^ B;
					cout << "i'm in 4\n";
			}else if ((ALU_code_4,ALU_code_2_0) == 3){ 
					tmp_result = (A_unint<B_uint)?1:0; //to be done
					cout << "i'm in 3\n";
			}else if ((ALU_code_4,ALU_code_2_0) == 2){ 
					tmp_result = (A<B)?1:0;
					cout << "i'm in 2\n";
			}else if ((ALU_code_4,ALU_code_2_0) == 1){
					tmp_result = A << B;
					cout << "i'm in 1\n";
			}else if ((ALU_code_4,ALU_code_2_0) == 0){
					tmp_result = sum;
					cout << "i'm in 0\n";
			}

			//calculate branch;
			if (ALU_code_2_0 == 0){
					take_branch = (sum == 0)?1:0;
			}else if (ALU_code_2_0 == 1){
					take_branch = (sum != 0)?1:0;
			}else if (ALU_code_2_0 == 4){ //blt
					take_branch = (A<B)?1:0;
			}else if (ALU_code_2_0 == 5){//bge
					take_branch = (A>=B)?1:0;
			}else if (ALU_code_2_0 == 6){
					take_branch = (A_unint<B_uint)?1:0;
			}else if (ALU_code_2_0 == 7){
					take_branch = (A_unint>=B_uint)?1:0;
			}

		}
		if (stall.read()){
			result.write(0);
			control_registers_MEM.write(0);
			rs2_store_data_MEM.write(0);
			PC_MEM.write(0);
			immidiate_to_MEM.write(0);
		}else{
			result.write(tmp_result);
			control_registers_MEM.write(control_registers);
			rs2_store_data_MEM.write(inputB_to_store);	
			PC_MEM.write(PC_exec);
			immidiate_to_MEM.write(immidiate);
		}
		wait();
	}
}

#endif
