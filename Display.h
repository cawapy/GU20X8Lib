#pragma once

#include <inttypes.h>

namespace GU20x8
{
    class IConnection;

    class Display
    {
        IConnection& connection;

    public:

        Display(IConnection& connection);

        static const uint8_t ColumnCount = 20;
        void Begin();

        void DisableDisplay();
        void EnableDisplay();

        void ClearScreen();

        void WriteColumn(uint8_t column, uint8_t red, uint8_t blue);
        void WriteBlueColumn(uint8_t column, uint8_t value);
        void WriteRedColumn(uint8_t column, uint8_t value);

        // 0 = dimmest; 3 = brightest
        void SetBrightness(uint8_t brightness);

    private:

        static const uint8_t BluePageOffset = 0x00;
        static const uint8_t RedPageOffset = 0x20;
        static const uint8_t BrightnessSettingAddress = 0b00111111;

        void TransferValue(uint8_t address, uint8_t value, bool isDataTransfer);
    };
}
