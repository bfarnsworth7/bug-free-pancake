/***************************************************************************    
*     
* Module: debounce Module    
*    
* Author: Benjamin Farnsworth    
* Class: ECEN 220, Section 2, Fall 2018    
* Date: 11/7/2018    
*    
* Description: A full debouncer
*    
*    
****************************************************************************/  

module debounce(
input logic clk, noisy,
output logic debounced);

typedef enum {S0, S1, S2, S3} StateType;
StateType current_state = S0;

logic timerDone;

//IFL and State Memory
always_ff @(posedge clk)
	case (current_state)
		S0: if (noisy) 
				current_state <= S1;
		S1: if (~noisy) 
				current_state <= S0;
			else if (noisy && timerDone) 
				current_state <= S2;
		S2: if (~noisy)
				current_state <= S3;
		S3: if (noisy)
				current_state <= S2;
			else if (~noisy && timerDone)
				current_state <= S0;
	endcase

logic clrTimer;
	
//OFL
always_comb 
begin
	clrTimer = 0;
	debounced = 0;
	case (current_state)
		S0: clrTimer = 1;
		S2: begin
				debounced = 1;
				clrTimer = 1;
			end
		S3: debounced = 1;
	endcase
end
		
//Timer
logic[18:0] expandedClk;

always_ff @(negedge clk)
	if (clrTimer) 
	    begin
		     expandedClk <= 0;
		     timerDone = 0;
		end
	else 
	    begin
		expandedClk <= expandedClk + 1;
	    if (expandedClk == 19'd499999)
			timerDone = 1;
		end

endmodule