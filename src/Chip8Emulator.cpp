#include "Chip8Emulator.hpp"
#include "Logger.hpp"

Chip8Emulator::Chip8Emulator(uint8_t winWidth, uint8_t winHeight)
    : winWidth_(winWidth)
    , winHeight_(winHeight)
    , screen_(winWidth, std::vector<uint8_t>(winHeight, 0))
    , waitReg_(vReg_.begin())
    , engine_(seed_())
    , distrib_(0, 255)
{
    vReg_.fill(0);
    keyState_.fill(false);
}

void Chip8Emulator::SetRom(uint8_t* content, uint16_t len)
{
    memcpy(content, fontSet_.data(), fontSet_.size());
    romContent_ = std::vector<uint8_t>(content, content + len);
}

void Chip8Emulator::DecreaseDelayTimer()
{
    if (delayTimer_ > 0) {
        delayTimer_--;
    }
}

Chip8Opcode Chip8Emulator::Fetch()
{
    auto opcode = static_cast<uint16_t>(romContent_[pc_]);
    opcode <<= 8;
    opcode |= romContent_[pc_ + 1];
    pc_ += 2;
    return { opcode };
}

void Chip8Emulator::Decode(const Chip8Opcode& opcode)
{
    Chip8OpcodeType opcodeType = opcode.GetOpcodeType();

    switch (opcodeType) {
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
    {
        ERROR("Unknown opcode type: {:#04X}", static_cast<int>(opcodeType));
        break;
    }
    }
}

void Chip8Emulator::HandleEvent(const sf::Event& event)
{
    HandleCloseEvent(event);
    HandleKeyEvent(event);
}

void Chip8Emulator::HandleCloseEvent(const sf::Event& event)
{
    if (event.type == sf::Event::Closed) {
        isQuit_ = true;
    }
}

void Chip8Emulator::HandleKeyEvent(const sf::Event& event)
{
    auto keyType = event.type;
    auto keyCode = event.key.code;

    if (keyType != sf::Event::EventType::KeyPressed &&
        keyType != sf::Event::EventType::KeyReleased) {
        return;
    }
    if (!chip8KeyMap_.count(keyCode)) {
        return;
    }
    uint8_t chip8Key = chip8KeyMap_[keyCode];
    keyState_[chip8Key] = (keyType == sf::Event::EventType::KeyPressed);
    if (!inWait_ || !keyState_[chip8Key]) {
        return;
    }
    inWait_ = false;
    *waitReg_ = chip8Key;
}

const std::vector<std::vector<uint8_t>>& Chip8Emulator::GetScreen() const
{
    return screen_;
}

bool Chip8Emulator::IsWait() const
{
    return inWait_;
}

bool Chip8Emulator::IsQuit() const
{
    return isQuit_;
}

void Chip8Emulator::PushStack(uint16_t val)
{
    romStack_.push(val);
}

uint16_t Chip8Emulator::PopStack()
{
    uint16_t ret = romStack_.top();
    romStack_.pop();
    return ret;
}

uint8_t Chip8Emulator::GetRandom()
{
    return distrib_(engine_);
}

void Chip8Emulator::DecodeOpcode0(const Chip8Opcode& opcode)
{
    if (opcode.IsClsOpcode()) {
        std::for_each(screen_.begin(), screen_.end(), [](auto& row) {
            std::fill(row.begin(), row.end(), 0);
        });
    } else if (opcode.IsRetOpcode()) {
        pc_ = PopStack();
    }
}

void Chip8Emulator::DecodeOpcode1(const Chip8Opcode& opcode)
{
    pc_ = opcode.GetAddr();
}

void Chip8Emulator::DecodeOpcode2(const Chip8Opcode& opcode)
{
    PushStack(pc_);
    pc_ = opcode.GetAddr();
}

void Chip8Emulator::DecodeOpcode3(const Chip8Opcode& opcode)
{
    if (vReg_[opcode.GetRegX()] == opcode.GetKk()) {
        pc_ += 2;
    }
}

void Chip8Emulator::DecodeOpcode4(const Chip8Opcode& opcode)
{
    if (vReg_[opcode.GetRegX()] != opcode.GetKk()) {
        pc_ += 2;
    }
}

void Chip8Emulator::DecodeOpcode5(const Chip8Opcode& opcode)
{
    if (vReg_[opcode.GetRegX()] == vReg_[opcode.GetRegY()]) {
        pc_ += 2;
    }
}

void Chip8Emulator::DecodeOpcode6(const Chip8Opcode& opcode)
{
    vReg_[opcode.GetRegX()] = opcode.GetKk();
}

void Chip8Emulator::DecodeOpcode7(const Chip8Opcode& opcode)
{
    vReg_[opcode.GetRegX()] += opcode.GetKk();
}

