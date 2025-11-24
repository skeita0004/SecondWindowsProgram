#pragma once

#include <string>
#include <vector>
#include "Transform.h"
#include "Fbx.h"

namespace Model
{
    struct Resource
    {
        Fbx* pModel;
        std::string fileName;
    };

    struct Instance
    {
        int resourceIndex;
        Transform* transform;
    };

    int  Load(const std::string& _path);
    void Draw(int _modelHandle);
    void SetTransForm(int _modelHandle, Transform* _transform);
    void AllRelease();

    XMFLOAT3 GetBonePosition(int _modelHandle,
                             const std::string& _boneName);
}