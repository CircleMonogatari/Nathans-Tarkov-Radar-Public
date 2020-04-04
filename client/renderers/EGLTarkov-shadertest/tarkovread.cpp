#include "tarkovread.h"

//#include "Includes.h"
#include "GameSpecific/Tarkov/Tarkov.h"

bool GetTarkovGame(TarkovGame **Tarkov, const char *ProcessName, const char *ModuleName)
{
    FILE *out = stdout;
    pid_t pid;

#if (LMODE() == MODE_EXTERNAL())
    FILE *pipe = popen("pidof qemu-system-x86_64", "r");
    fscanf(pipe, "%d", &pid);
    pclose(pipe);
#else
    out = fopen("/tmp/testr.txt", "w");
    pid = getpid();
#endif

    try
    {
        //WinContext ctx(pid);
        WinContext *ctx = new WinContext(pid); /* de-allocation handled in TarkovGame() */
        ctx->processList.Refresh();
        //ctx->processList.FindProc(ProcessName);

        fprintf(out, "\nInitializing Game Offsets\n");
        bool ProcessFound, ModuleFound = false;

        for (auto &i : ctx->processList)
        {
            if (!strcasecmp(ProcessName, i.proc.name))
            {
                fprintf(out, "Game:\tLooping process %lx:%s\n", i.proc.dirBase, i.proc.name);
                ProcessFound = true;

                //fprintf(out, "\tExports:\n");
                for (auto &o : i.modules)
                {
                    fprintf(out, "\t%.8lx\t%.8lx\t%lx\t%s\n", o.info.baseAddress, o.info.entryPoint, o.info.sizeOfModule, o.info.name);
                    if (!strcmp(ModuleName, o.info.name))
                    {
                        ModuleFound = true;
                        *Tarkov = new TarkovGame(ctx, &i, o.info.baseAddress);
                        return true;
                    }
                }
            }
        }

        if (ProcessFound == false)
            fprintf(out, "Error finding process: %s\n", ProcessName);

        if (ModuleFound == false)
            fprintf(out, "Error finding module: %s\n", ModuleName);
    }
    catch (VMException &e)
    {
        fprintf(out, "Initialization error: %d\n", e.value);
    }

    return false;
}

void DestroyTarkovGame(TarkovGame *TarkovGame)
{
    delete TarkovGame;
}

bool TickTarkov(TarkovGame *Tarkov)
{
    if (Tarkov == nullptr)
        return false;

    return Tarkov->Tick();
}

bool TarkovInGame(TarkovGame *Tarkov)
{
    return Tarkov->GetGameStatus().IsGamePlaying();
}

bool initArray(TarkovESPArray *a, int initialSize)
{
    a->array = (TarkovESPObject *)malloc(initialSize * sizeof(TarkovESPObject));
    if (!a->array)
        return false;

    memset(a->array, 0, initialSize * sizeof(TarkovESPObject));

    a->used = 0;
    a->size = initialSize;
    return true;
}

void insertArray(TarkovESPArray *a, TarkovESPObject element)
{
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = (TarkovESPObject *)realloc(a->array, a->size * sizeof(TarkovESPObject));
    }
    a->array[a->used++] = element;
}

void clearArray(TarkovESPArray *a)
{
    memset(a->array, 0, a->size * sizeof(TarkovESPObject));
    a->used = 0;
}

void freeArray(TarkovESPArray *a)
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

void GetTarkovPlayers(TarkovGame *Tarkov, TarkovESPArray *a, float width, float height)
{
    Matrix4f CameraMatrix = Tarkov->GetCameraMatrix();
    Vector3f CameraLocation = Tarkov->GetCameraLocation();

    std::list<TarkovPlayer> Players = Tarkov->GetPlayerList();

    clearArray(a);

    for (TarkovPlayer Player : Players)
    {
        Vector2f *ScreenPos = new Vector2f;

        TarkovESPObject Object;
        strcpy(Object.pName, Player.GetPlayerProfile().GetPlayerInfo().GetPlayerName().GetString().c_str());
        memcpy(Object.viewmatrix, CameraMatrix, sizeof(Object.viewmatrix));
        memcpy(Object.viewlocation, CameraLocation, sizeof(Object.viewlocation));
        memcpy(Object.targetlocation, Player.GetPlayerBody().GetSkeletonRoot().GetLocationMatrixTest(), sizeof(Object.targetlocation));
        
        Object.screenw = width;
        Object.screenh = height;

        insertArray(a, Object);

        delete ScreenPos;
    }
}

void GetTarkovLoot(TarkovGame *Tarkov, TarkovESPArray *a, float width, float height)
{
    Matrix4f CameraMatrix = Tarkov->GetCameraMatrix();
    Vector3f CameraLocation = Tarkov->GetCameraLocation();

    std::list<TarkovLootItem> Items = Tarkov->GetLootList();

    clearArray(a);

    for (TarkovLootItem Item : Items)
    {
        Vector2f *ScreenPos = new Vector2f;

        TarkovESPObject Object;
        strcpy(Object.pName, Item.GetLootName().c_str());
        memcpy(Object.viewmatrix, CameraMatrix, sizeof(Object.viewmatrix));
        memcpy(Object.viewlocation, CameraLocation, sizeof(Object.viewlocation));
        memcpy(Object.targetlocation, Item.GetLootLocation(), sizeof(Object.targetlocation));
        
        Object.screenw = width;
        Object.screenh = height;

        insertArray(a, Object);

        delete ScreenPos;
    }
}