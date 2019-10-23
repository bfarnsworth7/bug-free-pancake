`timescale 1ns / 100ps
//////////////////////////////////////////////////////////////////////////////////
//
//  Filename: tb_tx.v
//
//  Author: Mike Wirthlin
//  
//  Version 1.2
//
//////////////////////////////////////////////////////////////////////////////////

module tb_tx();

	reg tb_clk, tb_send, tb_odd;
    wire tb_busy, tb_tx_out;
	reg [7:0] tb_din;
	integer errors,b_errors, i;
    reg expected_parity;
    	
	localparam CLK_FREQUECY = 100000000;
	localparam BAUD_RATE = 19200;
	localparam CLOCKS_PER_BIT = CLK_FREQUECY / BAUD_RATE;

	task simulate_tx_transfer;
		input [7:0] data_to_transfer;
		input parity;
		begin

			b_errors = errors;

			// Wait for negative edge of clock and then start setting inputs
			@(negedge tb_clk);
			tb_din = data_to_transfer;
			tb_odd = parity;
			@(negedge tb_clk);
			tb_send = 1;
			$display("Sending byte 0x%h at time %0t", data_to_transfer, $time);

			@(negedge tb_clk);
			// Busy should be high now
			if (~tb_busy) begin
				$display("*** ERROR: busy did not go high at time %0t ***", $time);
				errors = errors + 1;
			end
			@(negedge tb_clk);
			// tx out should be low now (for stop bit)
			if (tb_tx_out) begin
				$display("*** ERROR: tx_out did not go low at time %0t ***", $time);
				errors = errors + 1;
			end
			// Wait for half of a bit period and check start bit
			repeat (CLOCKS_PER_BIT / 2) @(negedge tb_clk);
			if (tb_tx_out) begin
				$display("*** ERROR: tx_out is not low half way through start bit at time %0t ***", $time);
				errors = errors + 1;
			end

			tb_odd = 0;
			tb_send = 0;
			tb_din = 0;
			
			// check each bit
			for (i=0;i<8;i=i+1) begin
				// Wait for a bit period and check against expected data
				repeat (CLOCKS_PER_BIT) @(negedge tb_clk);
				if (tb_tx_out != data_to_transfer[i]) begin
					$display("*** ERROR: tx_out is %0d but expecting %0d for bit %0d at time %0t ***", tb_tx_out, data_to_transfer[i],i,$time);
					errors = errors + 1;
				end
			end

			// Check the parity bit
			repeat (CLOCKS_PER_BIT) @(negedge tb_clk);
            expected_parity = ^data_to_transfer ^ parity;
			if (tb_tx_out != expected_parity) begin
				$display("*** ERROR: tx_out is %0d but expecting %0d for parity bit (din=0x%h,odd=%h) at time %0t ***", 
					tb_tx_out, expected_parity, data_to_transfer, parity, $time);
				errors = errors + 1;
			end
			
			// Check the stop bit
			repeat (CLOCKS_PER_BIT) @(negedge tb_clk);
			if (tb_tx_out != 1) begin
				$display("*** ERROR: tx_out is %0d but expecting 1 for stop bit at time %0t ***", 
					tb_tx_out, $time);
				errors = errors + 1;
			end else if (b_errors == errors) begin
				$display("Successfully transmitted byte 0x%h at time %0t", 
					data_to_transfer, $time);
			end
			
		end
	endtask

	
	// Instance the DUT
	tx #(.CLK_FREQUENCY(CLK_FREQUECY), .BAUD_RATE(BAUD_RATE)) 
	   student_tx(.clk(tb_clk), .send(tb_send), .odd(tb_odd), .din(tb_din), .busy(tb_busy), .tx_out(tb_tx_out));
	//defparam student_tx.CLK_FREQUENCY = CLK_FREQUECY;
	//defparam student_tx.BAUD_RATE = BAUD_RATE;
 

	// Clock
	initial begin
		#100 // wait 100 ns before starting clock (after inputs have settled)
		tb_clk = 0;
		forever begin
			#5  tb_clk = ~tb_clk;
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
		
		tb_send = 0;
		tb_odd = 0;
		tb_din = 0;

		if (tb_tx_out != 1) begin
			$display("*** ERROR: tx_out is '0' but should start out as a '1' at time %0t ***",$time);
			errors = errors + 1;
		end

		repeat (1000) @ (negedge tb_clk); 		

        // test very basic transfers
		simulate_tx_transfer(8'hff,0);
		repeat (20000) @ (negedge tb_clk); 		
        simulate_tx_transfer(8'h00,1);
        repeat (20000) @ (negedge tb_clk);         
        // test forward and backwards swapping
		simulate_tx_transfer(8'h0f,0);
        repeat (20000) @ (negedge tb_clk);         
		simulate_tx_transfer(8'hf0,1);
        // test odd number of bits
        repeat (20000) @ (negedge tb_clk);         
        simulate_tx_transfer(8'h37,0);
        repeat (20000) @ (negedge tb_clk);         
        simulate_tx_transfer(8'h73,1);
        repeat (20000) @ (negedge tb_clk);         
		simulate_tx_transfer(8'haa,0);
		repeat (20000) @ (negedge tb_clk); 		
        simulate_tx_transfer(8'h55,1);
		repeat (20000) @ (negedge tb_clk); 		

		repeat (10) @ (negedge tb_clk); 		
		$display("*** Simulation done with %0d errors at time %0t ***", errors, $time);
		$finish;

	end  // end initial begin

	
endmodule