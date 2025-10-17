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
        int resourseIndex;
        Transform transform;
    };

	//struct Model
	//{
	//	Fbx* pModel;
	//	std::string fileName;
	//	Transform transform;
	//};

	int  Load(const std::string& _path);
	void Draw(int _modelHandle);
	void SetTransForm(int _modelHandle, const Transform& _transform);
	void AllRelease();
}