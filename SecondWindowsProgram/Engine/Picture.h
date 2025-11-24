#pragma once

#include <string>
#include <vector>
#include "Transform.h"
#include "Sprite.h"

namespace Picture
{
    struct Resource
    {
        Sprite* pSprite;
        std::string fileName;
    };

    struct Instance
    {
        int resourceIndex;
        Transform* transform;
        RECT rect;
        float alpha;
    };

    int  Load(const std::string& _path);
    void Draw(int _pictureHandle);
    void SetTransForm(int _pictureHandle, Transform* _transform);
    void AllRelease();

    void ResetRect(int _pictureHandle);
}