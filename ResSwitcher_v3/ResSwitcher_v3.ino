
// v3: Switch to series boards, inverted bit layout in byte, so that numbers matche board labeling

//#include "SPI.h"

int switch1, switch2 = 0;
int error = 0;
#define SPIlatch 3
#define SPIoe 4
//#define SPIdat 11
//#define SPIclk 13

#define SoftSPI1CLK 8
#define SoftSPI1DAT 9
#define SoftSPI2CLK 10
#define SoftSPI2DAT 11


#define NROFBOARDS 13

void setup()
{
  pinMode(SPIlatch, OUTPUT);
  pinMode(SPIoe, OUTPUT);
  pinMode(SoftSPI1CLK, OUTPUT);
  pinMode(SoftSPI1DAT, OUTPUT);
  pinMode(SoftSPI2CLK, OUTPUT);
  pinMode(SoftSPI2DAT, OUTPUT);
//  pinMode(SPIdat, OUTPUT);
//  pinMode(SPIclk, OUTPUT);


  digitalWrite(SPIlatch, LOW);
  digitalWrite(SoftSPI1CLK, LOW);
  digitalWrite(SoftSPI2CLK, LOW);
  digitalWrite(SPIoe, HIGH); //disable output on Shift Registers (active low)
  
  Serial.begin(9600);
  Serial.println("*********************************************");
  Serial.println("Resistivity Switcher Ver. 3.0");
  Serial.println("*********************************************");
  Serial.println("Enter two relay numbers (starting from one),");
  Serial.println("either seperately or separated by ','");
  Serial.println("Example: 4,22 followed by <CR> OR <NL>, not both");
  Serial.println("Corresponding relays will be switched on,");
  Serial.println("all other relays will be switched off.");
  Serial.println("Counting starts at 1 for both Banana Probes");
  Serial.println("Maximum valid value is 100");
  Serial.println("*********************************************");
  Serial.println("Contact: andre.zibell@cern.ch");
  Serial.println("*********************************************");
  
  //SPI.begin();
  //SPI.setBitOrder(MSBFIRST);
  //SPI.setClockDivider(SPI_CLOCK_DIV128);

  for(int i = 0; i < NROFBOARDS; i++){
    //SPI.transfer(0x0);  //reset shift register
    shiftOut(SoftSPI1DAT, SoftSPI1CLK, LSBFIRST, 0);
    shiftOut(SoftSPI2DAT, SoftSPI2CLK, LSBFIRST, 0);

  }

  digitalWrite(SPIlatch, HIGH);  //latch the data.
  digitalWrite(SPIlatch, LOW);

}

void loop() 
{
  switch1 = getnumber();
  switch2 = getnumber();
/*
  Serial.println("");
  Serial.print("# Activate probe ");
  Serial.print(switch1);
  Serial.print(" on left banana plug and ");
  Serial.print(switch2);
  Serial.println(" on right banana plug");
*/
  if((switch1 > 100) || (switch2 > 100) || (switch1 < 1) || (switch2 < 1))
  {
    //Serial.println("# Illegal integer");
    error = 1;
  }

  switch1--; //account for counting from 1, but internally counting from 0
  switch2--;

  if (!error) 
  {

    digitalWrite(SPIoe, LOW); //enable output on Shift Registers (active low)
  
    byte outval1[NROFBOARDS];
    byte outval2[NROFBOARDS];
    
    //Serial.print("# Sending Bytes: ");
    
    for(int i = 0; i < NROFBOARDS; i++){
      outval1[i] = 0;
      outval2[i] = 0;
    
      if((switch1 >= (i*8))&&(switch1 < (i*8)+8)){
        outval1[i] |= (1 << switch1%8);
      }
    
      if((switch2 >= (i*8))&&(switch2 < (i*8)+8)){
        outval2[i] |= (1 << switch2%8);
      }
    
      //Serial.print(outval1[i]);  Serial.print(" "); Serial.print(outval2[i]); Serial.print(" - "); 
    
     // SPI.transfer(outval[i]);  //send bit pattern for first relay
    }
    
    //Serial.println("");
    
    for(int i = 0; i < NROFBOARDS; i++){
  //    SPI.transfer(outval[NROFBOARDS - (i+1)]);  //send bit pattern for relay
      //SPI.transfer(0x81);  //send bit pattern for relay
      shiftOut(SoftSPI1DAT, SoftSPI1CLK, LSBFIRST, outval1[NROFBOARDS - (i+1)]);
      shiftOut(SoftSPI2DAT, SoftSPI2CLK, LSBFIRST, outval2[NROFBOARDS - (i+1)]);
    }
    
    digitalWrite(SPIlatch, HIGH);  //latch the data. Done.
    digitalWrite(SPIlatch, LOW);
    
    //Serial.println("# Done!");
  }
  
  error = 0;
  
}
    
  
int getnumber()
{
  int number, index = 0;
  byte done = 0;
  char strValue[4];

  while(done == 0)
  {
    if(Serial.available())
    {
      char ch = Serial.read();
      if(index < 5 && isDigit(ch))
      {
        strValue[index++] = ch; // add the ASCII character to the string;
      }
      else
      {
        // here when buffer full or on the first non digit
        strValue[index] = 0;        // terminate the string with a 0
        number = atoi(strValue);  // use atoi to convert the string to an int
        index = 0;
        done = 1;
      }
    }
  }
  return number;
}
 
