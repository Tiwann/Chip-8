#pragma once
#include "Stack.h"
#include "Containers/StringView.h"
#include <cstdint>

#include "Runtime/Object.h"


namespace Nova::Rendering
{
    class CommandBuffer;
}

using Nova::Rendering::CommandBuffer;

enum Register
{
    V0, V1, V2, V3,
    V4, V5, V6, V7,
    V8, V9, VA, VB,
    VC, VD, VE, VF, REG_COUNT
};

class Emulator : public Nova::Object
{
public:
    static constexpr uint16_t MEMORY_SIZE = 0x1000;
    static constexpr uint16_t REGISTER_COUNT = REG_COUNT;
    static constexpr uint16_t STACK_SIZE = 0x10;
    static constexpr uint16_t PROGRAM_START = 0x200;
    static constexpr uint8_t FONTSET [] =
    {
        0xF0,0x90,0x90,0x90,0xF0, // 0
        0x20,0x60,0x20,0x20,0x70, // 1
        0xF0,0x10,0xF0,0x80,0xF0, // 2
        0xF0,0x10,0xF0,0x10,0xF0, // 3
        0x90,0x90,0xF0,0x10,0x10, // 4
        0xF0,0x80,0xF0,0x10,0xF0, // 5
        0xF0,0x80,0xF0,0x90,0xF0, // 6
        0xF0,0x10,0x20,0x40,0x40, // 7
        0xF0,0x90,0xF0,0x90,0xF0, // 8
        0xF0,0x90,0xF0,0x10,0xF0, // 9
        0xF0,0x90,0xF0,0x90,0x90, // A
        0xE0,0x90,0xE0,0x90,0xE0, // B
        0xF0,0x80,0x80,0x80,0xF0, // C
        0xE0,0x90,0x90,0x90,0xE0, // D
        0xF0,0x80,0xF0,0x80,0xF0, // E
        0xF0,0x80,0xF0,0x80,0x80  // F
    };

    Emulator();
    ~Emulator() override = default;
    
    bool Initialize();
    void Update(float deltaTime);
    void Render(CommandBuffer& cmdBuffer);
    void Destroy();

    bool LoadROM(Nova::StringView filepath);
protected:
    uint16_t Read(uint16_t address);
    void Write(uint16_t address, uint8_t data);
    void CpuCycle();
    void ExecuteOpcode(uint16_t opcode);


private:
    bool romLoaded = false;

    uint16_t m_Registers[REGISTER_COUNT]{};
    uint8_t m_Memory[MEMORY_SIZE]{};
    uint16_t m_ProgramCounter = 0;

    Stack<uint16_t, STACK_SIZE> m_Stack;
    uint16_t m_StackPointer = 0;
};