void Chip8Emulator::DecodeOpcode8(const Chip8Opcode& opcode)
{
    uint8_t& regX = vReg_[opcode.GetRegX()];
    uint8_t& regY = vReg_[opcode.GetRegY()];
    uint8_t& vfReg = vReg_[0xF];
    switch (opcode.GetOpsType8()) {
    case CHIP8_OPCODE_OPS_LDR:
    {
        regX = regY;
        break;
    }
    case CHIP8_OPCODE_OPS_ORR:
    {
        regX |= regY;
        break;
    }
    case CHIP8_OPCODE_OPS_AND:
    {
        regX &= regY;
        break;
    }
    case CHIP8_OPCODE_OPS_XOR:
    {
        regX ^= regY;
        break;
    }
    case CHIP8_OPCODE_OPS_ADD:
    {
        uint16_t sum =
            static_cast<uint16_t>(regX) + static_cast<uint16_t>(regY);
        vfReg = sum > 0xFF ? 1 : 0;
        regX = static_cast<uint8_t>(sum);
        break;
    }
    case CHIP8_OPCODE_OPS_SUB:
    {
        vfReg = regX < regY ? 0 : 1;
        regX = regX - regY;
        break;
    }
    case CHIP8_OPCODE_OPS_SHR:
    {
        vfReg = regX & 0x1;
        regX >>= 1;
        break;
    }
    case CHIP8_OPCODE_OPS_SBN:
    {
        vfReg = regY < regX ? 0 : 1;
        regX = regY - regX;
        break;
    }
    case CHIP8_OPCODE_OPS_SHL:
    {
        vfReg = (regX >> 7) & 0x1;
        regX <<= 1;
        break;
    }
    }
}

void Chip8Emulator::DecodeOpcode9(const Chip8Opcode& opcode)
{
    if (vReg_[opcode.GetRegX()] != vReg_[opcode.GetRegY()]) {
        pc_ += 2;
    }
}

void Chip8Emulator::DecodeOpcodeA(const Chip8Opcode& opcode)
{
    iReg_ = opcode.GetAddr();
}

void Chip8Emulator::DecodeOpcodeB(const Chip8Opcode& opcode)
{
    pc_ = opcode.GetAddr() + vReg_[0];
}

void Chip8Emulator::DecodeOpcodeC(const Chip8Opcode& opcode)
{
    vReg_[opcode.GetRegX()] = GetRandom() & opcode.GetKk();
}

void Chip8Emulator::DecodeOpcodeD(const Chip8Opcode& opcode)
{
    uint8_t locXStart = vReg_[opcode.GetRegX()] % winWidth_;
    uint8_t locYStart = vReg_[opcode.GetRegY()] % winHeight_;
    uint8_t nibble = opcode.GetNibble();
    auto romIt = romContent_.begin() + iReg_;
    uint8_t& vfReg = vReg_[0xF];
    vfReg = 0;
    for (uint8_t x = 0; x < nibble; x++) {
        uint8_t sprite = *(romIt + x);
        for (uint8_t bit = 0; bit < 8; bit++) {
            uint8_t locX = (locXStart + bit) % winWidth_;
            uint8_t locY = (locYStart + x) % winHeight_;
            uint8_t pixel = (sprite >> (7 - bit)) & 0x1;
            if (screen_[locX][locY] == 1 && pixel == 1) {
                vfReg = 1;
            }
            screen_[locX][locY] ^= pixel;
        }
    }
}

void Chip8Emulator::DecodeOpcodeE(const Chip8Opcode& opcode)
{
    uint8_t key = vReg_[opcode.GetRegX()];
    switch (opcode.GetOpsTypeE()) {
    case CHIP8_OPCODE_SK_KP:
    {
        if (keyState_[key]) pc_ += 2;
        break;
    }
    case CHIP8_OPCODE_SK_KN:
    {
        if (!keyState_[key]) pc_ += 2;
        break;
    }
    }
}

void Chip8Emulator::DecodeOpcodeF(const Chip8Opcode& opcode)
{
    switch (opcode.GetOpsTypeF()) {
    case CHIP8_OPCODE_LD_DT:
    {
        vReg_[opcode.GetRegX()] = delayTimer_;
        break;
    }
    case CHIP8_OPCODE_LD_KP:
    {
        inWait_ = true;
        waitReg_ = vReg_.begin() + opcode.GetRegX();
        break;
    }
    case CHIP8_OPCODE_ST_DT:
    {
        delayTimer_ = vReg_[opcode.GetRegX()];
        break;
    }
    case CHIP8_OPCODE_ST_ST:
    {
        soundTimer_ = vReg_[opcode.GetRegX()];
        break;
    }
    case CHIP8_OPCODE_AD_IR:
    {
        iReg_ += vReg_[opcode.GetRegX()];
        break;
    }
    case CHIP8_OPCODE_LD_DG:
    {
        iReg_ = vReg_[opcode.GetRegX()] * 5;
        break;
    }
    case CHIP8_OPCODE_ST_BR:
    {
        uint8_t val = vReg_[opcode.GetRegX()];
        romContent_[iReg_] = val / 100;
        romContent_[iReg_ + 1] = (val / 10) % 10;
        romContent_[iReg_ + 2] = val % 10;
        break;
    }
    case CHIP8_OPCODE_ST_RG:
    {
        uint8_t reg = opcode.GetRegX();
        for (uint8_t i = 0; i <= reg; i++) romContent_[iReg_ + i] = vReg_[i];
        iReg_ += (reg + 1);
        break;
    }
    case CHIP8_OPCODE_FL_RG:
    {
        uint8_t reg = opcode.GetRegX();
        for (uint8_t i = 0; i <= reg; i++) vReg_[i] = romContent_[iReg_ + i];
        iReg_ += (reg + 1);
        break;
    }
    }
}
