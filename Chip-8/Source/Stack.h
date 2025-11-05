#pragma once
#include "Runtime/Iterator.h"
#include <concepts>


template<std::integral T, T N>
class Stack final : public Nova::Iterable<T>
{
public:
    using PointerType = T*;
    using ReferenceType = T&;

    T& operator[](T index) { return m_Data[index]; }
    const T& operator[](T index) const { return m_Data[index]; }

    Nova::Iterator<T> begin() override { return &m_Data[0]; }
    Nova::Iterator<T> end() override { return &m_Data[N]; }
    Nova::ConstIterator<T> begin() const override { return &m_Data[0]; }
    Nova::ConstIterator<T> end() const override { return &m_Data[N]; }
private:
    T m_Data[N]{};
};
