#include "Chip8Opcode.hpp"

chip8_opcode::chip8_opcode() : code(0) {}

chip8_opcode::chip8_opcode(uint16_t op) : code(op) {}

uint16_t chip8_opcode::get_addr() const { return code.chip8_opcode_adr_t.addr; }

chip8_opcode_type chip8_opcode::get_opcode_type() const
{
    return chip8_opcode_type(code.chip8_opcode_adr_t.opcode_type);
}

uint8_t chip8_opcode::get_kk() const { return code.chip8_opcode_srv_t.val; }

uint8_t chip8_opcode::get_reg_x() const
{
    return code.chip8_opcode_srr_t.reg_x;
}

uint8_t chip8_opcode::get_reg_y() const
{
    return code.chip8_opcode_srr_t.reg_y;
}

uint8_t chip8_opcode::get_nibble() const
{
    return code.chip8_opcode_srr_t.ops_type;
}

chip8_opcode_8_type chip8_opcode::get_ops_type_8() const
{
    return chip8_opcode_8_type(code.chip8_opcode_srr_t.ops_type);
}

chip8_opcode_F_type chip8_opcode::get_ops_type_F() const
{
    return chip8_opcode_F_type(code.chip8_opcode_srv_t.val);
}

chip8_opcode_E_type chip8_opcode::get_ops_type_E() const
{
    return chip8_opcode_E_type(code.chip8_opcode_srv_t.val);
}

bool chip8_opcode::is_cls_opcode() const { return code.opcode == 0x00E0; }

bool chip8_opcode::is_ret_opcode() const { return code.opcode == 0x00EE; }
