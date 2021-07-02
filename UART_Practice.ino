
/*
 * 
 * UART Setup for Arduino Due
 * By Brendan Aguiar
 * Description: This program uses Due Register assignments for communication between the keyboard and serial monitor.
 * 
 * Version 0.1 Implemented UART
 * VErsion 0.2 Began Timer Counter Implement
 */
//UART Register Assignments
volatile unsigned long* UART_CR = (unsigned long*)0x400E0800; //Control Register (Write-Only)
volatile unsigned long* UART_MR = (unsigned long*)0x400E0804; //Mode Register (Read-Write)
volatile unsigned long* UART_SR = (unsigned long*)0x400E0814; //Status Register (Read-Only)
volatile unsigned long* UART_RHR = (unsigned long*)0x400E0818; //Receiver Holding Register (Read-Only)
volatile unsigned long* UART_THR = (unsigned long*)0x400E081C; //Transmitter Holding Register (Write-Only)
volatile unsigned long* UART_BRGR = (unsigned long*)0x400E0820; //Baud Rate Generator Register (Read-Write)
unsigned long MCK = 84000000; //Clock speed in Hz

//Timer counter Register Assignments
volatile unsigned long* TC_CCR0[3] = {(unsigned long*)0x40080000, (unsigned long*)0x40080040, (unsigned long*)0x40080080 };
volatile unsigned long* TC_CMR0[3] = {(unsigned long*)0x40080004, (unsigned long*)0x40080044, (unsigned long*)0x40080084 };
volatile unsigned long* TC_CV0[3] = {(unsigned long*)0x40080010, (unsigned long*)0x40080050, (unsigned long*)0x40080090 };

//Message Assignments
String msg1 = "Hello World";
  
void setup() {
  // put your setup code here, to run once:
  UART_init(9600); //initialize UART
  //timer_init();
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned char msg;
  //msg = read_char();
  print_message(msg1);
  print_char('\n');
  my_delay(1000);
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

void my_delay(unsigned int freq)
{
  double period = 1.0/double(freq); //calc period
  double half_period = period/ 2.0f; //50% duty cycle  
  double clk_period = 0.0000000625; //clock period def
  unsigned int ticks = half_period / clk_period; //calc ticks
  *TC_CCR0 &= 0x00000001; //Enable Timer Counter
  *TC_CMR &= 0x000004?0;
  *TC_CMR 
  /*
   *myTCCR1B &= 0xF8; //stop the timer
  *myTCNT1 = (unsigned int) (65536 - ticks); //set the counts
  * myTCCR1B |= 0b00000001; //start the timer
  while((*myTIFR1 & 0x01)==0); //wait for overflow
  *myTCCR1B &= 0xF8; //stop the timer
  *myTIFR1 |= 0x01; //reset TOV          */
}

void print_char(unsigned char data)
{
  while ((*UART_SR & 0x00000002) == 0); //wait for unprinted character to transmit
  *UART_THR = data; //write to Transmitter Holding Register
}

void print_message(String arr)
{
   for (int i = 0; i < arr.length(); i++)
  {
    msg = arr.charAt(i);
    print_char(msg);
  }
}
