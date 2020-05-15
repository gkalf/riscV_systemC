#include "systemc.h"
SC_MODULE (DATA_MEMORY) {
  sc_in    <int>          address;
  sc_in    <int>          write_data;
  sc_in    <bool>         read_from_mem;
  sc_in    <bool>         write_to_mem;
  sc_out   <int>          data_to_reg;
  sc_in    <bool>          reset;

  //-----------Internal variables-------------------
  int mem [RAM_DEPTH];
  //-----------Methods------------------------------
  void  data_memory () {
    int temp_address;
    if (reset.read()){
      printf("\ninitialize data_memory\n");
      for (int i=0; i<40; i=i+4){
        mem[i]=0;
      }
    }else{
      temp_address=address.read();
      printf("\n %d \n",temp_address);
      if (read_from_mem.read()){
        data_to_reg.write(mem[temp_address]);
        printf("\n Someone read from me value = %d\n",mem[temp_address]);
      }else{
        data_to_reg.write(temp_address);
      }
    } 
  }   
void write_mem(){
  int temp_write_data=write_data.read();
  if (write_to_mem.read()){
    mem[address.read()]=temp_write_data;
  }
}
  //-----------Constructor--------------------------
  SC_CTOR(DATA_MEMORY) {
    SC_METHOD (data_memory);
      sensitive << read_from_mem << address;
    SC_METHOD (write_mem);
      sensitive << write_to_mem << write_data << address;
  } 

};
