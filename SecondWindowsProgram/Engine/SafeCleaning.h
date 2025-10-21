#pragma once
#include <concepts>

namespace SafeCleaning
{
    template<typename TYPE>
    concept HasRelease = requires(TYPE _ptr)
    {
        _ptr.Release();
    };

    template<typename TYPE>
    inline static void SafeDelete(TYPE*& _ptr)
    {
        if (_ptr != nullptr)
        {
            delete _ptr;
            _ptr = nullptr;
        }
    }

    template<typename TYPE>
    inline static void SafeDeleteArray(TYPE*& _ptr)
    {
        if (_ptr != nullptr)
        {
            delete[] _ptr;
            _ptr = nullptr;
        }
    }

    template<HasRelease TYPE>
    inline static void SafeRelease(TYPE*& _ptr)
    {
        if (_ptr != nullptr)
        {
            _ptr->Release();
            _ptr = nullptr;
        }
    }
}