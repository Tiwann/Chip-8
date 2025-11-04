#include "Emulator.h"
#include "Rendering/CommandBuffer.h"
#include "Runtime/Memory.h"
#include "Serialization/FileStream.h"

using namespace Nova;
using namespace Nova::Rendering;
using namespace Nova::Memory;



bool Emulator::Initialize()
{
    Memcpy(&m_Memory[0], FONTSET, sizeof(FONTSET));
    m_ProgramCounter = PROGRAM_START;
    return true;
}

void Emulator::Update(const float deltaTime)
{
    if (!romLoaded) return;
    CpuCycle();
}

void Emulator::CpuCycle()
{
    const uint16_t opcode = (m_Memory[m_ProgramCounter] << 8) | m_Memory[m_ProgramCounter + 1];
    m_ProgramCounter += 2;
    ExecuteOpcode(opcode);
}

void Emulator::ExecuteOpcode(uint16_t opcode)
{
}

void Emulator::Render(CommandBuffer& cmdBuffer)
{

}

void Emulator::Destroy()
{

}

bool Emulator::LoadROM(StringView filepath)
{
    if (!filepath.EndsWith(".ch8")) return false;

    FileStream stream(filepath, OpenModeFlagBits::ReadBinary);
    if (!stream.IsOpened()) return false;
    const size_t fileSize = stream.GetSize();
    if (PROGRAM_START + fileSize > MEMORY_SIZE) return false;

    const size_t read = stream.Read(&m_Memory[PROGRAM_START], fileSize);
    if (read != fileSize) return false;

    stream.Close();
    romLoaded = true;
    return true;
}

