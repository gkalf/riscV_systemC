#include "systemc.h"
SC_MODULE(STATE_MACHINE){
sc_in_clk clock;
sc_in<bool> reset;
sc_in<bool> start;
sc_in<sc_uint<32> > a;
sc_in<sc_uint<32> > b;
sc_out<sc_uint<32> > c;

sc_signal<sc_uint<2> > state,n_state;

  void sequential_part(){
      state = 0;
      while(1){
          if (reset)
              state =0;
          else
              state = n_state;
     }
  }
  void combinational_part(){
      c=0;
      if (state.read() == 0){
          if (start.read()){
              n_state = 1;
          }else{
              n_state = 0;
          }
      }else if (state.read() == 1){
          c = a.read();
          n_state = 2;
      }else if(state.read() == 2){
           c = b.read();
           n_state = 3;
      }else if(state.read() == 3){
           c=(a.read().range(15,0), b.read().range(15,0));
           n_state = 0;
      }
  }
  SC_CTOR(STATE_MACHINE){
      SC_METHOD(combinational_part);
    sensitive << a << b << state << start;
    
    SC_CTHREAD(sequential_part,clock.pos());
  }
};
