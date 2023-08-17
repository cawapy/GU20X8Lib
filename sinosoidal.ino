// Example sketch controlling GU20x8 display directly connected to Arduino Nano
//
// Pin connections:
// GU20x8 Address pins  A0..A5          <--> Arduino A0..A5
// GU20x8 Data pins     D0, D1, D2..D7  <--> Arduino RX0, TX1, D2..D7
// GU20x8 control pin   c/D             <--> Arduino D8
// GU20x8 control pin   WR              <--> Arduino D9
// GU20x8 control pin   BUSY            <--> Arduino D10
// GU20x8 control pin   EN              <--> Arduino D11
//

#include "Display.h"
#include "IConnection.h"

// Arduino PORTC = A0..A7
#define AddressPort PORTC
#define AddressPortDirection DDRC
#define AddressMask 0b00111111

// Arduino PORTD = RX0+TX1+D2..D7
#define DataPort PORTD
#define DataPortDirection DDRD
#define DataMask 0b11111111

// Arduino PORTB = D8..D13
#define ControlPort PORTB
#define ControlPortDirection DDRB
#define ControlPort_cD PORTB0
#define ControlPort_WR PORTB1
#define ControlPort_BUSY PORTB2
#define ControlPort_ENABLE PORTB3

//Image Buffer
byte red_buffer[20], blue_buffer[20];

#define SetBits(reg,value,valueMask) { (reg) = ((reg) & ~(valueMask)) | ((value) & (valueMask));}
#define SetBitPos(reg,pos,val) { if (val) (reg) |= (1<<(pos)); else (reg) &= ~(1 << (pos));}

class Connection : public GU20x8::IConnection
{
    virtual void ConfigureBus()
    {
        AddressPortDirection |= AddressMask;
        DataPortDirection |= DataMask;
        ControlPortDirection |= (1 << ControlPort_cD) | (1 << ControlPort_WR) | (1 << ControlPort_ENABLE);
        ControlPortDirection &= ~(1 << ControlPort_BUSY);
    }

    virtual void SetAddressBus(uint8_t address)
    {
        SetBits(AddressPort, address, AddressMask);
    }
    virtual void SetDataBus(uint8_t data)
    {
        SetBits(DataPort, data, DataMask);
    }
    virtual void SetCDBit(bool value)
    {
        SetBitPos(ControlPort, ControlPort_cD, value);
    }
    virtual void SetENBit(bool value)
    {
        SetBitPos(ControlPort, ControlPort_ENABLE, value);
    }
    virtual void SetWRBit(bool value)
    {
        SetBitPos(ControlPort, ControlPort_WR, value);
    }
    virtual bool GetBusyBit()
    {
        return ControlPort & ControlPort_BUSY;
    }
};

Connection connection;
GU20x8::Display vfd(connection);

void clear_all(void)
{
  for(int temp=0; temp<20; temp++)
    {
      red_buffer[temp]=0;
      blue_buffer[temp]=0;
      vfd.WriteColumn( temp, red_buffer[temp], blue_buffer[temp]);
    }
}

void draw_pixel(byte x, byte y, byte c){
if(c&0x01)
  red_buffer[x]=red_buffer[x]|(0x01<<(7-y));
else
  red_buffer[x]=red_buffer[x]&(~(0x01<<(7-y)));  

if(c&0x02)
  blue_buffer[x]=blue_buffer[x]|(0x01<<(7-y));
else
  blue_buffer[x]=blue_buffer[x]&(~(0x01<<(7-y)));  

vfd.WriteColumn( x, red_buffer[x], blue_buffer[x]);
}

void setup()
{
    vfd.Begin();
    vfd.ClearScreen();
    vfd.SetBrightness(3);
    vfd.EnableDisplay();

    clear_all();

}

void loop()
{
  for(byte off=0;off<20;off++){
  for(byte temp=0;temp<20;temp++)
  {
    int y=(byte)((sin(((float)(temp+off)*2*PI)/20)*4)+3.5);
    draw_pixel(temp,y,3);
  }
  delay(30);
  for(byte temp=0;temp<20;temp++)
  {
    int y=(byte)((sin(((float)(temp+off)*2*PI)/20)*4)+3.5);
    draw_pixel(temp,y,0);
  }
  }
 
}
