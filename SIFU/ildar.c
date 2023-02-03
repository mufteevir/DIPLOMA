//ICC-AVR application builder : 31.10.2010 12:51:20
// Target : M32
// Crystal: 8.0000Mhz

#include <iom32v.h>//Include files for Atmega 32 controller
#include <macros.h>

//------------converter---------------------
signed int alfa=0;//Declaring global variables
signed int alfa0=1015;//1 deg.=7
signed int alfamin=35;//1 deg.=7
signed int alfamax=1190;//1 deg.=7
signed int value;
signed int volt_os, tok_os;
unsigned char  k=0, s=1, o=1, p=1, r=1, l=1;
signed long oshibka_v=0, oshibka_i=0, oshibkaold_v=0, oshibkaold_i=0; 
signed long Uvh=0, Urn=0, Urt=0, Ui_p=0, Ui_i=0;
signed long Uout_v=0, Uout_i=0, Uv_i=0, Uv_p=0;
unsigned char FWD=1, REV=0;
unsigned char FWD_ref=1, REV_ref=0;
unsigned char pause=0,standby=1;
unsigned char count=0,count1=0;
unsigned char fault=1, global=0;
unsigned char soft_start=0;
unsigned int t=0;
//----------------indicator-----------------------
unsigned char sign_voltage=0,sign_current=0;
unsigned char cnt_i=0;//periods counter
unsigned int alfa_i=0, current_i[20], voltage_i[20];
signed int tok_i, volt_i;
//-----------------usart--------------------------
unsigned char flag_1=0,flag_2=0, post_1=0, post_2=0;
unsigned int temp=0;
unsigned char data=0x00;
unsigned char uart_data=0x00;
unsigned char flag1_1=0, flag1_2=0;
//---------------regulator-------------------
unsigned int kprop_v=0, tinteg_v=0, fkvant=0;
unsigned int kprop_i=0, tinteg_i=0, iupor=0;
unsigned int kos_v=0, kos_i=0;
//------------------------------------------
void init_devices(void)//the device initialization function
{
 CLI(); 
 port_init();
 adc_init();
 timer0_init();
 timer1_init();
 timer2_init();
 uart0_init();
 MCUCR = 0x05;//any logical change of INT0, INT1
 GICR  = 0xC0;// INT0, INT1 enabled
 TIMSK = 0x92;// T1Compare, T0Compare, T2Compare
 SEI(); 
}

void port_init(void)//Function to initialize ports
{
	// DDx | Px |  Mode  | resistor | Note
    // 0   | 0  |  Input | Off      | Pin disconnected from circuit
	// 0   | 1  |  Input | On       | Pin - current source
	// 1   | 0  |  Output| Off      | Output "0"
	// 1   | 1  |  Output| Off      | Output "1"
 PORTA = 0xF8;
 DDRA  = 0x00;
 PORTB = 0x3F;
 DDRB  = 0x00;
 PORTC = 0x00; 
 DDRC  = 0xFF;
 PORTD = 0x4C;//01001100=0x4C
 DDRD  = 0xB3; //10110011=0xB3
}

void adc_init(void)// ADC initialization function
{
 ADCSRA = 0x00; // Disable interrupt
 ADMUX = 0x40;// AVCC reference voltage, ADC0
 ADCSRA  = 0x8E;//ADEN, clk/64 prescaler (100us conversion time)// Single conversion, Kdel=64 (100us - conversion time), // interrupt enable.
}

void timer0_init(void)// Timer T0 initialization function
{
 TCCR0 = 0x00;
 TCNT0 = 0x00;
 OCR0  = 0x7D;//1 ms gain // Interrupt period - 1 ms
 TCCR0 = 0x8B;//clk/64 CTC mode // Kdel=64, reset on match
}

void timer1_init(void)
{
 TCCR1B = 0x00; 
 TCNT1H = 0x00; 
 TCNT1L = 0x00;
 OCR1AH = 0x00;
 OCR1AL = 0x00;
 OCR1BH = 0x00;
 OCR1BL = 0x00;
 TCCR1A = 0x00;
 TCCR1B = 0x00;// to start write 0x03 (clk/64)
}

void timer2_init(void)
{
 TCCR2 = 0x00; 
 TCNT2 = 0x00; 
 ASSR = 0x00;
 OCR2  = 0x9C;//5ms pause
 TCCR2 = 0x00;//to start write 0x06
}

