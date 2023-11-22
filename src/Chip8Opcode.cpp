#include "Chip8Opcode.hpp"

Chip8Opcode::Chip8Opcode()
    : code(0)
{}

Chip8Opcode::Chip8Opcode(uint16_t op)
    : code(op)
{}

uint16_t Chip8Opcode::GetAddr() const
{
    return code.chip8_opcode_adr_t.addr;
}

Chip8OpcodeType Chip8Opcode::GetOpcodeType() const
{
    return Chip8OpcodeType(code.chip8_opcode_adr_t.opcode_type);
}

uint8_t Chip8Opcode::GetKk() const
{
    return code.chip8_opcode_srv_t.val;
}

uint8_t Chip8Opcode::GetRegX() const
{
    return code.chip8_opcode_srr_t.reg_x;
}

uint8_t Chip8Opcode::GetRegY() const
{
    return code.chip8_opcode_srr_t.reg_y;
}

uint8_t Chip8Opcode::GetNibble() const
{
    return code.chip8_opcode_srr_t.ops_type;
}

Chip8Opcode8Type Chip8Opcode::GetOpsType8() const
{
    return Chip8Opcode8Type(code.chip8_opcode_srr_t.ops_type);
}

Chip8OpcodeFType Chip8Opcode::GetOpsTypeF() const
{
    return Chip8OpcodeFType(code.chip8_opcode_srv_t.val);
}

Chip8OpcodeEType Chip8Opcode::GetOpsTypeE() const
{
    return Chip8OpcodeEType(code.chip8_opcode_srv_t.val);
}

bool Chip8Opcode::IsClsOpcode() const
{
    return code.opcode == 0x00E0;
}

bool Chip8Opcode::IsRetOpcode() const
{
    return code.opcode == 0x00EE;
}
