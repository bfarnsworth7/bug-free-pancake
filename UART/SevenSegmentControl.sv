/***************************************************************************  
*   
* Module: Seven Segment Control  
*  
* Author: Benjamin Farnsworth  
* Class: ECEN 220, Section 2, Fall 2018  
* Date: 10/31/2018  
*  
* Description: A full Seven Segment Display
*  
*  
****************************************************************************/  

  
module SevenSegmentControl(  
input logic clk,
input logic[31:0] dataIn,
input logic[7:0] digitDisplay, digitPoint,
output logic[7:0] anode, segment);

logic[16:0] counter = 17'b0;
//Sequencing Counter
always_ff @(posedge clk)
begin
	counter <= counter + 1;
end

logic[2:0] digitSelect;
assign digitSelect = counter[16:14];
logic[7:0] binarySelect;
logic[3:0] dataOut;
logic DP;
//Anode Logic - a 3:8 decoder for the digitSelect signal
always_comb begin
	case(digitSelect)
		0:begin
			binarySelect = 8'b00000001;
			dataOut = dataIn[3:0];
			DP = digitPoint[0];
			end
		1:begin
			binarySelect = 8'b00000010;
			dataOut = dataIn[7:4];
			DP = digitPoint[1];
			end
		2:begin
			binarySelect = 8'b00000100;
			dataOut = dataIn[11:8];
			DP = digitPoint[2];
			end
		3:begin 
			binarySelect = 8'b00001000;
			dataOut = dataIn[15:12];
			DP = digitPoint[3];
			end
		4:begin
			binarySelect = 8'b00010000;
			dataOut = dataIn[19:16];
			DP = digitPoint[4];
			end
		5:begin
			binarySelect = 8'b00100000;
			dataOut = dataIn[23:20];
			DP = digitPoint[5];
			end
		6:begin 
			binarySelect = 8'b01000000;
			dataOut = dataIn[27:24];
			DP = digitPoint[6];
			end
		default:begin
			binarySelect = 8'b10000000;
			dataOut = dataIn[31:28];
			DP = digitPoint[7];
			end
	endcase
end

//Anode Logic - AND that signal with digitDisplay, invert the resulting signal
assign anode = ~(binarySelect & digitDisplay);

//Cathode Logic - Seven Segment Decoder
assign segment[7] = !DP;
assign segment[6:0] =  
	(dataOut == 0) ? 7'b1000000 :  
	(dataOut == 1) ? 7'b1111001 :  
	(dataOut == 2) ? 7'b0100100 :  
	(dataOut == 3) ? 7'b0110000 :  
	(dataOut == 4) ? 7'b0011001 :  
	(dataOut == 5) ? 7'b0010010 :  
	(dataOut == 6) ? 7'b0000010 :  
	(dataOut == 7) ? 7'b1111000 :  
	(dataOut == 8) ? 7'b0000000 :  
	(dataOut == 9) ? 7'b0010000 :  
	(dataOut == 10) ? 7'b0001000 :  
	(dataOut == 11) ? 7'b0000011 :  
	(dataOut == 12) ? 7'b1000110 :  
	(dataOut == 13) ? 7'b0100001 :  
	(dataOut == 14) ? 7'b0000110 :  
	7'b0001110; 
	
endmodule

