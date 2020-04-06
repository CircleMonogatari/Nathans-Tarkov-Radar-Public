#pragma once
#include "GameTypes/Unity/GameObjectManager.h"
#include "TarkovGameWorld.h"
#include "TarkovFPSCamera.h"

const struct Offsets
{
    static const uint64_t GameObjectManager = 0x15181E8;
} Offsets;

struct GameStatus
{
private:
    bool GameActive;
    bool GamePlaying;

    int32_t PlayerCount;

public:
    inline void SetGameActive_Intern(bool InGameActive) { GameActive = InGameActive; };
    inline void SetGamePlaying_Intern(bool InGamePlaying) { GamePlaying = InGamePlaying; };

    inline bool IsGameActive() { return GameActive; };
    inline bool IsGamePlaying() { return GamePlaying; };
};

class TarkovGame
{
protected:
    GameObjectManager GOM{};
    TarkovGameWorld LGW{};
    TarkovFPSCamera FPCamera{};

private:
    WinContext *ProcessContext;
    WinProcess *GameProcess;
    uint64_t ModuleBase;

    bool GameActive = false;
    bool InGame = false;
    GameStatus Status;

    Matrix4f CameraMatrix;
    Vector3f CameraLocation;
    std::vector<uint64_t> PlayerList;
    std::vector<uint64_t> LootList;

public:
    inline GameStatus GetGameStatus() { return Status; };
    inline Matrix4f GetCameraMatrix() { return CameraMatrix; };
    inline Vector3f GetCameraLocation() { return CameraLocation; };
    inline std::vector<uint64_t> GetPlayerList() { return PlayerList; };
    inline std::vector<uint64_t> GetLootList() { return LootList; };
    inline WinProcess * GetWinProc() { return GameProcess; }; // remove

    TarkovGame(WinContext *ctx, WinProcess *InProcess, uint64_t InModuleBase)
    {
        ProcessContext = ctx;
        GameProcess = InProcess;
        ModuleBase = InModuleBase;

        GOM = GameObjectManager(GameProcess, GameProcess->Read<uint64_t>(ModuleBase + Offsets.GameObjectManager));
        LGW = GOM.FindGameObjectActive<TarkovGameWorld>("GameWorld");
        FPCamera = GOM.FindGameObjectTagged<TarkovFPSCamera>("FPS Camera");

        Status = GameStatus();

        printf("Game Object Manager: %lx\tLocal Game World: %lx\n", GOM.Address, LGW.Address);
    }

    ~TarkovGame()
    {
        delete ProcessContext; /* rest of deallocation is handle inside ctx */
    }

    bool Tick()
    {
        Status.SetGameActive_Intern(GOM.Address != 0x0);
        Status.SetGamePlaying_Intern(LGW.IsValid && FPCamera.IsValid && LGW.GetPlayerCount() > 0);

        if (!Status.IsGameActive())
            return false;

        if (!Status.IsGamePlaying())
        {
            GOM = GameObjectManager(GameProcess, GameProcess->Read<uint64_t>(ModuleBase + Offsets.GameObjectManager));
            if (!LGW.IsValid)
                LGW = GOM.FindGameObjectActive<TarkovGameWorld>("GameWorld");

            if (!FPCamera.IsValid)
                FPCamera = GOM.FindGameObjectTagged<TarkovFPSCamera>("FPS Camera");
        }

        if (!Status.IsGameActive() || !Status.IsGamePlaying())
            return true;

        CameraMatrix = FPCamera.GetCameraMatrix();
        CameraLocation = FPCamera.GetCameraCoordinates();

        PlayerList = LGW.GetAllPlayers(true);
        LootList = LGW.GetAllLoot();

        return true;
    }
};