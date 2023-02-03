#include <iom32v.h>
#include <macros.h>

unsigned char data=0x00,start_ts=0,gain=0;
unsigned char i,i1,i2,delay=0;
unsigned char symbol=0x00;
unsigned char stop_init=0;
unsigned char flag=0,flag1=0,flag2=0;
unsigned char cnt=0;
unsigned int cnt1=0,cnt2=0;
unsigned char d=0,dd=0,ddd=0,dddd=0;
unsigned int alfa_i=0,alfa_temp=0;
unsigned int tok_i=0, volt_i=0,tok_temp=0,volt_temp=0;
unsigned int kprop_i_temp=0, tinteg_i_temp=0, global=0;
unsigned int kprop_i=8, tinteg_i=1;
unsigned int kprop_v_temp=0, tinteg_v_temp=0;
unsigned int kprop_v=4, tinteg_v=32;
unsigned int kos_i_temp=0, kos_v_temp=0;
unsigned int kos_i=1, kos_v=1;
unsigned int fkvant_temp=0, iupor_temp=0;
unsigned int fkvant=1, iupor=20;
unsigned char sign_current, sign_voltage,sign_power;
unsigned long int power;
unsigned char tmp=0, temp=0, perem=0;
unsigned char  mass[60]={0x08, 0x04, 0x24, 0x2F, 0x24, 0x2F, 0x24, 0x2F, 
                         0x08, 0x09, 0x24, 0x28, 0x2A, 0x28, 0x2A, 0x28,  
						 0x09, 0x02, 0x22, 0x22, 0x2A, 0x29, 0x2A, 0x2B, 
						 0x25, 0x24, 0x24, 0x25, 0x25, 0x28, 0x22, 0x2D, 
						 0x2A, 0x28, 0x25, 0x20, 0x24, 0x2F, 0x2A, 0x2A, 
						 0x2A, 0x25, 0x22, 0x22, 0x0D, 0x08, 0x27, 0x26,
					     0x26, 0x25, 0x27, 0x22, 0x28, 0x20, 0x23, 0x21,
						 0x22, 0x2E, 0x23, 0x21};
unsigned char mass1[62]={0x25, 0x25, 0x2C, 0x27, 0x23, 0x2D, 0x08, 0x08,
		 	  			 0x24, 0x22, 0x0C, 0x00, 0x24, 0x29, 0x2C, 0x27, 
						 0x23, 0x2D, 0x0C, 0x09, 0x24, 0x21, 0x09, 0x00,   
						 0x26, 0x21, 0x26, 0x2C, 0x26, 0x26, 0x26, 0x21,   
						 0x23, 0x2D, 0x09, 0x09, 0x26, 0x24, 0x26, 0x25,  
						 0x26, 0x27, 0x27, 0x22, 0x26, 0x25, 0x26, 0x25,
						 0x0D, 0x00, 0x25, 0x20, 0x2C, 0x27, 0x23, 0x2D,  
						 0x0D, 0x09, 0x24, 0x22, 0x2B, 0x2F};
unsigned char mass2[34]={0x0C, 0x00, 0x25, 0x24, 0x26, 0x25, 0x2B, 0x2A,
                         0x27, 0x29, 0x2E, 0x26, 0x2B, 0x28, 0x2B, 0x29,
						 0x28, 0x20, 0x2B, 0x2E, 0x26, 0x21, 0x27, 0x20,
						 0x26, 0x21, 0x2B, 0x2C, 0x26, 0x25, 0x2B, 0x2F,
						 0x27, 0x20};
unsigned char mass3[14]={0x09, 0x04, 0x24, 0x2B, 0x27, 0x20, 0x2B, 0x2F,
                         0x23, 0x2D, 0x23, 0x21, 0x22, 0x2F};
unsigned char mass4[10]={0x09, 0x04, 0x25, 0x24, 0x27, 0x20, 0x2B, 0x2F,
                         0x23, 0x2D};
