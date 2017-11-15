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

void setup()
{
    vfd.Begin();
    vfd.ClearScreen();
    vfd.SetBrightness(3);
    vfd.EnableDisplay();

    //      patterns:    red         blue
    vfd.WriteColumn( 0, 0b00001100, 0b00000000);
    vfd.WriteColumn( 1, 0b00001110, 0b00000000);
    vfd.WriteColumn( 2, 0b11111111, 0b00000000);
    vfd.WriteColumn( 3, 0b11111111, 0b00000000);
    vfd.WriteColumn( 4, 0b00000000, 0b00000000);
    vfd.WriteColumn( 5, 0b00000000, 0b00000000);
    vfd.WriteColumn( 6, 0b00000000, 0b11000010);
    vfd.WriteColumn( 7, 0b00000000, 0b11100011);
    vfd.WriteColumn( 8, 0b00000000, 0b11110011);
    vfd.WriteColumn( 9, 0b00000000, 0b11011111);
    vfd.WriteColumn(10, 0b00000000, 0b11001110);
    vfd.WriteColumn(11, 0b00000000, 0b00000000);
    vfd.WriteColumn(12, 0b00000000, 0b00000000);
    vfd.WriteColumn(13, 0b01000010, 0b01000010);
    vfd.WriteColumn(14, 0b11001011, 0b11001011);
    vfd.WriteColumn(15, 0b11001011, 0b11001011);
    vfd.WriteColumn(16, 0b11111111, 0b11111111);
    vfd.WriteColumn(17, 0b01110110, 0b01110110);
    vfd.WriteColumn(18, 0b00000000, 0b00000000);
    vfd.WriteColumn(19, 0b00000000, 0b00000000);
}

void loop()
{
}