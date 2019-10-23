`timescale 1ns / 100ps
//////////////////////////////////////////////////////////////////////////////////
//
//  Filename: tb_rx.v
//
//  Author: Mike Wirthlin
//  
//  Description: 
//
//  Version 1.0
//
//  Change Log:
//   
//////////////////////////////////////////////////////////////////////////////////

module tb_rx();

	reg tb_clk, tb_rx_in, tb_odd;
    wire tb_busy, tb_error, tb_data_strobe;
	wire [7:0] tb_dout;
	integer errors,b_errors, i;
	reg [7:0] tb_received_data;
	reg tb_received_error;
	time tb_receive_time;
	
	localparam CLK_FREQUECY = 100000000;
	localparam BAUD_RATE = 19200;
	localparam CLOCKS_PER_BIT = CLK_FREQUECY / BAUD_RATE;

	task simulate_rx_transfer;
		input [7:0] data_to_transfer;
		input parity;
		input parity_error;
		begin

			b_errors = errors;

			// Wait for negative edge of clock and then start setting inputs
			@(negedge tb_clk);
			tb_odd = parity;
			repeat (8) @(negedge tb_clk);

			tb_rx_in = 0;

			@(negedge tb_clk);
			// Busy should be high now
			if (~tb_busy) begin
				$display("*** ERROR: busy did not go high at time %0t ***", $time);
				errors = errors + 1;
			end
			// Wait for a full bit period for start bit
			repeat (CLOCKS_PER_BIT - 1) @(negedge tb_clk);
			// transmit each of the data bits
			for (i=0;i<8;i=i+1) begin
				tb_rx_in = data_to_transfer[i];
				// Wait for a bit period and check against expected data
				repeat (CLOCKS_PER_BIT) @(negedge tb_clk);
			end
			// set parity bit
			tb_rx_in = (^data_to_transfer) ^ parity ^ parity_error;	
			repeat (CLOCKS_PER_BIT) @(negedge tb_clk);
			
			// set stop bit (half of the stop bit)
			tb_rx_in = 1;
			repeat (CLOCKS_PER_BIT/2) @(negedge tb_clk);

			// Busy should still be high now
			if (~tb_busy) begin
				$display("*** ERROR: busy is low before end of stop bit at time %0t ***", $time);
				errors = errors + 1;
			end

			// set rest of stop bit 
			repeat (CLOCKS_PER_BIT/2) @(negedge tb_clk);

			// should be done. Wait a few clocks and then see if the data strobe sent the correct data
			repeat (100) @(negedge tb_clk);
			if (tb_received_data != data_to_transfer) begin
				$display("*** ERROR: received %h but expecting %h at time %0t (message at time %0t) ***", 
					tb_received_data, data_to_transfer, tb_receive_time, $time);
				errors = errors + 1;
			end
			
			if (tb_received_error != parity_error) begin
				if (~parity_error)
					$display("*** ERROR: error signal given but no error in data at time %0t (message at time %0t) ***", 
						tb_receive_time, $time);
				else
					$display("*** ERROR: no error signal given but error in data at time %0t (message at time %0t) ***", 
						tb_receive_time, $time);				
				errors = errors + 1;
			end

			if (b_errors == errors) begin
				$display("Successfully received 0x%h at time %0t ***", 
					tb_received_data, tb_receive_time);				
			end
			
		end
	endtask

	// Instance the DUT
	rx #(.CLK_FREQUENCY(CLK_FREQUECY), .BAUD_RATE(BAUD_RATE)) 
	   student_xx(.clk(tb_clk), .rx_in(tb_rx_in), .odd(tb_odd), .error(tb_error), .dout(tb_dout), .busy(tb_busy), .data_strobe(tb_data_strobe));
 

	// Clock
	initial begin
		#100 // wait 100 ns before starting clock (after inputs have settled)
		tb_clk = 0;
		forever begin
			#5  tb_clk = ~tb_clk;
		end
	end	
	
	// Receive block
	always@(posedge tb_clk) begin
		if (tb_data_strobe) begin
			tb_received_data = tb_dout;
			tb_received_error = tb_error;
			tb_receive_time = $time;
			$display("  Data Strobe with data=%h and error=%h at time %0t ***", tb_dout, tb_error, $time);
		end
	end
	
	// Main test block
	initial begin

		errors = 0;
        //shall print %t with scaled in ns (-9), with 2 precision digits, and would print the " ns" string
		$timeformat(-9, 0, " ns", 20);
		$display("*** Start of Simulation ***");

		// - Wait a number of clocks before doing something. Make sure busy is low
		// - Send a byte of all zeros just to see if the first zero comes out. Exit simulation if no zero comes out (avoid time out)
		// - Create procedure for sending a byte and testing the result. Run a few bytes (with both odd and even parity)
		// - Try sending the "send" signal in the middle of the transmission: (see if it breaks it)
		// Transmit a known byte: check each bit and the timing (with some slack)

		tb_odd = 0;
		// Make sure it doesn't start receiving a character at the start
		tb_rx_in = 0;
		
		repeat (10) @ (negedge tb_clk); 		

		tb_rx_in = 1;

		repeat (10) @ (negedge tb_clk); 		
		
		// Test to see if the receiver is starting to receive with an initial 'rx_in = 0'
		if (tb_busy != 0) begin
			$display("*** ERROR: busy is '1' but should start out as a '0' at time %0t ***",$time);
			$display("            Initial rx_in=0 should be ignored");
			errors = errors + 1;
			// Wait until the rx is no longer busy before proceeding
			@(negedge tb_busy);
		end

		simulate_rx_transfer(8'hff,0,0);
		repeat (10000) @ (negedge tb_clk); 		
		simulate_rx_transfer(8'ha5,1,0);
		repeat (10000) @ (negedge tb_clk); 		
        simulate_rx_transfer(8'h5a,0,0);
		repeat (10000) @ (negedge tb_clk); 		
        simulate_rx_transfer(8'h00,1,0);
        repeat (10000) @ (negedge tb_clk);         
		simulate_rx_transfer(8'hff,0,1);
		repeat (10000) @ (negedge tb_clk); 		
		simulate_rx_transfer(8'ha5,1,1);
		repeat (10000) @ (negedge tb_clk); 		
        simulate_rx_transfer(8'h5a,0,1);
		repeat (10000) @ (negedge tb_clk); 		
        simulate_rx_transfer(8'h00,1,1);
        repeat (10000) @ (negedge tb_clk);         

		repeat (10) @ (negedge tb_clk); 		
		$display("*** Simulation done with %0d errors at time %0t ***", errors, $time);
		$finish;

	end  // end initial begin

	
endmodule