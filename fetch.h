//#ifndef RAM_DP_AR_AW
//#define RAM_DP_AR_AW
#include "systemc.h"

#define DATA_WIDTH        32 
#define ADDR_WIDTH        6 
#define RAM_DEPTH         1 << ADDR_WIDTH

SC_MODULE (FETCH) {
  sc_in    <int> PC_address;
  sc_out   <unsigned int> instruction;
  sc_in    <bool>          reset;

  //-----------Internal variables-------------------
  int mem [RAM_DEPTH];

  //-----------Methods------------------------------
  void  fetch () {
    
    if (reset.read()){  
      mem[0]=0x00108093;
      mem[4]=0xfffff237;
      mem[8]=0x800002b7;
      mem[12]=0x00527333;
      mem[16]=0x00030863;
      mem[20]=0x00110113;
      mem[24]=0x00121233;
      mem[28]=0xff1ff06f;
      mem[32]=0x00202023;
      mem[36]=0x00002103;
      printf("\ninstruction memory initialized \n");
    }else{
      instruction.write(mem[PC_address.read()]);
    }
  }  

  //-----------Constructor--------------------------
  SC_CTOR(FETCH) {
    SC_METHOD (fetch);
      sensitive << PC_address<<reset;
  } 

};
