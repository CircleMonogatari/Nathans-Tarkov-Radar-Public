#pragma once
#include "hlapi/hlapi.h"
#include "vmath.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <codecvt>
#include <locale>
#include <random>
#include <chrono>
#include <list>
#include <map>
#include <math.h>

uint64_t ReadPtrChain(WinProcess *GameProc, uint64_t Base, std::vector<uint64_t> Chain)
{
    uint64_t result = GameProc->Read<uint64_t>(Base + Chain[0]);
    for (uint64_t i = 1; i < Chain.size(); i++)
    {
        result = GameProc->Read<uint64_t>(result + Chain[i]);
    }
    return result;
}

bool WorldToScreen(Matrix4f CameraMatrix, Vector3f WorldPosition, Vector2f &ScreenPosition, float width, float height)
{
    CameraMatrix = CameraMatrix.transpose();

    const Vector3f translation = {CameraMatrix.at(3, 0), CameraMatrix.at(3, 1), CameraMatrix.at(3, 2)};
    const Vector3f up = {CameraMatrix.at(1, 0), CameraMatrix.at(1, 1), CameraMatrix.at(1, 2)};
    const Vector3f right = {CameraMatrix.at(0, 0), CameraMatrix.at(0, 1), CameraMatrix.at(0, 2)};

    const auto w = translation.dotProduct(WorldPosition) + CameraMatrix.at(3,3);

    if (w < 0.1f)
        return false;

    const auto x = right.dotProduct(WorldPosition) + CameraMatrix.at(0, 3);
    const auto y = up.dotProduct(WorldPosition) + CameraMatrix.at(1, 3);

    ScreenPosition.x = (width) * (1.f + x / w) - (width);
    ScreenPosition.y = -((height) * (1.f - y / w) - (height));

    return true;
}