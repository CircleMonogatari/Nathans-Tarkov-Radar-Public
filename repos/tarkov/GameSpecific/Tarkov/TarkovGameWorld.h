#pragma once
#include "GameTypes/Unity/GameWorld.h"
#include "TarkovPlayer.h"
#include "TarkovLootItem.h"
#include "TarkovExfilPoint.h"

class TarkovGameWorld : public GameWorld
{
public:
    WinProcess *GameProcess;
    uint64_t Address;
    bool IsValid;

    TarkovGameWorld()
    {
        Address = 0x0;
        IsValid = false;
    }

    TarkovGameWorld(WinProcess *GameProc, GameObject Object)
    {
        GameProcess = GameProc;
        Address = ReadPtrChain(GameProc, Object.GetComponent(), {0x18, 0x28});
        IsValid = true;
    }

    int32_t GetPlayerCount()
    {
        uint64_t m_pPlayerList = GameProcess->Read<uint64_t>(Address + 0x70);

        return GameProcess->Read<int32_t>(m_pPlayerList + 0x18);
    }

    std::vector<TarkovPlayer> GetAllPlayers(bool SkipFirst = false)
    {
        std::vector<TarkovPlayer> PlayerList;
        PlayerList.reserve(32);

        uint64_t m_pPlayerList = GameProcess->Read<uint64_t>(Address + 0x70);

        int32_t m_pPlayerListSize = GameProcess->Read<int32_t>(m_pPlayerList + 0x18);
        uint64_t m_pPlayerListObject = GameProcess->Read<uint64_t>(m_pPlayerList + 0x10);

        for (int i = SkipFirst ? 1 : 0; i < m_pPlayerListSize; i++)
        {
            TarkovPlayer m_pPlayer = TarkovPlayer(GameProcess, GameProcess->Read<uint64_t>(m_pPlayerListObject + 0x20 + (i * 0x8)));
            PlayerList.emplace_back(m_pPlayer);
        }

        return PlayerList;
    }

    std::vector<TarkovLootItem> GetAllLoot()
    {
        std::vector<TarkovLootItem> LootList;
        LootList.reserve(512);

        uint64_t m_lLootList = GameProcess->Read<uint64_t>(Address + 0x50);

        int32_t m_lLootListSize = GameProcess->Read<int32_t>(m_lLootList + 0x18);
        uint64_t m_lLootListObject = GameProcess->Read<uint64_t>(m_lLootList + 0x10);

        for (int i = 0; i < m_lLootListSize; i++)
        {
            TarkovLootItem m_lItem = TarkovLootItem(GameProcess, GameProcess->Read<uint64_t>(m_lLootListObject + 0x20 + (i * 0x8)), i);
            LootList.emplace_back(m_lItem);
        }

        return LootList;
    }

    std::vector<TarkovExfilPoint> GetExfilArray()
    {
        std::vector<TarkovExfilPoint> ExfilList;

        uint64_t m_eExfilController = GameProcess->Read<uint64_t>(GameProcess->Read<uint64_t>(Address + 0x18) + 0x20);

        int64_t m_eExfilListSize = GameProcess->Read<int64_t>(m_eExfilController + 0x18);
        for (int i = 0; i < m_eExfilListSize; i++)
        {
            TarkovExfilPoint m_eExfilPoint = TarkovExfilPoint(GameProcess, GameProcess->Read<uint64_t>(m_eExfilController + 0x20 + (i * 0x8)));
            ExfilList.push_back(m_eExfilPoint);
        }

        return ExfilList;
    }
};