void uart0_init(void)
{
 UCSRB = 0x00; 
 UCSRA = 0x00;//uni-processor mode, 1x speed
 UCSRC = (1<<URSEL)|(1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0); //8-bit frame
 UBRRL = 0x33;//0x33 - 9600 baud for 8MHz
 UBRRH = 0x00;//
 UCSRB = (1<<RXCIE)|(1<<RXEN)|(1<<TXEN);//RX enable, RX complete int,TX enable//Enable interrupt on completion of receive, //enable receive, enable transmit
}

#pragma interrupt_handler adc_isr:iv_ADC// ADC interrupt vector
void adc_isr(void)// Interrupt handler at the end of the ADC conversion
{
 CLI();
 value=ADCL;// Read the low byte of the conversion result,
 value|=ADCH<<8;//and then the high one and connect them.
 value=value-0x1FF;//2,5V zero point// 0 is shifted by 2.5V
 switch(ADMUX)
  {
  case 0x40://Uvh ADC0
  	   		  	 if(value>=-4&&value<=4) PORTC|=0x80;
                 else PORTC&=0x7F;//zero point indication  
  	   		  	  Uvh=value;
				  ADMUX=0x41;
				  ADCSRA|=0x40;//restart ADC
  break;
  case 0x41://Voltage ADC1
  	   			  if(value>=-4&&value<=4) PORTC|=0x40;
                  else PORTC&=0xBF;//zero point indication  
  	   		  	  voltage_i[count1]=value;
				  volt_os=value;
				  count1+=1;
				  if(count1==20)
				   {
				    volt_i=0;
				    for(count1=0;count1<20;count1++)
				    {
				     volt_i=volt_i+voltage_i[count1];
				    }
				   volt_i=volt_i/20;//average
				   count1=0;
				   if(volt_i>=0) 
					 {
					 sign_voltage=0;
					 }
					else 
					 {
					 sign_voltage=1;//sign of measured voltage
					 volt_i=-volt_i;//modul of measured voltage
					 }
				  }
				  ADMUX=0x42;
				  ADCSRA|=0x40;//restart ADC
  break;
  case 0x42://Current ADC2
  	   			 if(value>=-4&&value<=4) PORTC|=0x20;
                 else PORTC&=0xDF;//zero point indication
  	   			 current_i[count]=value;
				 tok_os=value;
				 count+=1;
				 if(count==20)
				   {
				   tok_i=0;
				   for(count=0;count<20;count++)
				    {
				    tok_i=tok_i+current_i[count];
				    }
				   tok_i=tok_i/20;//average
				   count=0;
				   if(tok_i>=0) 
					 {
					 sign_current=1;
					 }
					else 
					 {
					 sign_current=0;//sign of measured current
					 tok_i=-tok_i;//modul of measured current
					 }
				   if(tok_i>=511) //102-1A; 153-1,5A; 204-2A; 255-2,5A; 306-3A; 358-3,5A; 409-4A, 511 - 4,5A
				    {
					fault=1;
					PORTC&=0xE0;//switch all pulces off
					}
				   }
				 ADMUX=0x40;
  break;
  }
 SEI();
}

#pragma interrupt_handler int0_isr:iv_INT0// External interrupt vector INT0
void int0_isr(void)
{
CLI();
if(TCCR1B==0x00)
 {
 PORTC&=0xE0;//switch all pulces off
 alfa+=alfa0;
 if(alfa<=alfamin) alfa=alfamin;//lower limit of alfa
 if(alfa>=alfamax) alfa=alfamax;//upper limit of alfa
 //-----------------------------------------
 if((PIND&0x40)==0x00)//start/stop
    {
	soft_start=0;
	alfa=alfamax;
	}
	else
	 {
	 if(soft_start==0)
	  {
	  alfa=alfa0;
	  soft_start=1;
	  }
	 }
 //-----------------------------------------	
 OCR1A=alfa;
 TCCR1B=0x03;//start T1 for alfa counting
 }
cnt_i+=1;
if(cnt_i==100)//reboot time ~1 sec.
 {
 cnt_i=0;
 alfa_i=alfa/7;
 flag_2=1;
// flag_1=1;//flag for start transmition
 }
alfa=0;//reset alfa gain
SEI();
}

