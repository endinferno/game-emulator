#pragma once

#include <cstdint>

using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::uint8_t;

enum Chip8OpcodeType
{
    CHIP8_OPCODE_TYPE_0 = 0x0,
    CHIP8_OPCODE_TYPE_1 = 0x1,
    CHIP8_OPCODE_TYPE_2 = 0x2,
    CHIP8_OPCODE_TYPE_3 = 0x3,
    CHIP8_OPCODE_TYPE_4 = 0x4,
    CHIP8_OPCODE_TYPE_5 = 0x5,
    CHIP8_OPCODE_TYPE_6 = 0x6,
    CHIP8_OPCODE_TYPE_7 = 0x7,
    CHIP8_OPCODE_TYPE_8 = 0x8,
    CHIP8_OPCODE_TYPE_9 = 0x9,
    CHIP8_OPCODE_TYPE_A = 0xA,
    CHIP8_OPCODE_TYPE_B = 0xB,
    CHIP8_OPCODE_TYPE_C = 0xC,
    CHIP8_OPCODE_TYPE_D = 0xD,
    CHIP8_OPCODE_TYPE_E = 0xE,
    CHIP8_OPCODE_TYPE_F = 0xF,
};

enum Chip8Opcode8Type
{
    CHIP8_OPCODE_OPS_LDR = 0x0,
    CHIP8_OPCODE_OPS_ORR = 0x1,
    CHIP8_OPCODE_OPS_AND = 0x2,
    CHIP8_OPCODE_OPS_XOR = 0x3,
    CHIP8_OPCODE_OPS_ADD = 0x4,
    CHIP8_OPCODE_OPS_SUB = 0x5,
    CHIP8_OPCODE_OPS_SHR = 0x6,
    CHIP8_OPCODE_OPS_SBN = 0x7,
    CHIP8_OPCODE_OPS_SHL = 0xE,
};

enum Chip8OpcodeEType
{
    CHIP8_OPCODE_SK_KP = 0x9E,
    CHIP8_OPCODE_SK_KN = 0xA1,
};

enum Chip8OpcodeFType
{
    CHIP8_OPCODE_LD_DT = 0x7,
    CHIP8_OPCODE_LD_KP = 0xA,
    CHIP8_OPCODE_ST_DT = 0x15,
    CHIP8_OPCODE_ST_ST = 0x18,
    CHIP8_OPCODE_AD_IR = 0x1E,
    CHIP8_OPCODE_LD_DG = 0x29,
    CHIP8_OPCODE_ST_BR = 0x33,
    CHIP8_OPCODE_ST_RG = 0x55,
    CHIP8_OPCODE_FL_RG = 0x65,
};

struct chip8_opcode_t
{
    chip8_opcode_t(uint16_t code)
        : opcode(code)
    {}
    union
    {
        struct __attribute__((packed))
        {
            uint16_t addr : 12;
            uint8_t opcode_type : 4;
        } chip8_opcode_adr_t;
        struct __attribute__((packed))
        {
            uint8_t val : 8;
            uint8_t reg_x : 4;
            uint8_t opcode_type : 4;
        } chip8_opcode_srv_t;
        struct __attribute__((packed))
        {
            uint8_t ops_type : 4;
            uint8_t reg_y : 4;
            uint8_t reg_x : 4;
            uint8_t opcode_type : 4;
        } chip8_opcode_srr_t;
        uint16_t opcode;
    };
};

class Chip8Opcode
{
public:
    Chip8Opcode();
    Chip8Opcode(uint16_t op);
	[[nodiscard]] uint16_t GetAddr() const;
    [[nodiscard]] Chip8OpcodeType GetOpcodeType() const;
    [[nodiscard]] uint8_t GetKk() const;
    [[nodiscard]] uint8_t GetRegX() const;
    [[nodiscard]] uint8_t GetRegY() const;
    [[nodiscard]] uint8_t GetNibble() const;
    [[nodiscard]] Chip8Opcode8Type GetOpsType8() const;
    [[nodiscard]] Chip8OpcodeFType GetOpsTypeF() const;
    [[nodiscard]] Chip8OpcodeEType GetOpsTypeE() const;
    [[nodiscard]] bool IsClsOpcode() const;
    [[nodiscard]] bool IsRetOpcode() const;

private:
    chip8_opcode_t code_;
};