unsigned char mass5[10]={0x09, 0x04, 0x24, 0x2B, 0x27, 0x20, 0x2B, 0x2D, 
                         0x23, 0x2D};
unsigned char mass6[10]={0x09, 0x04, 0x25, 0x24, 0x27, 0x20, 0x2B, 0x2D,
                         0x23, 0x2D};
unsigned char mass7[10]={0x09, 0x04, 0x24, 0x2B, 0x2E, 0x23, 0x2B, 0x2F,
                         0x23, 0x2D};
unsigned char mass8[10]={0x09, 0x04, 0x24, 0x2B, 0x2E, 0x23, 0x2B, 0x2D,
                         0x23, 0x2D};
unsigned char mass9[16]={0x09, 0x05, 0x27, 0x24, 0x2B, 0x2A, 
                         0x23, 0x2D, 0x09, 0x0A, 0x2B, 0x2C, 0x26, 0x23};
unsigned char mass10[14]={0x09, 0x04, 0x24, 0x29, 0x26, 0x2D,
                          0x23, 0x2D, 0x09, 0x0A, 0x24, 0x21}; 					 						 						  
unsigned char uart_data=0x00, post_1=0, post_2=0, answer=0, f=0, y=0;						 

void port_init(void)
{
 PORTA = 0x00;
 DDRA  = 0xFF;
 PORTB = 0x00;
 DDRB  = 0xFF;
 PORTC = 0x00; 
 DDRC  = 0xFF;
 PORTD = 0x00;
 DDRD  = 0xFF;
}

void timer0_init(void)
{
 TCCR0 = 0x00; 
 TCNT0 = 0x00;
 OCR0  = 0x7D;  
 TCCR0 = 0x0B;//CTC mode, clk/64, 1 ms preset
}

void uart0_init(void)
{
 UCSRB = 0x00; 
 UCSRA = 0x00;//uni-processor mode, 1x speed
 UCSRC = (1<<URSEL)|(1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0); //8-bit frame
 UBRRL = 0x33;//0x33 - 9600 baud for 8MHz
 UBRRH = 0x00;//
 UCSRB = (1<<RXCIE)|(1<<RXEN)|(1<<TXEN);//RX enable, RX complete int,TX enable
}

#pragma interrupt_handler timer0_comp_isr:iv_TIM0_COMP
void timer0_comp_isr(void)
{
 CLI();
 switch(start_ts)
  {
  case 0: break;
  case 1:
            i1+=1;
  	   	 	if(i1>=i2)
			 {
			 delay=1;
			 i1=0;
			 }
  break;
  }
 if(flag1==1)//time delay 1 second
  {
   cnt1+=1;
   if(cnt1==1000)
   {
   flag=1;
   flag1=0;
   cnt1=0;
   }
  }
 if(flag2==1)
  {
   cnt2+=1;
   if(cnt2==1000)
   {
   if(y==1)
   {
   flag=1;
   y=0;
   }
   else
   {
   flag=3;
   }//go to "alfa"-->"current"-->"voltage"-->"power" indication
   flag2=0;
   cnt2=0;
   }
  }  
 SEI();
}

