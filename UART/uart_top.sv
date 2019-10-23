/***************************************************************************      
*       
* Module: Asynchronous Receiver Module top
*      
* Author: Benjamin Farnsworth      
* Class: ECEN 220, Section 2, Fall 2018      
* Date: 12/5/2018      
*      
* Description: A full Asynchronous Receiver Module    
*      
*      
****************************************************************************/    
   
module uart_top(
input logic clk,
input logic[7:0] sw,
input logic btnc,
input logic rx_in,
output logic tx_out, rx_error, tx_debug, rx_debug,
output logic[7:0] segment, anode);

logic send; 
logic busy, busy2, strobe; 
  
tx Transceiver(clk, send, 1'b0, sw, busy, tx_out); 
assign tx_debug = tx_out; 
  
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
  
logic rx_data;
rx Receiver(clk, rx_in, 1'b0, rx_data, busy2, rx_error, strobe );
assign rx_debug = rx_in;

SevenSegmentControl Display(clk, {8'b0, rx_data, 8'b0, sw}, 8'b00110011, 8'b0, anode, segment);  
  
endmodule 