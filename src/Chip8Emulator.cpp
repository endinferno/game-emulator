#include "Chip8Emulator.hpp"
#include "Logger.hpp"

uint8_t font_set[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, 0x20, 0x60, 0x20, 0x20, 0x70, 0xF0, 0x10,
    0xF0, 0x80, 0xF0, 0xF0, 0x10, 0xF0, 0x10, 0xF0, 0xA0, 0xA0, 0xF0, 0x20,
    0x20, 0xF0, 0x80, 0xF0, 0x10, 0xF0, 0xF0, 0x80, 0xF0, 0x90, 0xF0, 0xF0,
    0x10, 0x20, 0x40, 0x40, 0xF0, 0x90, 0xF0, 0x90, 0xF0, 0xF0, 0x90, 0xF0,
    0x10, 0xF0, 0xF0, 0x90, 0xF0, 0x90, 0x90, 0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0, 0xE0, 0x90, 0x90, 0x90, 0xE0, 0xF0, 0x80,
    0xF0, 0x80, 0xF0, 0xF0, 0x80, 0xF0, 0x80, 0x80,
};

Chip8Emulator::Chip8Emulator(uint8_t width, uint8_t height)
    : V_reg(CHIP8_REG_COUNT, 0)
    , engine(seed())
    , distrib(0, 255)
    , rom_stack()
    , screen_width(width)
    , screen_height(height)
    , screen(width, std::vector<uint8_t>(height, 0))
    , key_state(CHIP8_KEY_COUNT, false)
    , in_wait(false)
    , wait_reg(V_reg.begin())
    , pc(0x200)
{
    chip8_key_map = {
        { SDLK_1, 0x1 }, { SDLK_2, 0x8 }, { SDLK_3, 0x3 }, { SDLK_4, 0xC },
        { SDLK_q, 0x4 }, { SDLK_w, 0x5 }, { SDLK_e, 0x6 }, { SDLK_r, 0xD },
        { SDLK_a, 0x7 }, { SDLK_s, 0x2 }, { SDLK_d, 0x9 }, { SDLK_f, 0xE },
        { SDLK_z, 0xA }, { SDLK_x, 0x0 }, { SDLK_c, 0xB }, { SDLK_v, 0xF },
    };
}

void Chip8Emulator::SetRom(uint8_t* content, uint16_t len)
{
    memcpy(content, font_set, sizeof(font_set) / sizeof(uint8_t));
    rom_content = std::vector<uint8_t>(content, content + len);
}

void Chip8Emulator::DecreaseDelayTimer()
{
    if (delay_timer > 0) delay_timer--;
}

Chip8Opcode Chip8Emulator::Fetch()
{
    uint16_t opcode = static_cast<uint16_t>(rom_content[pc]);
    opcode <<= 8;
    opcode |= rom_content[pc + 1];
    pc += 2;
    return Chip8Opcode(opcode);
}

void Chip8Emulator::Decode(const Chip8Opcode& opcode)
{
    Chip8OpcodeType opcode_type = opcode.GetOpcodeType();
    switch (opcode_type) {
    case CHIP8_OPCODE_TYPE_0: DecodeOpcode0(opcode); break;
    case CHIP8_OPCODE_TYPE_1: DecodeOpcode1(opcode); break;
    case CHIP8_OPCODE_TYPE_2: DecodeOpcode2(opcode); break;
    case CHIP8_OPCODE_TYPE_3: DecodeOpcode3(opcode); break;
    case CHIP8_OPCODE_TYPE_4: DecodeOpcode4(opcode); break;
    case CHIP8_OPCODE_TYPE_5: DecodeOpcode5(opcode); break;
    case CHIP8_OPCODE_TYPE_6: DecodeOpcode6(opcode); break;
    case CHIP8_OPCODE_TYPE_7: DecodeOpcode7(opcode); break;
    case CHIP8_OPCODE_TYPE_8: DecodeOpcode8(opcode); break;
    case CHIP8_OPCODE_TYPE_9: DecodeOpcode9(opcode); break;
    case CHIP8_OPCODE_TYPE_A: DecodeOpcodeA(opcode); break;
    case CHIP8_OPCODE_TYPE_B: DecodeOpcodeB(opcode); break;
    case CHIP8_OPCODE_TYPE_C: DecodeOpcodeC(opcode); break;
    case CHIP8_OPCODE_TYPE_D: DecodeOpcodeD(opcode); break;
    case CHIP8_OPCODE_TYPE_E: DecodeOpcodeE(opcode); break;
    case CHIP8_OPCODE_TYPE_F: DecodeOpcodeF(opcode); break;
    default:
        ERROR("Unknown opcode type: {:#04X}", static_cast<int>(opcode_type));
        break;
    }
}

