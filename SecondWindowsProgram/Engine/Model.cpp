#include "Model.h"
#include "SafeCleaning.h"

#undef FindResource

namespace
{
    enum MODEL_STATE
    {
        NOTFOUND = -1
    };

    std::vector<Model::Resource> resourceList;
    std::vector<Model::Instance> instanceList;

    bool IsDuplicate(const std::string& _path, int* _index)
    {
        int i = 0;
        for (auto& model : resourceList)
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
        for (int i = 0; i < resourceList.size(); i++)
        {
            if (resourceList[i].fileName == _path)
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
        resourceList.push_back(modelRes);
        resIndex = static_cast<int>(resourceList.size() - 1);
    }

    Instance modelIns
    {
        .resourceIndex = resIndex,
        .transform     = nullptr
    };

    instanceList.push_back(modelIns);

    return static_cast<int>(instanceList.size() - 1);
}

void Model::Draw(int _modelHandle)
{
    auto& modelIns = instanceList[_modelHandle];
    auto& modelRes = resourceList[modelIns.resourceIndex];

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
    for (auto itr = resourceList.begin(); itr != resourceList.end();)
    {
        auto model = (*itr);
        SafeCleaning::SafeRelease(model.pModel);
        itr = resourceList.erase(itr);
    }
    resourceList.clear();
    instanceList.clear();
}

XMFLOAT3 Model::GetBonePosition(int _modelHandle, const std::string& _boneName)
{
    XMFLOAT3 pos{};
    auto& instance = instanceList[_modelHandle];
    auto& resourse = resourceList[instance.resourceIndex];

    // 骨の座標を取得
    resourse.pModel->GetBonePosition(_boneName, &pos);

    // 骨の座標をモデルの平行移動成分で変形
    XMVECTOR vTransformed{XMVector3TransformCoord(XMLoadFloat3(&pos),
                                                  instance.transform->GetWorldMatrix())};

    XMStoreFloat3(&pos, vTransformed);
    return pos;
}