#pragma interrupt_handler int1_isr:iv_INT1
void int1_isr(void)
{
CLI();
if((PIND&0x08)==0x08)//zero-current
    {
	TCCR2=0x06;//start T2 for 5ms pause counting
	PORTD|=0x80;
	pause=1;
	}
else//current is not zero
	{
	TCCR2=0x00;//stop T2
	TCNT2=0x00;
	pause=0;
	}
SEI();
}

#pragma interrupt_handler timer0_comp_isr:iv_TIM0_COMP
void timer0_comp_isr(void)
{
 CLI();
 if(((PINB&0x40)==0x40)&&((PINB&0x80)==0x80))//mode of output regulator
 {
 Urn=Uvh<<1;
 if(Urn>=2047) {Urn=2047;}
 else if(Urn<-2047){ Urn=-2047;}
 Urt=Urn;
 }
 else  //mode of input regulator
 {
 switch(fkvant)
 {
 case 1: {OCR0=0x7D; TCCR0=0x8B;} break;//kdel=64, 1ms
 case 2: {OCR0=0x65; TCCR0=0x8B;} break;//kdel=64, 2ms
 case 3: {OCR0=0xA1; TCCR0=0x8C;} break;//kdel=256, 3ms
 case 4: {OCR0=0x82; TCCR0=0x8C;} break;//kdel=256, 4ms
 case 5: {OCR0=0x62; TCCR0=0x8C;} break;//kdel=256, 5ms
 case 6: {OCR0=0x43; TCCR0=0x8C;} break;//kdel=256, 6ms
 case 7: {OCR0=0x24; TCCR0=0x8C;} break;//kdel=256, 7ms
 case 8: {OCR0=0x05; TCCR0=0x8C;} break;//kdel=256, 8ms
 case 9: {OCR0=0xB8; TCCR0=0x8D;} break;//kdel=1024, 9ms
 case 10: {OCR0=0xB0; TCCR0=0x8D;} break;//kdel=1024, 10ms
 }
 //-------------------Speed regulator---------------
 if((PINB&0x10)==0x10)//Torque maintenance mode
 {
  Urn=Uvh;
 }
 else//speed maintenance mode
 {
 oshibka_v=Uvh;
 if(kos_v==0)
 {Urn=Uvh;}
 else { 
 if(kos_v==1) volt_os=volt_os;//Feedback amplification
 else if(kos_v==2) volt_os=volt_os>>1;
 else if(kos_v==4) volt_os=volt_os>>2;
 else if(kos_v==8) volt_os=volt_os>>3;
 else if(kos_v==16) volt_os=volt_os>>4;
 else if(kos_v==32) volt_os=volt_os>>5;
 else if(kos_v==64) volt_os=volt_os>>6;
 else if(kos_v==128) volt_os=volt_os>>7;
 oshibka_v-=volt_os;
 if(kprop_v==1) Uv_p=oshibka_v;//proportional P-channel
 else if(kprop_v==2) Uv_p=oshibka_v<<1;
 else if(kprop_v==4) Uv_p=oshibka_v<<2;
 else if(kprop_v==8) Uv_p=oshibka_v<<3;
 else if(kprop_v==16) Uv_p=oshibka_v<<4;
 else if(kprop_v==32) Uv_p=oshibka_v<<5;
 else if(kprop_v==64) Uv_p=oshibka_v<<6;
 else if(kprop_v==128) Uv_p=oshibka_v<<7;
 if((PIND&0x40)==0x00)Uv_i=0;//integral I-channel
 else if(tinteg_v==0) Uv_i=0;
 else if(tinteg_v==1) Uv_i=oshibka_v;
 else if(tinteg_v==2) Uv_i=oshibka_v>>1;
 else if(tinteg_v==4) Uv_i=oshibka_v>>2;
 else if(tinteg_v==8) Uv_i=oshibka_v>>3;
 else if(tinteg_v==16) Uv_i=oshibka_v>>4;
 else if(tinteg_v==32) Uv_i=oshibka_v>>5;
 else if(tinteg_v==64) Uv_i=oshibka_v>>6;
 else if(tinteg_v==128) Uv_i=oshibka_v>>7;
 Urn=Uv_p+Uv_i+oshibkaold_v;
 Uout_v=Urn;
 oshibkaold_v=Uout_v-Uv_p; 
 switch(iupor)//Restriction block
         {
         case 5: { if(Urn>=50) Urn=50;
                   else if(Urn<-50) Urn=-50;}//0.5A
				   break;
		 case 10: { if(Urn>=100) Urn=100;
                   else if(Urn<-100) Urn=-100;}//1A
				   break;
		 case 15: { if(Urn>=150) Urn=150;
                   else if(Urn<-150) Urn=-150;}//1.5A
				   break;
		 case 20: { if(Urn>=200) Urn=200;
                   else if(Urn<-200) Urn=-200;}//2A
				   break;
		 }         
 }}
 //-------------------Current regulator---------------
  if(kos_i==1) tok_os=tok_os;//Feedback amplification
 else if(kos_i==2) tok_os=tok_os>>1;
 else if(kos_i==4) tok_os=tok_os>>2;
 else if(kos_i==8) tok_os=tok_os>>3;
 else if(kos_i==16) tok_os=tok_os>>4;
 else if(kos_i==32) tok_os=tok_os>>5;
 else if(kos_i==64) tok_os=tok_os>>6;
 else if(kos_i==128) tok_os=tok_os>>7;
 oshibka_i=Urn-tok_os;
 if(kprop_i==1) Ui_p=oshibka_i;//proportional P-channel
 else if(kprop_i==2) Ui_p=oshibka_i>>1;
 else if(kprop_i==4) Ui_p=oshibka_i>>2;
 else if(kprop_i==8) Ui_p=oshibka_i>>3;
 else if(kprop_i==16) Ui_p=oshibka_i>>4;
 else if(kprop_i==32) Ui_p=oshibka_i>>5;
 if((PIND&0x40)==0x00)Ui_i=0;
 else if(tinteg_i==0) Ui_i=0;
 else if(tinteg_i==1) Ui_i=oshibka_i;//integral I-channel
 else if(tinteg_i==2) Ui_i=oshibka_i>>1;
 else if(tinteg_i==4) Ui_i=oshibka_i>>2;
 else if(tinteg_i==8) Ui_i=oshibka_i>>3;
 else if(tinteg_i==16) Ui_i=oshibka_i>>4;
 else if(tinteg_i==32) Ui_i=oshibka_i>>5;
 else if(tinteg_i==64) Ui_i=oshibka_i>>6;
 else if(tinteg_v==128) Uv_i=oshibka_v>>7;
 Urt=Ui_p+Ui_i+oshibkaold_i;
 Uout_i=Urt;
 oshibkaold_i=Uout_i-Ui_p;
 Urt=Urt>>5;
 }
 //-----------------Characteristics switcher--------------
  if(FWD==1)
  {
  Urt=-Urt;//drive mode
  }
  alfa=Urt;
  //---------------Seed direction sensor-----------------
  if(Urn>=2)
  {
   FWD_ref=1;
   REV_ref=0;
  }
  else if(Urn<=-2)
    {
	FWD_ref=0;
	REV_ref=1;
	}
 //---------------------------------------------------------
 ADCSRA|=0x40;//restart ADC conversion
 SEI();
}

