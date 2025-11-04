#pragma once
#include <concepts>

template<std::integral T, uint32_t N>
class Stack
{

    T& operator[](uint32_t index) { return m_Data[index]; }
    const T& operator[](uint32_t index) const { return m_Data[index]; }

    uint32_t GetPointer() const { return m_Pointer; }
private:
    T m_Data[N]{};
    uint32_t m_Pointer = 0;
};