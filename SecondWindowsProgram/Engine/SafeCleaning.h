#pragma once
#include <concepts>

namespace SafeCleaning
{
    template<typename TYPE>
    concept HasRelease = requires(TYPE _ptr)
    {
        _ptr.Release();
    };

    /// @brief new演算子で取得したポインタ変数の完全な削除を行う
    /// @tparam TYPE プリミティブ型、独自型なんでもござれ、ただしRelease関数を持つものには
    /// SafeReleaseを用いること。
    /// @param _ptr ポインタ変数
    template<typename TYPE>
    inline void SafeDelete(TYPE*& _ptr)
    {
        if (_ptr != nullptr)
        {
            delete _ptr;
            _ptr = nullptr;
        }
    }

    /// @brief new演算子で取得したポインタの配列の完全な削除を行う
    /// @tparam TYPE プリミティブ型、独自型、なんでもござれ。
    /// @param _ptr newで領域確保した配列
    template<typename TYPE>
    inline void SafeDeleteArray(TYPE*& _ptr)
    {
        if (_ptr != nullptr)
        {
            delete[] _ptr;
            _ptr = nullptr;
        }
    }

    /// @brief DirectXAPIやWin32APIのポインタ変数を削除する
    /// @tparam TYPE Release関数を持っている型
    /// @param _ptr ポインタ
    template<HasRelease TYPE>
    inline void SafeRelease(TYPE*& _ptr)
    {
        if (_ptr != nullptr)
        {
            _ptr->Release();
            _ptr = nullptr;
        }
    }
}