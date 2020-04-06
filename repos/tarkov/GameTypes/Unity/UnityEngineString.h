#pragma once
#include "Includes.h"

class UnityEngineString
{
public:
    WinProcess *GameProcess;
    uint64_t Address;

    UnityEngineString(WinProcess *GameProc, uint64_t Addr)
    {
        GameProcess = GameProc;
        Address = Addr;
    }

    int32_t GetStringSize()
    {
        return GameProcess->Read<int32_t>(Address + 0x10);
    }

    std::string GetString()
    {
        std::string result;

        int32_t StringSize = GetStringSize();
        result.resize(StringSize);

        GameProcess->Read(Address + 0x14, result.data(), StringSize * 2);

        return result;
    }
};