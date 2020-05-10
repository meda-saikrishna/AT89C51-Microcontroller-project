/*#include<reg52.h>
#include<string.h>
#include"serial.h"
#defiane lcd_data P0
sbit lcd_rs = P0^0;
sbit lcd_rw = P0^1;
sbit lcd_en = P0^2;
sbit relay  = P1^0;
sbit vib = P1^1;
sbit vehicle=P1^2;
void gsmmode();
void gpsmode();
unsigned char rcv,rcv1,rcv2,rcv3,rcv4,rcv5,msg1[15],msg2[15],msg11[15],msg12[15],msg3[8],r;
unsigned char rcg,pastnumber[11],i,count;
 void lcdcmd(unsigned char value)		   // LCD COMMAND
  {
   lcd_data=value&(0xf0); //send msb 4 bits
      lcd_rs=0;	  //select command register
      lcd_en=1;	  //enable the lcd to execute command
	  delay(3);
	  lcd_en=0;  
      lcd_data=((value<<4)&(0xf0));	 //send lsb 4 bits
      lcd_rs=0;	 //select command register
      lcd_en=1;	 //enable the lcd to execute command
	  delay(3);
	  lcd_en=0;  
        }

    void lcd_init(void)
{
 lcdcmd(0x02);	
 lcdcmd(0x02);
 lcdcmd(0x28);  
 lcdcmd(0x28);  
  lcdcmd(0x0e);	//cursor blinking
 lcdcmd(0x06);	//move the cursor to right side
 lcdcmd(0x01);	//clear the lcd
}
void lcddata(unsigned char value)
{
      lcd_data=value&(0xf0); //send msb 4 bits
      lcd_rs=1;	  //select data register
      lcd_en=1;	  //enable the lcd to execute data
	  delay(3);
	  lcd_en=0;  
      lcd_data=((value<<4)&(0xf0));	 //send lsb 4 bits
      lcd_rs=1;	  //select data register
      lcd_en=1;	  //enable the lcd to execute data
	  delay(3);
	  lcd_en=0;  
           delay(3); 
  }  
void msgdisplay(unsigned char b[]) // send string to lcd
  {
	unsigned char s,count=0;
	for(s=0;b[s]!='\0';s++)
  {
  count++;	 
  if(s==16)
   lcdcmd(0xc0);
   if(s==32)
   {
   lcdcmd(1);
   count=0;
   }
  lcddata(b[s]);
  }
  }
  void okcheck()
  {
  	do{
	
	rcv=receive();
	}	while(rcv!='K');
  
  }
  void sie()
 {
 ET0 = 0;
 ES   = 1;               
 EA   = 1;
 }
 void sid()
 {
 ET0 = 1;
 ES   = 0;                
 EA   = 0;
 }    
void main()
{
 vib=0;
 lcd_init();
 lcdcmd(0x01);
 lcdcmd(0x80);
 //msgdisplay("ACCIDNT IDNTFCTN USNG GSM & GPS");
   msgdisplay("GSM INITIALIZING......");
 gsmmode(); 
 delay(100);
  serinit();
  sertxs("AT\r\n");
   sertxs("AT+CMGF=1\r\n");
   sertxs("AT+CNMI=1,2,0,0\r\n");
   lcdcmd(0x01);
   msgdisplay("SEND MESSAGE TO STR MOBL NUM");
  	  for(count=0;count<10;count++);
	      {
	   pastnumber[count]=receive();
		  }	  
 sertxs("AT+CMGS=\"");
		 sertxs(pastnumber);
		 sertxs("\"\r\n");
   		 sertxs("Mobile no. registered\r\n");
   		 sertx(0x1A);
		 lcdcmd(0x01);
		 msgdisplay("MOBILE NUMBER   IS REGISTERED");
		 delay(200);
		 lcdcmd(0x01);
		 sie();
		 delay(10);
		 msgdisplay("ACCIDNT IDNTFCTN USNG GSM & GPS");
while(1)
{
 vehicle=1;
 if(vib == 1)
  {
  vehicle=0;
  lcdcmd(0x01);
  msgdisplay("ACCIDENT OCCURED");
  sid();
   gpsmode();
		 if((rcv1 == 'G') && (rcv2 == 'P') && (rcv3 == 'R') && (rcv4 == 'M') && (rcv5 == 'C')) 
            {
			  for(r=0;r<14;r++)
			    {
				  rcv=receive();
				}
              for(r=0;r<11;r++)
			    {
				  msg1[r]=receive();
				}
				 rcv=receive();
			  for(r=0;r<12;r++)
			    {
				  msg2[r]=receive();
				}

				for(r=0;r<13;r++)
			    {
				  rcv=receive();
				}
				for(r=0;r<6;r++)
			    {
				  msg3[r]=receive(); // date
				}
				lcdcmd(0x01);
				msgdisplay("SMS SENDING.....");
			 delay(350);
			 gsmmode();
			 delay(150);
             	 sertxs("AT+CMGS=\"");
		 		sertxs(pastnumber);
		 		sertxs("\"\r\n");
   		 		sertxs("Accident occured @ -\r\n");
   		 
			 sertxs("Latitude:");
			 sertxs(msg1);
			 sertxs("\r\n");
			 sertxs("Longitude:");
			 sertxs(msg2);
			 sertxs("\r\n");
			 sertx(0x1A);
			 delay(150);
			 lcdcmd(0x01);
			 msgdisplay("SMS SEND COMPLTE");
			 delay(150);
			 lcdcmd(0x01);
			   lcdcmd(0x80);
				msgdisplay("LT:");
				lcdcmd(0xc0);
				msgdisplay("LG:");
				lcdcmd(0x83);
				msgdisplay(msg1);
					lcdcmd(0xc3);
				msgdisplay(msg2);
			 //lcdcmd(0x01);
			 while(1);
			}
		else
		    {
		  goto loop;
			}
			sie();
			delay(10);
			
  }
 else
  {
		;;;
  }
}
}

void gsmmode()
{
   relay=0;
   delay(10);
}
void gpsmode()
{
 relay=1;
  delay(10);
}
 void serial_int (void) interrupt 4
{
       if (RI == 1)        
        {               rcv = SBUF;      
               RI = 0;        
               	}
	if(rcv=='*')
		{
		lcdcmd(0x01);
		msgdisplay("REQUEST RECEIVED");
		lcdcmd(0xc0);
		msgdisplay("FOR THE LOCATION");
		sid();
		 gpsmode();
loop:	do{
    	 rcv=receive();
      }while(rcv != '$'); 
	     rcv1=receive();
		 rcv2=receive();
		 rcv3=receive();
		 rcv4=receive();
		 rcv5=receive(); 
		 if((rcv1 == 'G') && (rcv2 == 'P') && (rcv3 == 'R') && (rcv4 == 'M') && (rcv5 == 'C')) 
            {
			  for(r=0;r<14;r++)
			    {
				  rcv=receive();
				}
              for(r=0;r<11;r++)
			    {
				  msg1[r]=receive();
				}
				 rcv=receive();
			  for(r=0;r<12;r++)
			    {
				  msg2[r]=receive();
				}

				for(r=0;r<13;r++)
			    {
				  rcv=receive();
				}
				for(r=0;r<6;r++)
			    {
				  msg3[r]=receive(); // date
				}
		
			 delay(350);
			 gsmmode();
			 delay(150);
			 lcdcmd(0x01);
			 msgdisplay("SMS SENDING....");
             	 sertxs("AT+CMGS=\"");
		 		sertxs(pastnumber);
		 		sertxs("\"\r\n");     		 			 	             
                        sertxs("Vehicle location@ -\r\n");
	 		  sertxs("Latitude:");
			 sertxs(msg1);
			 sertxs("\r\n");
		            sertxs("Longitude:");
			 sertxs(msg2);
			 sertxs("\r\n");
			  sertx(0x1A);
			 delay(200);
			 lcdcmd(0x01);
			 msgdisplay("SMS SENT");
			 delay(200);
			 lcdcmd(0x01);
			//  lcdcmd(0x01);
			   lcdcmd(0x80);
				msgdisplay("LT:");
				lcdcmd(0xc0);
				msgdisplay("LG:");
				lcdcmd(0x83);
				msgdisplay(msg1);
					lcdcmd(0xc3);
				msgdisplay(msg2);
}
		else
		    {
			  goto loop;
			}
			sie();
			delay(10);
		}       
	  }

	   */