#pragma interrupt_handler uart0_rx_isr:iv_USART0_RXC
void uart0_rx_isr(void)
{
 CLI();
 uart_data=UDR;
 global=uart_data;
 global=global>>7;
 global&=0x01;
 if(global==1)
    {
	if(f==0)
	{
	f=1;
	y=1;
	}
	if(post_2==0)
		   {  
	          if(uart_data==0x99)
			          {post_2=1; post_1=0; answer=0x99;}
	          else    {post_1=0; post_2=0; answer=0xFF;}
            }
    else    {post2();   } 
	 }
 else
 {
 if(f==1)
{
f=0;
y=1;
}
else {y=1;}
    if(uart_data==0x77)     {post_1=0;answer=0x77;}
    else {
		post1();
		 }
   }
 SEI();
}
void post1(void)
{
switch(post_1)
    {
    case 0:
	    {
 	  	perem=uart_data;
		temp=uart_data&0xF0;
		       switch(temp)
			   {
			   case 0x00: 
						{perem&=0x0F; kprop_i_temp=perem<<4;
						 post_1=1;answer=0x44;}
                         break;
			   case 0x10:
			            {perem&=0x0F; tinteg_i_temp=perem<<4;
						 post_1=1;answer=0x44;}
                         break;
			   case 0x20:
			            {perem&=0x0F; kprop_v_temp=perem<<4;
						 post_1=1;answer=0x44;}
                         break;
			   case 0x30:
			            {perem&=0x0F; tinteg_v_temp=perem<<4;
						 post_1=1;answer=0x44;}
                         break;
			   case 0x40:
			            {perem&=0x0F; kos_i_temp=perem<<4;
						 post_1=1;answer=0x44;}
                         break;
			   case 0x50:
			            {perem&=0x0F; kos_v_temp=perem<<4;
						 post_1=1;answer=0x44;}
                         break;
			   case 0x60:
			            {perem&=0x0F; fkvant_temp=perem<<4;
						 post_1=1; answer=0x44;}
                         break;
			   case 0x70:
			            {perem&=0x0F; iupor_temp=perem<<4;
						 post_1=1;answer=0x44;}
                         break;
			   default: {post_1=0;answer=0x0F;}
			   }
		 }
		 perem=0;
		 break;
    case 1:
	    {
 	  	perem=uart_data;
		temp=uart_data&0xF0;
		switch(temp)
			   {
			   case 0x00: {perem&=0x0F; kprop_i_temp|=perem;
			               kprop_i=kprop_i_temp;
						   kprop_i_temp=0;
						   answer=0x55;post_1=0;}
                           break;
			   case 0x10: {perem&=0x0F; tinteg_i_temp|=perem;
			               tinteg_i=tinteg_i_temp;
						   tinteg_i_temp=0;
						   answer=0x55;post_1=0;}
                           break;
			   case 0x20: {perem&=0x0F; kprop_v_temp|=perem;
			               kprop_v=kprop_v_temp;
						   kprop_v_temp=0;
						   answer=0x55;post_1=0;}
                           break;
			   case 0x30: {perem&=0x0F; tinteg_v_temp|=perem;
			               tinteg_v=tinteg_v_temp;
						   tinteg_v_temp=0;
						   answer=0x55;post_1=0;}
                           break;
			   case 0x40: {perem&=0x0F; kos_i_temp|=perem;
			               kos_i=kos_i_temp;
						   kos_i_temp=0;
						   answer=0x55;post_1=0;}
                           break;
			   case 0x50: {perem&=0x0F; kos_v_temp|=perem;
			               kos_v=kos_v_temp;
						   kos_v_temp=0;
						   answer=0x55;post_1=0;}
                           break;
			   case 0x60: {perem&=0x0F; fkvant_temp|=perem;
			               fkvant=fkvant_temp;
						   fkvant_temp=0;
						   perem=0;
						   answer=0x55;post_1=0;}
                           break;
			   case 0x70: {perem&=0x0F; iupor_temp|=perem;
			               iupor=iupor_temp;
						   iupor_temp=0;
						   answer=0x55;post_1=0;}
                           break;
		       default: {post_1=1;answer=0x0F;}
			   }
		 }
		 break;
    }
}
void post2(void)
{
switch(post_2)
                                 {
                                 case 1:
								       perem=uart_data;
								       temp=uart_data&0xF0;
 	  	                               if(temp==0x80) 
									         {perem&=0x0F; alfa_temp=perem<<12;
										                  post_2=2;answer=0x80;}
		                               else			     {post_2=1;answer=0xFF;}
                                       break;
                                 case 2:
								       perem=uart_data&0x0F;
 	  	                               alfa_temp|=perem<<8;
		                               post_2=3;
		                               answer=0x80;
                                       break;
								 case 3:
								       perem=uart_data&0x0F;
 	  	                               alfa_temp|=perem<<4;
		                               post_2=4;
		                               answer=0x80;
                                       break;
                                 case 4:
								       perem=uart_data&0x0F;
 	  	                               alfa_temp|=perem;
		                               alfa_i=alfa_temp;
		                               alfa_temp=0;
		                               post_2=5;
		                               answer=0x80;
                                       break;
                                 case 5:
 	  	                               perem=uart_data;
								       temp=uart_data&0xF0;
 	  	                               if(temp==0x90) 
									         {perem&=0x0F; tok_temp=perem<<12;
										                  post_2=6;answer=0x90;}
		                               else			     {post_2=5;answer=0xFF;}
                                       break;
                                 case 6:
 	  	                               perem=uart_data&0x0F;
 	  	                               tok_temp|=perem<<8;
		                               post_2=7;
		                               answer=0x90;
                                       break;
                                 case 7:
 	  	                               perem=uart_data&0x0F;
 	  	                               tok_temp|=perem<<4;
		                               post_2=8;
		                               answer=0x90;
                                       break;
							     case 8:
								       perem=uart_data&0x0F;
 	  	                               tok_temp|=perem;
		                               tok_i=tok_temp;
		                               tok_temp=0;
		                               post_2=9;
		                               answer=0x90;
                                       break;
                                 case 9:
 	  	                               tmp=UDR;
									   tmp&=0x0F;
		                               if((tmp==0x00)||(tmp==0x01)) 
		                               {post_2=10;answer=0x90;sign_current=tmp;}
		                               else	 {post_2=9;answer=0xFF;}
                                       break;
                                 case 10:
 	  	                               perem=uart_data;
								       temp=uart_data&0xF0;
 	  	                               if(temp==0xA0) 
									         {perem&=0x0F; volt_temp=perem<<12;
										                 post_2=11;answer=0xA0;}
		                               else			    {post_2=10;answer=0xFF;}
                                       break;
                                 case 11:
 	  	                               perem=uart_data&0x0F;
 	  	                               volt_temp|=perem<<8;
		                               post_2=12;
		                               answer=0xA0;
                                       break;
                                 case 12:
 	                                   perem=uart_data&0x0F;
 	  	                               volt_temp|=perem<<4;
		                               post_2=13;
		                               answer=0xA0;
                                       break;
							     case 13:
								       perem=uart_data&0x0F;
 	  	                               volt_temp|=perem;
		                               volt_i=volt_temp;
		                               volt_temp=0;
		                               post_2=14;
		                               answer=0xA0;
                                       break;
                                 case 14:
 	  	                               tmp=UDR;
									   tmp&=0x0F;
		                               if((tmp==0x00)||(tmp==0x01)) 
		                                {post_2=15;answer=0xA0;sign_voltage=tmp;}
		                               else	 {post_2=14;answer=0xFF;}
                                       break;
                                 case 15:
 	  	                             if(uart_data==0xCC) {post_2=0;answer=0xCC;}
		                               else	 {post_2=15;answer=0xFF;} 
                                       break;
                                 }
}
void transmit_data(void)
{
 if((UCSRA&0x20)==0x20)//data register empty
  {
  UDR=answer;
  answer=0;
   
  } 
 }