#pragma interrupt_handler timer1_compa_isr:iv_TIM1_COMPA
void timer1_compa_isr(void)
{
 CLI();
 TCCR1B=0x00;//stop T1
 TCNT1=0;//reset T1
 PORTC|=0x10;//control output
 if(pause==0)
  {
  if((standby==0)&&(fault==0))
   {
    switch(FWD)
     {
	  case 0://backward
  	   	 	if((PIND&0x04)==0x04)//positive sin
			 {
			 PORTC&=0xF0;//switch all pulces off
			 PORTC|=0x01;//VS1,VS2 "REV"
			 }
			 else//negative sin
			  {
			  PORTC&=0xF0;//switch all pulces off
			  PORTC|=0x02;//VS3,VS4 "REV"
			  }
      break;
      case 1://forward
  	   	 	if((PIND&0x04)==0x00)//positive sin
			 {
			 PORTC&=0xF0;//switch all pulces off
			 PORTC|=0x04;//VS1,VS2 "FWD"
			 }
			 else//negative sin
			  {
			  PORTC&=0xF0;//switch all pulces off
 			  PORTC|=0x08;//VS3,VS4 "FWD"
			  }
      break;
	 }
    }
  }
  else
   {
   PORTC&=0xF0;//switch all pulces off
   }
 SEI();
}

#pragma interrupt_handler timer2_comp_isr:iv_TIM2_COMP
void timer2_comp_isr(void)
{
CLI();
TCCR2=0x00;//stop T2
TCNT2=0x00;//reset T2
PORTD&=0x7F;
if((PIND&0x08)==0x08)//if there is no current, then change direction
 {
 FWD=FWD_ref;
 REV=REV_ref;
 }
pause=0;
SEI();
}