#include<reg52.h>
#include<stdlib.h>
#include<string.h>
#define lcd_data P0
sbit  lcd_rs   =  P0^0;  
sbit lcd_rw=P0^1;
sbit  lcd_en   =  P0^2;
sbit relay=P1^0;
sbit motor=P2^0;
sbit ir=P1^1;
  unsigned char str;
  unsigned char count;
  unsigned char rcv;
  unsigned char pastnumber[11],count=0;
unsigned char rcv,rcv1,rcv2,rcv3,rcv4,rcv5,msg1[15],msg2[15],msg11[15],msg12[15],msg3[8],r;
unsigned char rcg,pastnumber[11],i,count;
void delay(unsigned char time)
{
 unsigned int i,j;
 for(i=0;i<time;i++)
 for(j=0;j<200;j++); 
}
void sertxs(unsigned char *tx)
 {
 //   unsigned char v;
   for(;*tx != '\0';tx++)
     {
	   SBUF=*tx;
	  while(TI == 0);
	   TI=0;
//	   v= receive();
//	   delay(2);
	   	 }
 }
void sertx(unsigned char tx)
 {
  //	   unsigned char v;
	   SBUF=tx;
	  while(TI == 0);
	   TI=0;
	//     v= receive();
	//	 delay(2);
 }
