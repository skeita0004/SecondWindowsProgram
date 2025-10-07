#include "Model.h"

namespace
{
    std::vector<Model::Model> modelList;

    bool IsDuplicate(const std::string& _path, int* _index)
    {
        int i = 0;
        for (auto& model : modelList)
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
}

int Model::Load(const std::string& _path)
{
    int index{0};

    if (not(IsDuplicate(_path, &index)))
    {
        Model model
        {
            .pModel{nullptr},
            .fileName{_path}
        };

        model.pModel = new Fbx();
        model.pModel->Load(model.fileName);

        modelList.push_back(model);
        index = { static_cast<int>(modelList.size() - 1) };
        return index;
    }

    return index;
}

void Model::Draw(int _modelHandle)
{
    for (auto& model : modelList)
    {
        model.pModel->Draw(model.transform);
    }
}

void Model::SetTransForm(int _modelHandle, Transform _transform)
{
    modelList[_modelHandle].transform = _transform;
}

void Model::AllRelease()
{
    for (auto itr = modelList.begin(); itr != modelList.end();)
    {
        auto model = (*itr);
        SAFE_RELEASE(model.pModel);
        itr = modelList.erase(itr);
    }
}
