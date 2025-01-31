/** main3.v                 Archlab, Institute of Science Tokyo / Tokyo Tech **/
/******************************************************************************/
`default_nettype none

`define UART_CNT 100 // UART wait count, 100MHz / 100 = 1Mbaud

/******************************************************************************/
`ifndef SYNTHESIS
module top;

    reg r_clk = 0; always #5 r_clk <= !r_clk;

    reg  [15:0] r_cnt       = 0             ;
    reg   [2:0] r_bit       = 0             ;
    reg         r_en                        ;
    reg  [31:0] r_tx_data   = 32'h00208633  ; // add x12, x1, x2
    always @(posedge r_clk) begin
        r_cnt   <= (r_cnt>=`UART_CNT*10) ? 0 : r_cnt+1;
        if ((r_bit<4) && (r_cnt==0)) begin
            r_bit   <= r_bit+1  ;
            r_en    <= 1        ;
        end else begin
            r_en    <= 0        ;
        end
        if (r_en) r_tx_data <= {8'h0, r_tx_data[31:8]}  ;
    end

    wire w_txd;
    m_uart_tx m_uart_tx0 (r_clk, r_en, r_tx_data[7:0], w_txd);
    m_main    m          (r_clk, w_txd);

    reg [2:0] r_rx_bit = 0;
    always @(posedge r_clk) begin
        if (m.w_en) begin
            $write("%08x \n", {m.w_dout, m.r_data[31:8]});
            r_rx_bit <= r_rx_bit+1;
        end
        if (r_rx_bit==4) $finish(2);
    end

endmodule
`endif // !SYNTHESIS

/******************************************************************************/
module m_main (
    input  wire w_clk       , // 100 MHz clock signal
    input  wire w_rxd
);

    wire       w_en      ;
    wire [7:0] w_dout    ;
    m_uart_rx m_uart_rx0 (w_clk, w_rxd, w_dout, w_en);

    reg [31:0] r_data = 0;
    reg [4:0] r_cnt = 0;
    reg r_en = 0;
    always @(posedge w_clk) if (w_en) begin 
        r_data <= {w_dout, r_data[31:8]};
        r_cnt <= (r_cnt == 3 ? 0 : r_cnt+1);
        r_en <= w_en;
    end

    reg [31:0] r_sum = 0;
    // Complete here to calculate the checksum r_sum
    // Connect VIO to r_sum to verify the checksum value
    
    always @(posedge w_clk) 
        if (r_en && r_cnt == 0) r_sum <= r_sum + r_data;

    vio_0 vio_00 (w_clk, r_sum);

endmodule

/******************************************************************************/
module m_uart_tx (
    input  wire       w_clk    , // 100 MHz clock signal
    input  wire       w_we     , // write enable
    input  wire [7:0] w_din    , // data in
    output wire       w_txd      // UART tx, data line from FPGA to PC
);

    reg [8:0] r_buf = 9'b1_1111_1111;
    reg [7:0] r_cnt = 1;
    always @(posedge w_clk) begin
        r_cnt <= (w_we) ? 1 : (r_cnt>=`UART_CNT) ? 1 : r_cnt+1;
        r_buf <= (w_we) ? {w_din, 1'b0} : (r_cnt>=`UART_CNT) ? {1'b1, r_buf[8:1]} : r_buf;
    end
    assign w_txd = r_buf[0];

endmodule

/******************************************************************************/
module m_uart_rx (
    input  wire       w_clk     , // 100 MHz clock signal
    input  wire       w_rxd     , // UART rx, data line from PC to FPGA
    output wire [7:0] w_dout    , // 8-bit data received
    output reg        r_en = 0    // data enable
);

    reg [2:0] r_detect_cnt = 0; // to detect the start bit
    always @(posedge w_clk) r_detect_cnt <= (w_rxd) ? 0 : r_detect_cnt+1;
    wire w_detected = (r_detect_cnt>2);

    reg       r_busy = 0;
    reg [7:0] r_cnt  = 1;
    always @(posedge w_clk) r_cnt <= (r_busy==0) ? 1 : (r_cnt>=`UART_CNT) ? 1 : r_cnt+1;

    reg [3:0] r_bit  = 0;
    reg [8:0] r_data = 0;
    always @(posedge w_clk) begin
        if (r_busy==0) begin
            {r_data, r_bit, r_en} <= 0;
            if (w_detected) r_busy <= 1;
        end
        else if (r_cnt>=`UART_CNT) begin
            r_bit  <= r_bit+1;
            r_data <= {w_rxd, r_data[8:1]};
            if (r_bit==8) begin r_en <= 1; r_busy <= 0; end
        end
    end
    assign w_dout = r_data[7:0];

endmodule