#pragma interrupt_handler uart0_rx_isr:iv_USART0_RXC
void uart0_rx_isr(void)
{
 CLI();
 uart_data=UDR;
 global=uart_data;
 global=global>>7;//high bit selection
 global&=0x01;
 if(global==1)
   {
   flag1_2=0;
   post_1=0;
 switch(post_2)
  {
  case 0:
  	   	 if(uart_data==0x99) 		{post_2=1;}//start talking
		 else if(uart_data==0xFF)	{post_2=0;} 
  break;
  case 1:
  	   	 if(uart_data==0x80) 		{post_2=2;}//alfa1
		 else if(uart_data==0xFF)	{post_2=1;} 
  break;
  case 2:
  	   	 if(uart_data==0x80) 		{post_2=3;}//alfa2
		else if(uart_data==0xFF)	{post_2=2;} 
  break;
  case 3:
  	   	 if(uart_data==0x80) 		{post_2=4;}//alfa3
		 else if(uart_data==0xFF)	{post_2=3;} 
  break;
  case 4:
  	   	 if(uart_data==0x80) 		{post_2=5;}//alfa4
		 else if(uart_data==0xFF)	{post_2=4;} 
  break;
  case 5:
  	   	 if(uart_data==0x90) 		{post_2=6;}//current1
		 else if(uart_data==0xFF)	{post_2=5;} 
  break;
  case 6:
  	   	 if(uart_data==0x90) 		{post_2=7;}//current2
		else if(uart_data==0xFF)	{post_2=6;} 
  break;
  case 7:
  	   	 if(uart_data==0x90) 		{post_2=8;}//current3
		else if(uart_data==0xFF)	{post_2=7;} 
  break;
  case 8:
  	   	 if(uart_data==0x90) 		{post_2=9;}//current4
		 else if(uart_data==0xFF)	{post_2=8;} 
  break;
  case 9:
  	   	 if(uart_data==0x90) 		{post_2=10;}//sign current
		 else if(uart_data==0xFF)	{post_2=9;} 
  break;
  case 10:
  	   	 if(uart_data==0xA0) 		{post_2=11;}//voltage1
		 else if(uart_data==0xFF)	{post_2=10;} 
  break;
  case 11:
  	   	if(uart_data==0xA0) 		{post_2=12;}//voltage2
		else if(uart_data==0xFF)	{post_2=11;}   
  break;
  case 12:
  	   	if(uart_data==0xA0) 		{post_2=13;}//voltage3
		else if(uart_data==0xFF)	{post_2=12;}   
  break;
  case 13:
  	   	if(uart_data==0xA0) 		{post_2=14;}//voltage4
		else if(uart_data==0xFF)	{post_2=13;}   
  break;
  case 14:
  	   	if(uart_data==0xA0) 		{post_2=15;}//sign voltage
		else if(uart_data==0xFF)	{post_2=14;}   
  break;
  case 15:
  	   if(uart_data==0xCC) 		{post_2=0;flag_2=0;}//stop talking
	   else if(uart_data==0xFF)	{post_2=15;} 
  break;
  }
  }
  else 
 {
 flag1_1=0;
 post_2=0;
 if(uart_data==0x77) 		{post_1=0;}//questioning of button
 else {
 switch(post_1)
  {
  case 0:{
  	   	 if(uart_data==0x44) 		{post_1=1; }//high byte
		 else if(uart_data==0x0F)	{post_1=0;} }
  break;
  case 1:{
  	   	 if(uart_data==0x55) 		{post_1=0; flag_1=0;}//low byte
		 else if(uart_data==0x0F)	{post_1=1; } }
  break;
 }
 }
 }
 SEI();
}
void Progr(void)//Selecting the "Right" parameter                                                                                                   
{
if((PINB&0x01)==0x00) s=1;
else { if(s==1)
     {
	 flag_1=1;//transmission permission
	 flag1_1=0;
	 post_1=4;
	 if(t==7) {t=0; s=0;}
	 else {t=t+1; s=0;}
	 }}
}
void Esc(void)//Selecting the "Left" parameter
{
if((PINB&0x02)==0x00) o=1;
else { if(o==1)
     {
	  flag_1=1;//transmission permission
	  flag1_1=0;
	  post_1=4;
	 if(t==0) {t=7; o=0;}
	 else {t=t-1; o=0;}
	 }}
}
void Vverh(void)//increase parameter value
{
if((PINB&0x04)==0x00) p=1;
else { if(p==1)
     {
	 k=1;
	 obrabotka_t();//call the subroutine for processing button clicks
	 }}
}
void Vniz(void)//decreasing the parameter value
{
if((PINB&0x08)==0x00) r=1;
else { if(r==1)
     {
	 k=2;
	 obrabotka_t();//call the subroutine for processing button clicks
	 }}
}
void obrabotka_t(void)//call the subroutine for processing button clicks
{
     flag_1=1;// enable transmission
     flag1_1=0;
	 post_1=4;
     switch(t)
	      {
		  case 0: { switch(k)
		              {
					  case 1: {
					           if(kprop_i==32) {p=0;}
							   else{kprop_i=kprop_i<<1;p=0;}
							   }
							   break;
					  case 2: {
					           if(kprop_i==1) {r=0;}
					           else {kprop_i=kprop_i>>1;r=0;}
							   }
							   break;
                      }}
					  break;
		  case 1: { switch(k)
		              {
					  case 1: {
					           if(tinteg_i==0) {p=0;}
							   else {tinteg_i=tinteg_i>>1; p=0;}
					           }
							   break;
					  case 2: {if(tinteg_i==0) {tinteg_i=tinteg_i+1; r=0;}
					           else if(tinteg_i==128) {r=0;}
					           else {tinteg_i=tinteg_i<<1;r=0;}
					           }
							   break;
					  }}
					  break;
		 case 2: { switch(k)
		              {
					  case 1: {
					           if (kprop_v==1) {p=0;}
							   else {kprop_v=kprop_v>>1; p=0;}
					          }
							  break;
					  case 2: {
					           if (kprop_v==128) {r=0;}
							   else {kprop_v=kprop_v<<1; r=0; } 
					           } 
							   break;
					  }}
					  break;
	   	  case 3: {switch(k)
		              {
					  case 1: {
					           if(tinteg_v==0) {p=0;}
					           else {tinteg_v=tinteg_v>>1; p=0;} 
					           }
							   break;
					  case 2:  {
					           if(tinteg_v==0) {tinteg_v=tinteg_v+1; r=0;}
							   else if(tinteg_v==128){r=0;}
							   else {tinteg_v=tinteg_v<<1; r=0;} 
					           }
							   break;
					  }}
					  break;
		   case 4: { switch(k)
		              {
					  case 1: {
					           if(kos_i==1) {p=0;}
					           else {kos_i=kos_i>>1; p=0;} 
					           }
							   break;
					  case 2: {
					           if(kos_i==128) {r=0;}
							   else {kos_i=kos_i<<1; r=0;}
					           } 
							   break;
					  }}
					  break;
			 case 5: { switch(k)
			          {
					  case 1: {
					           if(kos_v==0) {p=0;}
							   else {kos_v=kos_v>>1; p=0;} 
					           }
							   break;
					  case 2: {
					           if(kos_v==0) {kos_v=kos_v+1; r=0;}
							   else if(kos_v==128) {r=0;}
					           else {kos_v=kos_v<<1; r=0;} 
					           }
							   break;
					  }}
					  break;
			 case 6: { switch(k)
			          {
					  case 1: {
					           if(fkvant==1) {p=0;}
					           else {fkvant=fkvant-1; p=0;} 
					           }
							   break;
					  case 2: {
					           if(fkvant==10) {r=0;}
							   else {fkvant=fkvant+1; r=0;}
					           } 
							   break;
							   }}
					  break;
			 case 7: { switch(k)
			          {
					  case 1: {
					           if(iupor==5) {p=0;}
					           else {iupor=iupor-5; p=0;} 
					           }
							   break;
					  case 2: {
					           if(iupor==20) {r=0;}
					           else {iupor=iupor+5; r=0;} 
					           }
							   break;
					  }}
					  break;	
     }                     
}
void Vvod(void)// Subroutine for pressing the Enter button - saving data to EEPROM
{
if((PINB&0x20)==0x00) l=1;
else { if(l==1)
     {
	    while(EECR&0x02){;}
	    EEAR=0;
	    EEDR=kprop_i;
		EECR=0x04;
		EECR=0x02;
		while(EECR&0x02){;}
		EEAR=1;
	    EEDR=tinteg_i;
		EECR=0x04;
		EECR=0x02;
		while(EECR&0x02){;}
		EEAR=2;
	    EEDR=kprop_v;
		EECR=0x04;
		EECR=0x02;
		while(EECR&0x02){;}
		EEAR=3;
	    EEDR=tinteg_v;
		EECR=0x04;
		EECR=0x02;
		while(EECR&0x02){;}
		EEAR=4;
	    EEDR=kos_i;
		EECR=0x04;
		EECR=0x02;
		while(EECR&0x02){;}
		EEAR=5;
	    EEDR=kos_v;
		EECR=0x04;
		EECR=0x02;
		while(EECR&0x02){;}
		EEAR=6;
	    EEDR=fkvant;
		EECR=0x04;
		EECR=0x02;
		while(EECR&0x02){;}
		EEAR=7;
	    EEDR=iupor;
		EECR=0x04;
		EECR=0x02;
		while(EECR&0x02){;}
	 l=0;
	 }}
}
void main(void)
{
 init_devices();
 //-----------------------driver/load-----------------------
 if(((PINB&0x40)==0x00)&&((PINB&0x80)==0x80))//read controller parameters from ROM
   {
   kprop_i=8; tinteg_i=1; fkvant=1; iupor=20;
   kprop_v=4; tinteg_v=32; kos_i=1; kos_v=1;
   }
 else { if(((PINB&0x40)==0x00)&&((PINB&0x80)==0x00))// read controller parameters from EEPROM
   {
   while(EECR&0x01) {;}
   EEAR=0;
   EECR=0x01;
   kprop_i=EEDR;
   while(EECR&0x01) {;}
   EEAR=1;
   EECR=0x01;
   tinteg_i=EEDR;
   while(EECR&0x01) {;}
   EEAR=2;
   EECR=0x01;
   kprop_v=EEDR;
   while(EECR&0x01) {;}
   EEAR=3;
   EECR=0x01;
   tinteg_v=EEDR;
   while(EECR&0x01) {;}
   EEAR=4;
   EECR=0x01;
   kos_i=EEDR;
   while(EECR&0x01) {;}
   EEAR=5;
   EECR=0x01;
   kos_v=EEDR;
   while(EECR&0x01) {;}
   EEAR=6;
   EECR=0x01;
   fkvant=EEDR;
   while(EECR&0x01) {;}
   EEAR=7;
   EECR=0x01;
   iupor=EEDR;
   while(EECR&0x01) {;}
   }}
while(1)
 {
 //---------------------------------------------------
  if((PIND&0x08)==0x08)//zero-current
   {
    FWD=FWD_ref;
    REV=REV_ref;
   }
   if((PIND&0x40)==0x00)//start/stop
    {
	 Progr();//obrabotka klavish
     Esc();
	 Vverh();
	 Vniz();
	 Vvod();
	 standby=1;//waiting
	 PORTD=0x00;//all LEDs off
	}
	else
	 {
	 standby=0;
	 if(fault==0) {PORTD=0x10;}//normal indication
	 else         {PORTD=0x20;}//fault indication
	 }
  if((fault==1)&&(standby==1))
    {
	fault=0;//fault reset
	}
//--------------------------------------------------
 if(flag_1==1)
 {transmit_data_1();}//call transfer routine 1
 if(flag_2==1)
 {
 transmit_data_2();//call transfer routine 2
 }
//--------------------------------------------------	
  }
}
void transmit_data_1(void)
{
 if((UCSRA&0x20)==0x20)//data register empty
  {
 if((PIND&0x40)==0x00)
 {
  if(flag1_1==0)
 {
       flag1_1=1;
	   flag1_2=0;
       post_2=0;
       post1();//call auxiliary data transfer routine 1
 }
 }
 }
 }
 void transmit_data_2(void)
{
 if((UCSRA&0x20)==0x20)//data register empty
  {
 if((PIND&0x40)==0x40)
 {
 if(flag1_2==0)
 {     
       flag_1=1;
       flag1_2=1;
	   flag1_1=0;
	   post_1=0;
       post2();
 }  
} 
}
}
void post2(void)
{
switch(post_2)
	         {
	         case 0:
	              UDR=0x99;//start talking post_2
		          break;   
             case 1://alfa_4
   		          temp=alfa_i>>12;
				  temp|=0x80;
		          data=temp;
		          UDR=data;
                  break;
             case 2://alfa_3
   		          temp=alfa_i>>8;
				  temp&=0x000F;
				  temp|=0x80;
		          data=temp;
		          UDR=data;
                  break;
		     case 3://alfa_2
   		          temp=alfa_i>>4;
				  temp&=0x000F;
				  temp|=0x80;
		          data=temp;
		          UDR=data;
                  break;
		     case 4://alfa_1
   		          temp=alfa_i;
				  temp&=0x000F;
				  temp|=0x80;
		          data=temp;
		          UDR=data;
                  break;
            case 5://tok_i_4
   		          temp=tok_i>>12;
				  temp|=0x90;
		          data=temp;
		          UDR=data;
                  break;
             case 6://tok_i_3
   		          temp=tok_i>>8;
				  temp&=0x000F;
				  temp|=0x90;
		          data=temp;
		          UDR=data;
                  break;
		     case 7://tok_i_2
   		          temp=tok_i>>4;
				  temp&=0x000F;
				  temp|=0x90;
		          data=temp;
		          UDR=data;
                  break;
		     case 8://tok_i_1
   		          temp=tok_i;
				  temp&=0x000F;
				  temp|=0x90;
		          data=temp;
		          UDR=data;
                  break;
            case 9://sign_current
			      temp=sign_current;
				  temp|=0x90;
   		          UDR=temp;
                  break;
            case 10://volt_i_4
   		          temp=volt_i>>12;
				  temp|=0xA0;
		          data=temp;
		          UDR=data;
                  break;
             case 11://volt_i_3
   		          temp=volt_i>>8;
				  temp&=0x000F;
				  temp|=0xA0;
		          data=temp;
		          UDR=data;
                  break;
		     case 12://volt_i_2
   		          temp=volt_i>>4;
				  temp&=0x000F;
				  temp|=0xA0;
		          data=temp;
		          UDR=data;
                  break;
		     case 13://volt_i_1
   		          temp=volt_i;
				  temp&=0x000F;
				  temp|=0xA0;
		          data=temp;
		          UDR=data;
                  break;
             case 14://sign_voltage
			      temp=sign_voltage;
				  temp|=0xA0;
   		          UDR=temp;
                  break;
             case 15://stop talking
   		          UDR=0xCC;
                  break;
             }
}
void post1(void)
{
switch(post_1)
       { 
	   case 0:
          {
		  switch(t)
		       {
			   case 0:
		          data=kprop_i>>4;
		          data&=0x0F;
		          UDR=data;
			      break;
			   case 1:	
		          data=tinteg_i>>4;
		          data|=0x10;
		          UDR=data;
			      break;
			   case 2:	
		          data=kprop_v>>4;
		          data|=0x20;;
		          UDR=data;
			      break;
			   case 3:	
		          data=tinteg_v>>4;
		          data|=0x30;
		          UDR=data;
			      break;
		       case 4:	
		          data=kos_i>>4;
		          data|=0x40;
		          UDR=data;
			      break;
			   case 5:	
		          data=kos_v>>4;
		          data|=0x50;
		          UDR=data;
			      break;
			   case 6:	
		          //data=fkvant>>4;
		          data=0x60;
		          UDR=data;
			      break;
			  case 7:	  
		          data=iupor>>4;
		          data|=0x70;
		          UDR=data;
			      break;
				  
	            }
		  }
		break;
		case 1:
		   {
		  switch(t)
		        {
				case 0:
		           data=kprop_i&0x0F;
		           UDR=data;
			       break;
				case 1:
		           data=tinteg_i&0x0F;
		           data|=0x10;
		           UDR=data;
			       break;
				case 2:
		           data=kprop_v&0x0F;
		           data|=0x20;
		           UDR=data;
			       break;
				case 3:
		           data=tinteg_v&0x0F;
		           data|=0x30;
		           UDR=data;
			       break;
				case 4:
		           data=kos_i&0x0F;
		           data|=0x40;
		           UDR=data;
			       break;  
				case 5:
		           data=kos_v&0x0F;
		           data|=0x50;
		           UDR=data;
			       break;
			    case 6:
		           data=fkvant&0x0F;
		           data|=0x60;
		           UDR=data;
			       break;
			    case 7:
		           data=iupor&0x0F;
		           data|=0x70;
		           UDR=data;
			       break;
	             }
		   }
		break;
		case 4:
		   UDR=0x77;
		   break;
	}
}