void serinit()
  {
    TMOD=0x20;
    TH1=0xFD;	  //9600
    SCON=0x50;
    TR1=1;
  }
unsigned char receive()
 {
  unsigned char rx; 
   while(RI == 0);
    rx=SBUF;
	RI=0;
	return rx;
 }
 void lcdcmd(unsigned char value)		   // LCD COMMAND
  {
        	 lcd_data=value&(0xf0); //send msb 4 bits
      lcd_rs=0;	  //select command registerlcd_en=1;	  //enable the lcd to execute command
	  delay(3);
	  lcd_en=0;  
      lcd_data=((value<<4)&(0xf0));	 //send lsb 4 bits
      lcd_rs=0;	 //select command register
      lcd_en=1;	 //enable the lcd to execute command
	  delay(3);
	  lcd_en=0;     
  } 
   void lcd_init(void)
{
 lcdcmd(0x02);	
 lcdcmd(0x02);
 lcdcmd(0x28);  
 lcdcmd(0x28); 
 lcdcmd(0x0C);	//cursor blinking
 lcdcmd(0x06);	//move the cursor to right side
 lcdcmd(0x01);	//clear the lcd
}	
void lcddata(unsigned char value)
  {
      lcd_data=value&(0xf0); //send msb 4 bits
      lcd_rs=1;	  //select data register
      lcd_en=1;	  //enable the lcd to execute data
	  delay(3);
	  lcd_en=0;  
      lcd_data=((value<<4)&(0xf0));	 //send lsb 4 bits
      lcd_rs=1;	  //select data register
      lcd_en=1;	  //enable the lcd to execute data
	  delay(3);
	  lcd_en=0;  
          delay(3); 
  }  