void Chip8Emulator::HandleKeyEvent(uint32_t keytype, int keycode)
{
    if (keytype != SDL_KEYDOWN && keytype != SDL_KEYUP) return;
    if (!chip8_key_map.count(keycode)) return;
    uint8_t chip8_key = chip8_key_map[keycode];
    key_state[chip8_key] = (keytype == SDL_KEYDOWN);
    if (!in_wait || !key_state[chip8_key]) return;
    in_wait = false;
    *wait_reg = chip8_key;
}

const std::vector<std::vector<uint8_t>>& Chip8Emulator::GetScreen() const
{
    return screen;
}

bool Chip8Emulator::IsWait() const
{
    return in_wait;
}

void Chip8Emulator::PushStack(uint16_t val)
{
    rom_stack.push(val);
}

uint16_t Chip8Emulator::PopStack()
{
    uint16_t ret = rom_stack.top();
    rom_stack.pop();
    return ret;
}

uint8_t Chip8Emulator::GetRandom()
{
    return distrib(engine);
}

void Chip8Emulator::DecodeOpcode0(const Chip8Opcode& opcode)
{
    if (opcode.IsClsOpcode()) {
        for (int i = 0; i < screen_width; i++)
            for (int j = 0; j < screen_height; j++) screen[i][j] = 0;
    }
    else if (opcode.IsRetOpcode()) {
        pc = PopStack();
    }
}

void Chip8Emulator::DecodeOpcode1(const Chip8Opcode& opcode)
{
    pc = opcode.GetAddr();
}

void Chip8Emulator::DecodeOpcode2(const Chip8Opcode& opcode)
{
    PushStack(pc);
    pc = opcode.GetAddr();
}

void Chip8Emulator::DecodeOpcode3(const Chip8Opcode& opcode)
{
    if (V_reg[opcode.GetRegX()] == opcode.GetKk()) pc += 2;
}

void Chip8Emulator::DecodeOpcode4(const Chip8Opcode& opcode)
{
    if (V_reg[opcode.GetRegX()] != opcode.GetKk()) pc += 2;
}

void Chip8Emulator::DecodeOpcode5(const Chip8Opcode& opcode)
{
    if (V_reg[opcode.GetRegX()] == V_reg[opcode.GetRegY()]) pc += 2;
}

void Chip8Emulator::DecodeOpcode6(const Chip8Opcode& opcode)
{
    V_reg[opcode.GetRegX()] = opcode.GetKk();
}

void Chip8Emulator::DecodeOpcode7(const Chip8Opcode& opcode)
{
    V_reg[opcode.GetRegX()] += opcode.GetKk();
}

void Chip8Emulator::DecodeOpcode8(const Chip8Opcode& opcode)
{
    uint8_t& reg_x = V_reg[opcode.GetRegX()];
    uint8_t& reg_y = V_reg[opcode.GetRegY()];
    uint8_t& VF_reg = V_reg[0xF];
    switch (opcode.GetOpsType8()) {
    case CHIP8_OPCODE_OPS_LDR: reg_x = reg_y; break;
    case CHIP8_OPCODE_OPS_ORR: reg_x |= reg_y; break;
    case CHIP8_OPCODE_OPS_AND: reg_x &= reg_y; break;
    case CHIP8_OPCODE_OPS_XOR: reg_x ^= reg_y; break;
    case CHIP8_OPCODE_OPS_ADD:
    {
        uint16_t sum =
            static_cast<uint16_t>(reg_x) + static_cast<uint16_t>(reg_y);
        VF_reg = sum > 0xFF ? 1 : 0;
        reg_x = static_cast<uint8_t>(sum);
        break;
    }
    case CHIP8_OPCODE_OPS_SUB:
    {
        VF_reg = reg_x < reg_y ? 0 : 1;
        reg_x = reg_x - reg_y;
        break;
    }
    case CHIP8_OPCODE_OPS_SHR:
    {
        VF_reg = reg_x & 0x1;
        reg_x >>= 1;
        break;
    }
    case CHIP8_OPCODE_OPS_SBN:
    {
        VF_reg = reg_y < reg_x ? 0 : 1;
        reg_x = reg_y - reg_x;
        break;
    }
    case CHIP8_OPCODE_OPS_SHL:
    {
        VF_reg = (reg_x >> 7) & 0x1;
        reg_x <<= 1;
        break;
    }
    }
}