void data_ts(void)
 {
 if(start_ts==0)
  {
  start_ts=1;
  switch(gain)
    {
    case 1:
		    i2=50;
 			i1=0;	
	break;
    case 2:
		    i2=5;
 			i1=0;
	break;
    case 3:
		   i2=1;
 		   i1=0;
	break;
    }
  }
 if(start_ts==1)
   {
   while(start_ts==1)
    {
   switch(delay)
     {
	case 0: ; break;
	case 1:
	         PORTC=data;
			 for(i=1;i<=100;i++) {;}
			 PORTC|=0x40;//set E
			 for(i=1;i<=100;i++) {;}
			 PORTC&=0xBF;//clear E
			 for(i=1;i<=100;i++) {;}
			 PORTC=0x00;
			 start_ts=0;
			 delay=0;
	break;
	 }
	}
   }	
 }

void digit(void)
  {
  switch(symbol)
		 {
		 case 0:
		            data=0x23;
					data_ts();
					data=0x20;
					data_ts();
		 break;
		 case 1:
		 	  		data=0x23;
					data_ts();
					data=0x21;
					data_ts();
		 break;
		 case 2:
		 	  		data=0x23;
					data_ts();
					data=0x22;
					data_ts();
		 break;
		 case 3:
		 	  		data=0x23;
					data_ts();
					data=0x23;
					data_ts();
		 break;
		 case 4:
		 	  		data=0x23;
					data_ts();
					data=0x24;
					data_ts();
		 break;
		 case 5:
		 	  		data=0x23;
					data_ts();
					data=0x25;
					data_ts();
		 break;
		 case 6:
		 	  		data=0x23;
					data_ts();
					data=0x26;
					data_ts();
		 break;
		 case 7:
		 	  		data=0x23;
					data_ts();
					data=0x27;
					data_ts();
		 break;
		 case 8:
		 	  		data=0x23;
					data_ts();
					data=0x28;
					data_ts();
		 break;
		 case 9:
		 	  		data=0x23;
					data_ts();
					data=0x29;
					data_ts();
		 break;
		 }
  } 
 
