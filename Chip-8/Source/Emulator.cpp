#include "Emulator.h"
#include "Rendering/CommandBuffer.h"
#include "Runtime/Memory.h"
#include "Serialization/FileStream.h"

using namespace Nova;
using namespace Nova::Rendering;
using namespace Nova::Memory;


Emulator::Emulator() : Object("Chip-8 Emulator")
{
}

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
    const uint16_t opcode = Read(m_ProgramCounter);
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

uint16_t Emulator::Read(const uint16_t address)
{
    if (address > MEMORY_SIZE) return 0;
    const uint16_t* memory = (uint16_t*)&m_Memory[address];
    m_ProgramCounter += sizeof(uint16_t);
    return *memory;
}

void Emulator::Write(const uint16_t address, const uint8_t data)
{
    m_Memory[address] = data;
}

