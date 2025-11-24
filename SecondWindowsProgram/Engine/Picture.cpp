#include "Picture.h"
#include "SafeCleaning.h"

#undef FindResource

namespace
{
    enum Picture_STATE
    {
        NOTFOUND = -1
    };

    std::vector<Picture::Resource> resourceList;
    std::vector<Picture::Instance> instanceList;

    bool IsDuplicate(const std::string& _path, int* _index)
    {
        int i = 0;
        for (auto& picture : resourceList)
        {
            i++;
            if (picture.fileName == _path)
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

int Picture::Load(const std::string& _path)
{
    int index{ 0 };

    int resIndex = FindResource(_path);

    if (resIndex == NOTFOUND)
    {
        Resource pictureRes{};
        pictureRes.pSprite = new Sprite();
        pictureRes.pSprite->Load(_path);
        pictureRes.fileName = _path;
        resourceList.push_back(pictureRes);
        resIndex = static_cast<int>(resourceList.size() - 1);
    }

    Instance pictureIns
    {
        .resourceIndex{ resIndex },
        .transform    { nullptr },
        .rect         {},
        .alpha        { 1.0f }
    };

    instanceList.push_back(pictureIns);

    int instanceHandle = static_cast<int>(instanceList.size() - 1);
    ResetRect(instanceHandle);

    return instanceHandle;
}

void Picture::Draw(int _pictureHandle)
{
    auto& pictureIns = instanceList[_pictureHandle];
    auto& pictureRes = resourceList[pictureIns.resourceIndex];

    pictureRes.pSprite->Draw(*pictureIns.transform, pictureIns.rect, pictureIns.alpha);
}

void Picture::SetTransForm(int _PictureHandle, Transform* _transform)
{
    if (_transform->pParent != nullptr)
    {
        _transform->pParent->Calculation();
    }
    instanceList[_PictureHandle].transform = _transform;
}

void Picture::AllRelease()
{
    for (auto itr = resourceList.begin(); itr != resourceList.end();)
    {
        auto picture = (*itr);
        SafeCleaning::SafeDelete(picture.pSprite);
        itr = resourceList.erase(itr);
    }
    resourceList.clear();
    instanceList.clear();
}

void Picture::ResetRect(int _pictureHandle)
{
    if (_pictureHandle < 0 || _pictureHandle >= instanceList.size())
    {
        return;
    }

    XMINT2 picSize = resourceList[instanceList[_pictureHandle].resourceIndex].pSprite->GetSize();

    instanceList[_pictureHandle].rect.left = 0;
    instanceList[_pictureHandle].rect.top = 0;
    instanceList[_pictureHandle].rect.right = picSize.x;
    instanceList[_pictureHandle].rect.bottom = picSize.y;
    
}