void init_devices(void)
{
 CLI(); 
 port_init();
 timer0_init();
 uart0_init();
 MCUCR = 0x00;
 GICR  = 0x00;//no external interrupts
 TIMSK = 0x02;//T0IC interrupt enable 
 SEI(); 
}

void main(void)
{
 init_devices();
 while(1)
  {
  //------------indicator----------------------------
  //------------initialisation------------------------
  if(stop_init==0)
   {
      data=0x02;
	  gain=1;//50ms
	  data_ts();
	  data=0x02;
	  gain=3;//1ms
	  data_ts();
	  data=0x08;
      data_ts();
	  data=0x00;
	  data_ts();
	  data=0x0C;
	  data_ts();
	  data=0x00;
	  gain=2;//5ms
	  data_ts();
	  data=0x06;
	  gain=3;//1ms
	  data_ts();
	  stop_init=1;
   }
  //-------data transfering------
  if(global==1)
  {
  flag_2();
  }
   else
   {
  flag_1();
   }
  if(answer!=0)
   {
   transmit_data();
   }
  }
}

void flag_2(void)// working condition!!!
{
switch(flag)
   {
   case 0://start window (1 second)
   		  for(cnt=0;cnt<60;cnt++)
	 	   {
	 	   data=mass[cnt];
	 	   data_ts();
	 	   }
		   flag1=1;
		   flag=10;
   break;
   case 1://clear screen
   		  gain=1;//delay 50ms
	  	  data=0x00;
	  	  data_ts();
	  	  data=0x01;
	  	  data_ts();
	  	  flag=2;
   break;
   case 2://switch display on, work window
   		  data=0x00;
		  data_ts();
		  data=0x0C;
		  data_ts();
		  gain=3;//delay 
	 	  for(cnt=0;cnt<62;cnt++)
	       {
	       data=mass1[cnt];
	       data_ts();
	       }
		   flag=3;
   break;
   case 3://alfa, deg
   		  ddd=alfa_i/100;
		  dd=(alfa_i/10)%10;
		  d=alfa_i%10;
		  data=0x09;
		  data_ts();
		  data=0x05;
		  data_ts();
		  symbol=ddd;
		  digit();
		  symbol=dd;
		  digit();
		  symbol=d;
		  digit();
		  flag=4;
		  
   break;
   case 4://armature current
   		  ddd=tok_i/100;
		  dd=(tok_i/10)%10;
		  d=tok_i%10;
		  data=0x0C;
		  data_ts();
		  data=0x03;
		  data_ts();
		  if(sign_current==1)
		   {
		   data=0x28;
		   data_ts();
		   data=0x20;
		   data_ts();
		   }
		  else
		   {
		   data=0x22;
		   data_ts();
		   data=0x2D;
		   data_ts();//-
		   }
		  symbol=ddd;
		  digit();
		  data=0x22;
		  data_ts();
		  data=0x2C;
		  data_ts();//,
		  symbol=dd;
		  digit();
		  symbol=d;
		  digit();
		  flag=5;
   break;
   case 5://armature voltage
   		  ddd=volt_i/100;
		  dd=(volt_i/10)%10;
		  d=volt_i%10;
		  data=0x08;
		  data_ts();
		  data=0x03;
		  data_ts();
		  if(sign_voltage==1)
		   {
		   data=0x28;
		   data_ts();
		   data=0x20;
		   data_ts();
		   }
		  else
		   {
		   data=0x22;
		   data_ts();
		   data=0x2D;
		   data_ts();//-
		   }
		  symbol=ddd;
		  digit();
		  symbol=dd;
		  digit();
		  symbol=d;
		  digit();
		  flag=6;
   break;
   case 6://output power
   		  power=volt_i*tok_i;
		  power=power/100;
		  if(sign_voltage==1)
		   {
		   if(sign_current==1) {sign_power=1;}
		   else {sign_power=0;}
		   }
		  else
		   {
		   if(sign_current==0) {sign_power=1;}
		   else {sign_power=0;}
		   }
		  dddd=power/1000;
		  ddd=(power/100)%10;
		  dd=(power/10)%10;
		  d=power%10;
		  data=0x0D;
		  data_ts();
		  data=0x03;
		  data_ts();
		  if(sign_power==1)
		   {
		   data=0x28;
		   data_ts();
		   data=0x20;
		   data_ts();
		   }
		  else
		   {
		   data=0x22;
		   data_ts();
		   data=0x2D;
		   data_ts();//-
		   }
		  symbol=dddd;
		  digit();  
		  symbol=ddd;
		  digit();
		  symbol=dd;
		  digit();
		  symbol=d;
		  digit();
		  flag=10;//wait mode
		  flag2=1;
		  cnt2=0;
		  break;
          case 10:
   		   ;
          break;
   }
   }
