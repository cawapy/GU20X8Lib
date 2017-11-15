#include "Display.h"

#include "IConnection.h"

using namespace GU20x8;

Display::Display(IConnection& connection) :
    connection(connection)
{}

void Display::Begin()
{
    connection.ConfigureBus();

    DisableDisplay();
}

void Display::DisableDisplay()
{
    connection.SetENBit(false);
}

void Display::EnableDisplay()
{
    connection.SetENBit(true);
}

void Display::ClearScreen()
{
    for (uint8_t column = 0; column < ColumnCount; column++)
    {
        WriteColumn(column, 0, 0);
    }
}

void Display::WriteColumn(uint8_t column, uint8_t red, uint8_t blue)
{
    WriteBlueColumn(column, blue);
    WriteRedColumn(column, red);
}

void Display::WriteBlueColumn(uint8_t column, uint8_t value)
{
    if (column < ColumnCount) TransferValue(BluePageOffset + column, value, true);
}

void Display::WriteRedColumn(uint8_t column, uint8_t value)
{
    if (column < ColumnCount) TransferValue(RedPageOffset + column, value, true);
}

void Display::SetBrightness(uint8_t brightness)
{
    TransferValue(BrightnessSettingAddress, brightness, false);
}

void Display::TransferValue(uint8_t address, uint8_t value, bool isDataTransfer)
{
    while (connection.GetBusyBit());

    connection.SetCDBit(isDataTransfer);
    connection.SetDataBus(value);
    connection.SetAddressBus(address);

    connection.SetWRBit(true);
    connection.SetWRBit(false);
}

