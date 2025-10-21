#include "Model.h"
#include "SafeCleaning.h"

#undef FindResource

namespace
{
    enum MODEL_STATE
    {
        NOTFOUND = -1
    };

    //std::vector<Model::Model> modelList;

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

    // 重複しなければ、新規登録
    //if (not(IsDuplicate(_path, &index)))
    //{
    //    Model model
    //    {
    //        .pModel{nullptr},
    //        .fileName{_path}
    //    };

    //    Resource modelRes
    //    {
    //        .pModel{nullptr},
    //        .fileName{_path}
    //    };

    //    model.pModel = new Fbx();
    //    model.pModel->Load(model.fileName);

    //    modelList.push_back(model);
    //    index = { static_cast<int>(modelList.size() - 1) };
    //    return index;
    //}

    int resIndex = FindResource(_path);

    if (resIndex == -1)
    {
        Resource modelRes{};
        modelRes.pModel = new Fbx();
        modelRes.pModel->Load(_path);
        modelRes.fileName = _path;
        resourseList.push_back(modelRes);
        resIndex = static_cast<int>(resourseList.size() - 1);
    }

    Instance modelIns{};
    modelIns.resourseIndex = resIndex;
    modelIns.transform = {};
    instanceList.push_back(modelIns);

    return static_cast<int>(instanceList.size() - 1);
}

void Model::Draw(int _modelHandle)
{
    //for (auto& model : modelList)
    //{
    //    model.pModel->Draw(model.transform);
    //}

    auto& modelIns = instanceList[_modelHandle];
    auto& modelRes = resourseList[modelIns.resourseIndex];

    modelRes.pModel->Draw(modelIns.transform);
}

void Model::SetTransForm(int _modelHandle, const Transform& _transform)
{
    instanceList[_modelHandle].transform = _transform;
}

void Model::AllRelease()
{
    //for (auto itr = modelList.begin(); itr != modelList.end();)
    //{
    //    auto model = (*itr);
    //    SAFE_RELEASE(model.pModel);
    //    itr = modelList.erase(itr);
    //}

    for (auto itr = resourseList.begin(); itr != resourseList.end();)
    {
        auto model = (*itr);
        SafeCleaning::SafeRelease(model.pModel);
        itr = resourseList.erase(itr);
    }
    resourseList.clear();
    instanceList.clear();
}
