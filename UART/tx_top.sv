/***************************************************************************     
*      
* Module: Top-Level Asynchronous Transceiver Module    
*     
* Author: Benjamin Farnsworth     
* Class: ECEN 220, Section 2, Fall 2018     
* Date: 11/28/2018     
*     
* Description: A full Top-Level Asynchronous Transceiver Module  
*     
*     
****************************************************************************/   
  
module tx_top(
input logic clk,
input logic[7:0] sw, 
input logic btnc,
output logic tx_out,
output logic[7:0] segment, anode,
output logic tx_debug);

logic send;
logic busy;

tx Transceiver(clk, send, 1'b0, sw, busy, tx_out);
assign tx_debug = tx_out;

SevenSegmentControl Display(clk, {24'b0, sw}, 8'b00000011, 8'b0, anode, segment); 

//Debounce Counter - Synchronizer 
logic tmpBtn, noisy; 
always_ff @(posedge clk) 
    begin 
        tmpBtn <= btnc; 
        noisy <= tmpBtn; 
    end 
  
//Debounce Counter - Debounce State Machine 
logic syncBtn; 
	debounce debounceFSM(clk, noisy, send);

endmodule