void msgdisplay(unsigned char b[]) // send string to lcd
  {
unsigned char s,count1=0;
for(s=0;b[s]!='\0';s++)
 {
  count1++;	 
  if(s==16)
   lcdcmd(0xc0);
   if(s==32)
   {
   lcdcmd(1);
   count1=0;
   }
  lcddata(b[s]);
 }
}
void ex0(void) interrupt 0
{
motor=0;
lcdcmd(1);
msgdisplay("ACCIDENT DETECTD");
delay(500);
lcdcmd(1);
msgdisplay("SMS SENDING...");
 sertxs("AT+CMGS=");
   sertx('"');
   sertxs(pastnumber);
   sertx('"');
   sertxs("\r\n");
   do{
     rcv = receive();
    }while(rcv != '>');
   sertxs("accident detected at \r\n");
   sertxs("Latitude:");
			 sertxs(msg1);
			 sertxs("\r\n");
				
			 sertxs("Longitude:");
			 sertxs(msg2);
			 sertxs("\r\n");
             sertx(0x1A);
            	do{
     rcv = receive();
    }while(rcv != 'K');	 
	delay(500);
	lcdcmd(1);
	msgdisplay("SMS SENT");
	delay(1000);
    lcdcmd(0x01);
	 msgdisplay("WAITING FOR SMS");
}
void ex1(void) interrupt 2
{
lcdcmd(1);
msgdisplay("SOMEBODY TAYING TO ACC UR VEHIC");
delay(500);
lcdcmd(1);
msgdisplay("SENDING SMS..");
sertxs("AT+CMGS=");
   sertx('"');
   sertxs(pastnumber);
   sertx('"');
   sertxs("\r\n");
   do{
     rcv = receive();
    }while(rcv != '>');
   sertxs("SOMEBODY TRYING TO ACCESS YOUR VEHICLE\r\n");
   sertxs("Latitude:");
			 sertxs(msg1);
			 sertxs("\r\n");
				
			 sertxs("Longitude:");
			 sertxs(msg2);
			 sertxs("\r\n");
    sertx(0x1A);
 	do{
     rcv = receive();
    }while(rcv != 'K');	
	lcdcmd(1);
	msgdisplay("SMS 1 SENT...");
///////////////////////////////////////////////////////
sertxs("AT+CMGS=");
   sertx('"');
   sertxs("7036914700");
   sertx('"');
   sertxs("\r\n");
   do{
     rcv = receive();
    }while(rcv != '>');
   sertxs("SOMEBODY TRYING TO ACCESS YOUR VEHICLE\r\n");
   sertxs("Latitude:");
			 sertxs(msg1);
			 sertxs("\r\n");
			 sertxs("Longitude:");
			 sertxs(msg2);
			 sertxs("\r\n");
    sertx(0x1A);
 	do{
     rcv = receive();
    }while(rcv != 'K');	
	lcdcmd(1);
	msgdisplay("SMS 2 SENT...");
	//////////////////////////////////////////////
	sertxs("AT+CMGS=");
   sertx('"');
   sertxs("7337055894");
   sertx('"');
   sertxs("\r\n");
   do{
     rcv = receive();
    }while(rcv != '>');
   sertxs("SOMEBODY TRYING TO ACCESS YOUR VEHICLE\r\n");
   sertxs("Latitude:");
			 sertxs(msg1);
			 sertxs("\r\n");
			 sertxs("Longitude:");
			 sertxs(msg2);
			 sertxs("\r\n");
    sertx(0x1A);
 	do{
     rcv = receive();
    }while(rcv != 'K');	
	///////////////////////////////////////////////////////
	lcdcmd(1);
	msgdisplay("SMS 3 SENT...");
	delay(1000); 
	 lcdcmd(0x01);
	 msgdisplay("WAITING FOR SMS");
	} 
void gpsmode(void)
{
relay=1;
}
void gsmmode()
{
relay=0;
}
  void gpsdata(void)
  {
  loop:	do{
    	 rcv=receive();
      }while(rcv != '$'); 
	     rcv1=receive();
		 rcv2=receive();
		 rcv3=receive();
		 rcv4=receive();
		 rcv5=receive(); 
		 if((rcv1 == 'G') && (rcv2 == 'P') && (rcv3 == 'R') && (rcv4 == 'M') && (rcv5 == 'C')) 
            {
			  for(r=0;r<13;r++)
			    {
				  rcv=receive();
				}
              for(r=0;r<11;r++)
			    {
				  msg1[r]=receive();
				}
				 rcv=receive();
			  for(r=0;r<13;r++)
			    {
				  msg2[r]=receive();
				}

				for(r=0;r<13;r++)
			    {
				  rcv=receive();
				}
				 lcdcmd(0x01);
			   lcdcmd(0x80);
				msgdisplay("LT:");
				lcdcmd(0xc0);
				msgdisplay("LG:");
				lcdcmd(0x83);
				msgdisplay(msg1);
					lcdcmd(0xc3);
				msgdisplay(msg2);
				}  
				else
				{
				goto loop;
				}
				}

   void gps_data_display()
   {
    lcdcmd(0x01);
			   lcdcmd(0x80);
				msgdisplay("LT:");
				lcdcmd(0xc0);
				msgdisplay("LG:");
				lcdcmd(0x83);
				msgdisplay(msg1);
					lcdcmd(0xc3);
				msgdisplay(msg2);
				}

				void send_gps_data()
				{
				sertxs("AT+CMGS=\"");
		 		sertxs(pastnumber);
		 		sertxs("\"\r\n");
   		 		sertxs("Vehicle location@ -\r\n");
			 sertxs("Latitude:");
			 sertxs(msg1);
			 sertxs("\r\n");
			 sertxs("Longitude:");
			 sertxs(msg2);
			 sertxs("\r\n");
			  sertx(0x1A);
			   do{
     rcv = receive();
    }while(rcv != 'K');
	delay(1000);
			  }
