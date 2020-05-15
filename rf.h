#include "systemc.h"

#define DATA_WIDTH        32 
#define ADDR_WIDTH        6
#define RAM_DEPTH         1 << ADDR_WIDTH

SC_MODULE (rf) {
  sc_in    <int> rs1_address;
  sc_in    <int> rs2_address;
  sc_in    <int> rd_address;
  sc_in    <bool> wr_to_reg ;
  sc_out   <int> rs1_data;
  sc_out   <int> rs2_data;
  sc_in    <int> rd_data;
  sc_in    <bool> reset;

  //-----------Internal variables-------------------
  int mem [RAM_DEPTH];

  //-----------Methods------------------------------
  void exec() { //initialize registers
    if (reset.read()){
      for (int i=0; i<32; i++){
        mem[i]=0;
      }
      printf("initialized Registers\n");

    }else{//when rs1_address is valid
      printf("rs1_address = %d\n",rs1_address.read());
      rs1_data= mem[rs1_address.read()];
    }
  }
  void read_rs2(){//when rs2_address is valid
    printf("rs2_address = %d\n",rs2_address.read());

    rs2_data = mem[rs2_address.read()];
  }
  void write_to_rf(){
  //if it is going to store from Memory
    if (wr_to_reg.read()) {
      mem[rd_address.read()] = rd_data.read();
    } 
  }

  //-----------Constructor--------------------------
  SC_CTOR(rf) {
    SC_METHOD (exec);
      sensitive << rs1_address<< reset;
    SC_METHOD (read_rs2);
      sensitive << rs2_address;
    SC_METHOD (write_to_rf);
      sensitive << rd_address<<rd_data<<wr_to_reg;
  }

};
