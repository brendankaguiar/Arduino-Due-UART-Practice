
/*
 * 
 * UART Setup for Arduino Due
 * By Brendan Aguiar
 * Description: This program uses Due Register assignments for communication between the keyboard and serial monitor.
 */
//UART Register Assignments
volatile unsigned long* UART_CR = (unsigned long*)0x400E0800; //Control Register (Write-Only)
volatile unsigned long* UART_MR = (unsigned long*)0x400E0804; //Mode Register (Read-Write)
volatile unsigned long* UART_SR = (unsigned long*)0x400E0814; //Status Register (Read-Only)
volatile unsigned long* UART_RHR = (unsigned long*)0x400E0818; //Receiver Holding Register (Read-Only)
volatile unsigned long* UART_THR = (unsigned long*)0x400E081C; //Transmitter Holding Register (Write-Only)
volatile unsigned long* UART_BRGR = (unsigned long*)0x400E0820; //Baud Rate Generator Register (Read-Write)
unsigned long MCK = 84000000; //Hz
unsigned char hex_msb;
unsigned char hex_lsb;

void setup() {
  // put your setup code here, to run once:
  UART_init(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned char msg;
  while (UART_Available() == 0);
  msg = read_char();
  print_char(msg);
  //print_char('\n');
  
}

void UART_init(unsigned long CD)
{
  unsigned int baud_rate = MCK / (16 * CD);
  *UART_CR = 0x00000050; //Enable Receive and Transmit
  *UART_MR = 0x00000800; //No Parity, Normal Mode
  *UART_BRGR = baud_rate;
}

unsigned char UART_Available() // Read USART0 RDA status bit and return non-zero true if set
{
  return *UART_SR & 0x00000001;
}

unsigned char read_char()
{
  return *UART_RHR;
}

void print_char(unsigned char data)
{
  while ((*UART_SR & 0x00000002) == 0);
  *UART_THR = data;
}
