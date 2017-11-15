#pragma once

#include <inttypes.h>

namespace GU20x8
{
    class IConnection
    {
    public:
        virtual void ConfigureBus() = 0;
        virtual void SetAddressBus(uint8_t address) = 0;
        virtual void SetDataBus(uint8_t data) = 0;
        virtual void SetCDBit(bool value) = 0;
        virtual void SetENBit(bool value) = 0;
        virtual void SetWRBit(bool value) = 0;
        virtual bool GetBusyBit() = 0;
    };
};

