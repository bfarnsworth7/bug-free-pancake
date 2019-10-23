/***************************************************************************      
*       
* Module: Asynchronous Receiver Module     
*      
* Author: Benjamin Farnsworth      
* Class: ECEN 220, Section 2, Fall 2018      
* Date: 12/5/2018      
*      
* Description: A full Asynchronous Receiver Module    
*      
*      
****************************************************************************/    
   
module tx #(parameter CLK_FREQUENCY=100000000, parameter BAUD_RATE=19200) (
input logic clk,
input logic rx_in, odd,
output logic[7:0] dout,
output logic busy, error, data_strobe);

//Parameters 
localparam MAX_COUNT = CLK_FREQUENCY/BAUD_RATE; 
localparam NUM_BITS = clogb2(MAX_COUNT);

//Baud Rate Timer 
logic[NUM_BITS-1:0] counter = 0; 
logic ResetTimer = 0, EnableTimer = 0; 
logic LastCycle, HalfCycle; 
always_ff @(posedge clk) 
    begin 
        if (ResetTimer) 
            counter <= 0; 
        else if (EnableTimer) 
            counter <= counter + 1; 
    end 
     
assign LastCycle = (counter == MAX_COUNT);
assign HalfCycle = (counter == MAX_COUNT/2);

//Bit Counter 
logic[3:0] numberOfBits = 0; 
logic ResetCounter = 0, NextBit = 0; 
logic LastBit; 
always_ff @(posedge clk) 
    begin 
        if (ResetCounter) 
            numberOfBits <= 0; 
        else if (NextBit) 
            if (numberOfBits == 10) 
                numberOfBits <= 0; 
            else 
                numberOfBits <= numberOfBits + 1; 
    end 
     
assign LastBit = (numberOfBits == 10); 

//Shift Register 
logic[9:0] shiftRegister = 10'b1111111111; 
logic shift = 0;
logic stop, parity; 
always_ff @(posedge clk) 
    begin  
		if (shift) 
            shiftRegister <= {rx_in, shiftRegister[9:1]}; 
    end 
  
assign dout = shiftRegister[7:0];  
assign parity = shiftRegister[8]; 
assign stop = shiftRegister[9];

//Error Checker
logic error, expectedParity;
assign expectedParity = (^dout) ^ odd;
assign error = !((expectedParity & parity) && stop);

//FSM 
typedef enum { Idle, HalfCount, Resync, FullCount, Shift} StateType;  
StateType current_state = Idle; 
  
always_ff @(posedge clk) //IFL 
    begin 
		data_strobe <= 0;
        case (current_state) 
            Idle: 
                begin 
					if (rx_in == 0)
						current_state <= HalfCount;
					else if (rx_in == 1)
						current_state <= Idle;
                end 
            HalfCount: 
                begin 
                    if (HalfCycle == 1)
						current_state <= FullCount;
					else if (HalfCycle == 0)
						current_state <= HalfCount;
                end 
			Resync:
				begin
					current_state <= FullCount;
				end
            FullCount: 
                begin 
                    if (LastCycle == 1)
						current_state <= Shift;
					else if (LastCycle == 0)
						current_state <= FullCount;
                end 
            Shift: 
                begin 
					if (LastBit == 1) 
						begin
							current_state <= Idle;
							data_strobe <= 1;
						end
					if (LastBit == 0)
						current_state <= FullCount;
                end 
            endcase 
    end 
  
always_comb //OFL 
    begin 
        case (current_state) 
            Idle: 
                begin 
                    ResetTimer = 0; 
                    EnableTimer = 0; 
					ResetCounter = 0;
					NextBit = 0;
					shift = 0;
                end 
            HalfCount: 
                begin 
                    ResetTimer = 0; 
                    EnableTimer = 1; 
					ResetCounter = 0;
					NextBit = 0;
					shift = 0;
                end 
			Resync:
				begin
				    ResetTimer = 1; 
                    EnableTimer = 0; 
					ResetCounter = 1;
					NextBit = 0;
					shift = 0;
				end
            FullCount: 
                begin 
                    ResetTimer = 0; 
                    EnableTimer = 1; 
					ResetCounter = 0;
					NextBit = 0;
					shift = 0;
                end 
            Shift: 
                begin 
                    ResetTimer = 1; 
                    EnableTimer = 0; 
					ResetCounter = 0;
					NextBit = 1;
					shift = 1;
                end 
            endcase 
    end 
  
endmodule 