void main()
{
lcd_rw=0;
ir=1;
gsmmode();
   lcd_init();
   serinit();
   lcdcmd(0x01);
   msgdisplay("GSM initilizing...");
   delay(100);
       sertxs("AT\r\n");
  do{
     rcv = receive();
    }while(rcv != 'K');
  sertxs("AT+CMGF=1\r\n");
  do{
     rcv = receive();
    }
while(rcv != 'K');
  sertxs("AT+CNMI=1,2,0,0\r\n");
  do{
     rcv = receive();
    }while(rcv != 'K');
	 lcdcmd(1);
  msgdisplay("Send a msg to store mobile no.");
   	do{
    	 rcv=receive();
       } while(rcv != '+'); 
	   for(count=0;count<9;count++)
	      {
		   rcv=receive();
		  }
  for(count=0;count<10;count++)
	      {
		   pastnumber[count]=receive();
		  }
       lcdcmd(0x01);
	msgdisplay("+91");
	  msgdisplay(pastnumber);
	   sertxs("AT+CMGS=");
   sertx('"');
   sertxs(pastnumber);
   sertx('"');
   sertxs("\r\n");
   do{
     rcv = receive();
    }while(rcv != '>');
   sertxs("YOUR MOBILE NUMBER IS REGISTERED\r\n");
    sertx(0x1A);
 	do{
     rcv = receive();
    }while(rcv != 'K');	 
	  lcdcmd(0x01);
	  msgdisplay("GSM INIT COMPLTE");
	  delay(200); 
	  lcdcmd(0x01);
 msgdisplay("ACCIDNT IDNTFCTN USNG GSM & GPS");
 gpsmode();
	 delay(10);
	 gpsdata();
	 delay(500);
	 	 lp:
	 lcdcmd(0x01);
	 msgdisplay("WAITING FOR SMS");
     gsmmode();
	 motor=1;
	 EA=1;
	 EX0=1;
	 EX1=1;
	while(1)
   {
  do{
                 rcv = receive();
               }while(rcv != '*');
			   rcv = receive();
			   lcdcmd(0x01);
			   msgdisplay("SMS RECEIVED");
			   delay(500);
		   if(rcv == '1')
		   {
		   gpsmode();
		   gpsdata();
		   gps_data_display();
		   delay(2000);
		   lcdcmd(0x01);
		   gsmmode();
		   msgdisplay("SENDING SMS");
  sertxs("AT+CMGS=");
   sertx('"');
   sertxs(pastnumber);
   sertx('"');
   sertxs("\r\n");
   do{
     rcv = receive();
    }while(rcv != '>');
 	sertxs("Vehicle location@ -\r\n");
		 sertxs("Latitude:");
			 sertxs(msg1);
			 sertxs("\r\n");
			 sertxs("Longitude:");
			 sertxs(msg2);
			 sertxs("\r\n");
    sertx(0x1A);
 	do{
     rcv = receive();
 }while(rcv != 'K');	 
		   lcdcmd(0x01);
		   msgdisplay("SMS sent...");
		   delay(1000);
		   goto lp;
		   }
		   else
		   {
		   ;;;
		   }
if(rcv == 'B')
		   {
		   motor=0;
		   gpsmode();
		   gpsdata();
		   gps_data_display();
		   delay(2000);
		   lcdcmd(0x01);
		   gsmmode();
		   msgdisplay("SENDING SMS");
				  sertxs("AT+CMGS=");
   sertx('"');
   sertxs(pastnumber);
   sertx('"');
   sertxs("\r\n");
   do{
     rcv = receive();
    }while(rcv != '>');
 	sertxs("Breaks applied vehicle location@\r\n");
	             	 sertxs("Latitude:");
			 sertxs(msg1);
			 sertxs("\r\n");
	   	            sertxs("Longitude:");
			 sertxs(msg2);
			 sertxs("\r\n");
    sertx(0x1A);
 	do{
     rcv = receive();
    }while(rcv != 'K');	 
lcdcmd(0x01);
		   msgdisplay("SMS sent...");
		   delay(1000);
		   goto lp;
  	}
	else
	{;;;}
	}
}