void flag_1(void)//program condition!!!
{
switch(flag)
   {
   case 0://start window (1 second)
   		  for(cnt=0;cnt<60;cnt++)
	 	   {
	 	   data=mass[cnt];
	 	   data_ts();
	 	   }
		   flag1=1;
		   flag=10;
   break;
   case 1://clear screen
   		  gain=1;//delay 50ms
	  	  data=0x00;
	  	  data_ts();
	  	  data=0x01;
	  	  data_ts();
	  	  flag=2;
   break;
   case 2: data=0x00;//switch display on, work window
		  data_ts();
		  data=0x0C;
		  data_ts();
		  gain=3;//delay 
	 	  for(cnt=0;cnt<34;cnt++)
	      {
	      data=mass2[cnt];
	      data_ts();
	      }
		  flag=3;
		  break;
	case 3: switch(temp)
			   {
			   case 0x00: 
			              for(cnt=0;cnt<14;cnt++)//kprop_i
	                      {
	                      data=mass3[cnt];
	                      data_ts();
	                      }
			              ddd=kprop_i/100;
			              dd=(kprop_i/10)%10;
						  d=kprop_i%10;
						  data=0x09;
		                  data_ts();
		                  data=0x0A;
		                  data_ts();
		                  //symbol=ddd;
		                  //digit();
		                  symbol=dd;
		                  digit();
		                  symbol=d;
		                  digit();
		                  flag=10;
						  flag2=1;
						  cnt2=0;
						  break;
				case 0x10: for(cnt=0;cnt<10;cnt++)//tinteg_i
	                      {
	                      data=mass4[cnt];
	                      data_ts();
	                      }
			              ddd=tinteg_i/100;
			              dd=(tinteg_i/10)%10;
						  d=tinteg_i%10;
						  data=0x09;
		                  data_ts();
		                  data=0x08;
		                  data_ts();
		                  symbol=ddd;
		                  digit();
		                  symbol=dd;
		                  digit();
		                  symbol=d;
		                  digit();
		                  flag=10;
						  flag2=1;
						  cnt2=0;
						  break;
				case 0x20: for(cnt=0;cnt<10;cnt++)//kprop_v
	                      {
	                      data=mass5[cnt];
	                      data_ts();
	                      }
			              ddd=kprop_v/100;
			              dd=(kprop_v/10)%10;
						  d=kprop_v%10;
						  data=0x09;
		                  data_ts();
		                  data=0x08;
		                  data_ts();
		                  symbol=ddd;
		                  digit();
		                  symbol=dd;
		                  digit();
		                  symbol=d;
		                  digit();
		                  flag=10;
						  flag2=1;
						  cnt2=0;
						  break;
				 case 0x30: for(cnt=0;cnt<10;cnt++)//tinteg_v
	                      {
	                      data=mass6[cnt];
	                      data_ts();
	                      }
			              ddd=tinteg_v/100;
			              dd=(tinteg_v/10)%10;
						  d=tinteg_v%10;
						  data=0x09;
		                  data_ts();
		                  data=0x08;
		                  data_ts();
		                  symbol=ddd;
		                  digit();
		                  symbol=dd;
		                  digit();
		                  symbol=d;
		                  digit();
		                  flag=10;
						  flag2=1;
						  cnt2=0;
						  break;
				  case 0x40: for(cnt=0;cnt<10;cnt++)//kos_i
	                      {
	                      data=mass7[cnt];
	                      data_ts();
	                      }
			              ddd=kos_i/100;
			              dd=(kos_i/10)%10;
						  d=kos_i%10;
						  data=0x09;
		                  data_ts();
		                  data=0x08;
		                  data_ts();
		                  symbol=ddd;
		                  digit();
		                  symbol=dd;
		                  digit();
		                  symbol=d;
		                  digit();
		                  flag=10;
						  flag2=1;
						  cnt2=0;
						  break;
				  case 0x50: for(cnt=0;cnt<10;cnt++)//kos_v
	                      {
	                      data=mass8[cnt];
	                      data_ts();
	                      }
			              ddd=kos_v/100;
			              dd=(kos_v/10)%10;
						  d=kos_v%10;
						  data=0x09;
		                  data_ts();
		                  data=0x08;
		                  data_ts();
		                  symbol=ddd;
		                  digit();
		                  symbol=dd;
		                  digit();
		                  symbol=d;
		                  digit();
		                  flag=10;
						  flag2=1;
						  cnt2=0;
						  break;
				  case 0x60: for(cnt=0;cnt<16;cnt++)//fkvant
	                      {
	                      data=mass9[cnt];
	                      data_ts();
	                      }
			              ddd=fkvant/100;
			              dd=(fkvant/10)%10;
						  d=fkvant%10;
						  data=0x09;
		                  data_ts();
		                  data=0x08;
		                  data_ts();
		                  //symbol=ddd;
		                  //digit();
		                  symbol=dd;
		                  digit();
		                  symbol=d;
		                  digit();
		                  flag=10;
						  flag2=1;
						  cnt2=0;
						  break;
					case 0x70: for(cnt=0;cnt<14;cnt++)//iupor
	                      {
	                      data=mass10[cnt];
	                      data_ts();
	                      }
			              ddd=iupor/100;
			              dd=(iupor/10)%10;
						  d=iupor%10;
						  data=0x09;
		                  data_ts();
		                  data=0x07;
		                  data_ts();
		                  //symbol=ddd;
		                  //digit();
						  symbol=dd;
		                  digit();
						  data=0x22;
		                  data_ts();
		                  data=0x2C;
		                  data_ts();//,
		                  symbol=d;
		                  digit();
		                  flag=10;
						  flag2=1;
						  cnt2=0;
						  break;  
						  }
	                      case 10: 
   		                  ;
                          break;
   }
   }




		  
		       
		          