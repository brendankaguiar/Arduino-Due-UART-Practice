
/*
 * 
 * UART Setup for Arduino Due
 * By Brendan Aguiar
 * Description: This program uses Arduino Due register assignments for communication between the keyboard and serial monitor.
 * Due Manual: tinyurl.com/55vn5pzb
 * Processor: ATSAM3X8E
 */
//UART Register Assignments
volatile unsigned long* UART_CR = (unsigned long*)0x400E0800; //Control Register (Write-Only)
volatile unsigned long* UART_MR = (unsigned long*)0x400E0804; //Mode Register (Read-Write)
volatile unsigned long* UART_SR = (unsigned long*)0x400E0814; //Status Register (Read-Only)
volatile unsigned long* UART_RHR = (unsigned long*)0x400E0818; //Receiver Holding Register (Read-Only)
volatile unsigned long* UART_THR = (unsigned long*)0x400E081C; //Transmitter Holding Register (Write-Only)
volatile unsigned long* UART_BRGR = (unsigned long*)0x400E0820; //Baud Rate Generator Register (Read-Write)
unsigned long MCK = 84000000; //Clock speed in Hz


void setup() 
{
  UART_init(9600); //initialize UART
}

void loop() 
{
  unsigned char msg;
  msg = read_char();
  print_char(msg);
  print_char('\n');
  
}

void UART_init(unsigned long CD)
{
  unsigned int baud_rate = MCK / (16 * CD); //( master clk / (16 * clk divisions));
  *UART_CR = 0x00000050; //enable receive and transmit
  *UART_MR = 0x00000800; //no parity, normal mode
  *UART_BRGR = baud_rate;//set baud rate
}

unsigned char read_char()
{
  while ((*UART_SR & 0x00000001) == 0); //wait for unread character to receive
  return *UART_RHR; //read from Receiver Holding Register
}

void print_char(unsigned char data)
{
  while ((*UART_SR & 0x00000002) == 0); //wait for unprinted character to transmit
  *UART_THR = data; // write to Transmitter Holding Register
}