void Chip8Emulator::DecodeOpcode9(const Chip8Opcode& opcode)
{
    if (V_reg[opcode.GetRegX()] != V_reg[opcode.GetRegY()]) pc += 2;
}

void Chip8Emulator::DecodeOpcodeA(const Chip8Opcode& opcode)
{
    I_reg = opcode.GetAddr();
}

void Chip8Emulator::DecodeOpcodeB(const Chip8Opcode& opcode)
{
    pc = opcode.GetAddr() + V_reg[0];
}

void Chip8Emulator::DecodeOpcodeC(const Chip8Opcode& opcode)
{
    V_reg[opcode.GetRegX()] = GetRandom() & opcode.GetKk();
}

void Chip8Emulator::DecodeOpcodeD(const Chip8Opcode& opcode)
{
    uint8_t loc_x_start = V_reg[opcode.GetRegX()] % screen_width;
    uint8_t loc_y_start = V_reg[opcode.GetRegY()] % screen_height;
    uint8_t nibble = opcode.GetNibble();
    std::vector<uint8_t>::iterator rom_it = rom_content.begin() + I_reg;
    uint8_t& VF_reg = V_reg[0xF];
    VF_reg = 0;
    for (uint8_t x = 0; x < nibble; x++) {
        uint8_t sprite = *(rom_it + x);
        for (uint8_t bit = 0; bit < 8; bit++) {
            uint8_t loc_x = (loc_x_start + bit) % screen_width;
            uint8_t loc_y = (loc_y_start + x) % screen_height;
            uint8_t pixel = (sprite >> (7 - bit)) & 0x1;
            if (screen[loc_x][loc_y] == 1 && pixel == 1) VF_reg = 1;
            screen[loc_x][loc_y] ^= pixel;
        }
    }
}

void Chip8Emulator::DecodeOpcodeE(const Chip8Opcode& opcode)
{
    uint8_t key = V_reg[opcode.GetRegX()];
    switch (opcode.GetOpsTypeE()) {
    case CHIP8_OPCODE_SK_KP:
        if (key_state[key]) pc += 2;
        break;
    case CHIP8_OPCODE_SK_KN:
        if (!key_state[key]) pc += 2;
        break;
    }
}

void Chip8Emulator::DecodeOpcodeF(const Chip8Opcode& opcode)
{
    switch (opcode.GetOpsTypeF()) {
    case CHIP8_OPCODE_LD_DT: V_reg[opcode.GetRegX()] = delay_timer; break;
    case CHIP8_OPCODE_LD_KP:
        in_wait = true;
        wait_reg = V_reg.begin() + opcode.GetRegX();
        break;
    case CHIP8_OPCODE_ST_DT: delay_timer = V_reg[opcode.GetRegX()]; break;
    case CHIP8_OPCODE_ST_ST: sound_timer = V_reg[opcode.GetRegX()]; break;
    case CHIP8_OPCODE_AD_IR: I_reg += V_reg[opcode.GetRegX()]; break;
    case CHIP8_OPCODE_LD_DG: I_reg = V_reg[opcode.GetRegX()] * 5; break;
    case CHIP8_OPCODE_ST_BR:
    {
        uint8_t val = V_reg[opcode.GetRegX()];
        rom_content[I_reg] = val / 100;
        rom_content[I_reg + 1] = (val / 10) % 10;
        rom_content[I_reg + 2] = val % 10;
        break;
    }
    case CHIP8_OPCODE_ST_RG:
    {
        uint8_t reg = opcode.GetRegX();
        for (uint8_t i = 0; i <= reg; i++) rom_content[I_reg + i] = V_reg[i];
        I_reg += (reg + 1);
        break;
    }
    case CHIP8_OPCODE_FL_RG:
    {
        uint8_t reg = opcode.GetRegX();
        for (uint8_t i = 0; i <= reg; i++) V_reg[i] = rom_content[I_reg + i];
        I_reg += (reg + 1);
        break;
    }
    }
}
