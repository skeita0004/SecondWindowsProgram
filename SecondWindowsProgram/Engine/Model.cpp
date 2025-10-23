#include "Model.h"
#include "SafeCleaning.h"

#undef FindResource

namespace
{
    enum MODEL_STATE
    {
        NOTFOUND = -1
    };

    std::vector<Model::Resource> resourseList;
    std::vector<Model::Instance> instanceList;

    bool IsDuplicate(const std::string& _path, int* _index)
    {
        int i = 0;
        for (auto& model : resourseList)
        {
            i++;
            if (model.fileName == _path)
            {
                *_index = i - 1;
                return true;
            }
        }
        return false;
    }

    int FindResource(const std::string& _path)
    {
        for (int i = 0; i < resourseList.size(); i++)
        {
            if (resourseList[i].fileName == _path)
            {
                return i;
            }
        }
        return NOTFOUND;
    }
}

int Model::Load(const std::string& _path)
{
    int index{0};

    int resIndex = FindResource(_path);

    if (resIndex == NOTFOUND)
    {
        Resource modelRes{};
        modelRes.pModel = new Fbx();
        modelRes.pModel->Load(_path);
        modelRes.fileName = _path;
        resourseList.push_back(modelRes);
        resIndex = static_cast<int>(resourseList.size() - 1);
    }

    Instance modelIns
    {
        .resourseIndex = resIndex,
        .transform     = nullptr
    };
    //modelIns.resourseIndex = resIndex;
    //modelIns.transform = {};
    instanceList.push_back(modelIns);

    return static_cast<int>(instanceList.size() - 1);
}

void Model::Draw(int _modelHandle)
{
    auto& modelIns = instanceList[_modelHandle];
    auto& modelRes = resourseList[modelIns.resourseIndex];

    modelRes.pModel->Draw(*modelIns.transform);
}

void Model::SetTransForm(int _modelHandle, Transform* _transform)
{
    if (_transform->pParent != nullptr)
    {
        _transform->pParent->Calculation();
    }
    instanceList[_modelHandle].transform = _transform;
}

void Model::AllRelease()
{
    for (auto itr = resourseList.begin(); itr != resourseList.end();)
    {
        auto model = (*itr);
        SafeCleaning::SafeRelease(model.pModel);
        itr = resourseList.erase(itr);
    }
    resourseList.clear();
    instanceList.clear();
}
