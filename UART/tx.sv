/***************************************************************************     
*      
* Module: Asynchronous Transceiver Module    
*     
* Author: Benjamin Farnsworth     
* Class: ECEN 220, Section 2, Fall 2018     
* Date: 11/28/2018     
*     
* Description: A full debouncer 
*     
*     
****************************************************************************/   
  
module tx #(parameter CLK_FREQUENCY=100000000, parameter BAUD_RATE=19200) (
input logic clk, send, odd,
input logic[7:0] din,
output logic busy, tx_out);

//Parameters
localparam MAX_COUNT = CLK_FREQUENCY/BAUD_RATE;
localparam NUM_BITS = clogb2(MAX_COUNT);

//Baud Rate Timer
logic[NUM_BITS-1:0] counter = 0;
logic ResetTimer = 0, EnableTimer = 0;
logic LastCycle;
always_ff @(posedge clk)
	begin
		if (ResetTimer)
			counter <= 0;
		else if (EnableTimer)
			counter <= counter + 1;
	end
	
assign LastCycle = (counter + 1 == 0);

//Parity Generator
logic parity;
assign parity = (^din) ^ odd;

//Bit Counter
logic numberOfBits = 0;
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
logic load = 0, shift = 0;
always_ff @(posedge clk)
	begin 
		if (load)
			shiftRegister <= {parity, din, 1'b0};
		else if (shift)
			shiftRegister <= {1'b1, shiftRegister[9:1]};
	end
	
assign tx_out = shiftRegister[0];

//FSM
typedef enum { Idle, Load, Count, Shift, Wait } StateType; 
StateType current_state = Idle;

always_ff @(posedge clk) //IFL
	begin
		case (current_state)
			Idle:
				begin
					if (send)
						current_state <= Load;
					if (!send)
						current_state <= Idle;
				end
			Load:
				begin
					current_state <= Count;
				end
			Count:
				begin
					if (LastCycle)
						current_state <= Shift;
					if (!LastCycle)
						current_state <= Count;
				end
			Shift:
				begin
					if (LastBit)
						current_state <= Wait;
					if (!LastBit)
						current_state <= Count;
				end
			Wait:
				begin
					if (send)
						current_state <= Wait;
					if (!send)
						current_state <= Idle;
				end
			endcase
	end

always_comb //OFL
	begin
		case (current_state)
			Idle:
				begin
					load = 0;
					busy = 0;
					ResetCounter = 0;
					ResetTimer = 0;
					EnableTimer = 0;
					shift = 0;
					NextBit = 0;
				end
			Load:
				begin
					load = 1;
					busy = 1;
					ResetCounter = 1;
					ResetTimer = 1;
					EnableTimer = 0;
					shift = 0;
					NextBit = 0;
				end
			Count:
				begin
					load = 0;
					busy = 1;
					ResetCounter = 0;
					ResetTimer = 0;
					EnableTimer = 1;
					shift = 0;
					NextBit = 0;
				end
			Shift:
				begin
					load = 0;
					busy = 1;
					ResetCounter = 0;
					ResetTimer = 1;
					EnableTimer = 0;
					shift = 1;
					NextBit = 1;
				end
			Wait:
				begin
					load = 0;
					busy = 0;
					ResetCounter = 0;
					ResetTimer = 0;
					EnableTimer = 0;
					shift = 0;
					NextBit = 0;
				end
			endcase
	end

endmodule


// clogb2
// 
// This function will compute the "ceiling" of the log base 2 of
// a number. This function is often used to determine how many
// bits are needed to represent a given number.
//
function integer clogb2;
  input [31:0] value;
  begin
    value = value - 1;
    for (clogb2 = 0; value > 0; clogb2 = clogb2 + 1) begin
      value = value >> 1;
    end
  end
endfunction