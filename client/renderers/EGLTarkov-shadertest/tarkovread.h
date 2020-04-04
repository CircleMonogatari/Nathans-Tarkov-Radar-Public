#pragma once

#include <stdbool.h>

#if __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/* todo sound
* sudo ./looking-glass-client -s -k -g EGLTarkov
*/

typedef struct TarkovGame TarkovGame;

typedef struct
{
    char pName[100];
    float viewmatrix[16];
    float viewlocation[3];
    float targetlocation[3];
    
    float screenw, screenh;
} TarkovESPObject;

typedef struct
{
    TarkovESPObject *array;
    int used;
    int size;
} TarkovESPArray;

EXTERNC bool GetTarkovGame(TarkovGame **Tarkov, const char *ProcessName, const char *ModuleName);
EXTERNC void DestroyTarkovGame(TarkovGame *Tarkov);
EXTERNC bool TickTarkov(TarkovGame *Tarkov);

EXTERNC bool TarkovInGame(TarkovGame *Tarkov);

EXTERNC bool initArray(TarkovESPArray *a, int initialSize);
EXTERNC void insertArray(TarkovESPArray *a, TarkovESPObject element);
EXTERNC void clearArray(TarkovESPArray *a);
EXTERNC void freeArray(TarkovESPArray *a);

EXTERNC void GetTarkovPlayers(TarkovGame *Tarkov, TarkovESPArray *a, float width, float height);
EXTERNC void GetTarkovLoot(TarkovGame *Tarkov, TarkovESPArray *a, float width, float height);