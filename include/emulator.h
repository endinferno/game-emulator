#ifndef __EMULATOR_H
#define __EMULATOR_H

#include <stack>
#include <vector>
#include <random>
#include <unordered_map>
#include <SDL2/SDL.h>
#include "chip8_opcode.h"

class chip8_emulator
{
public:
    chip8_emulator(uint8_t width, uint8_t height);

    void set_rom(uint8_t* content, uint16_t len);
    void decrease_delay_timer();
    chip8_opcode fetch();
    void decode(const chip8_opcode& opcode);
    void handle_key_event(uint32_t keytype, int keycode);

    const std::vector<std::vector<uint8_t>>& get_screen() const;

    bool is_wait() const;

    ~chip8_emulator();

private:
    void push_stack(uint16_t val);
    uint16_t pop_stack();
    uint8_t get_random();
    void decode_opcode_0(const chip8_opcode& opcode);
    void decode_opcode_1(const chip8_opcode& opcode);
    void decode_opcode_2(const chip8_opcode& opcode);
    void decode_opcode_3(const chip8_opcode& opcode);
    void decode_opcode_4(const chip8_opcode& opcode);
    void decode_opcode_5(const chip8_opcode& opcode);
    void decode_opcode_6(const chip8_opcode& opcode);
    void decode_opcode_7(const chip8_opcode& opcode);
    void decode_opcode_8(const chip8_opcode& opcode);
    void decode_opcode_9(const chip8_opcode& opcode);
    void decode_opcode_A(const chip8_opcode& opcode);
    void decode_opcode_B(const chip8_opcode& opcode);
    void decode_opcode_C(const chip8_opcode& opcode);
    void decode_opcode_D(const chip8_opcode& opcode);
    void decode_opcode_E(const chip8_opcode& opcode);
    void decode_opcode_F(const chip8_opcode& opcode);

private:
    static const uint8_t CHIP8_STACK_SIZE = 0xFF;
    static const uint8_t CHIP8_REG_COUNT = 16;
    static const uint8_t CHIP8_KEY_COUNT = 16;
    uint8_t screen_width;
    uint8_t screen_height;

    uint16_t pc;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t I_reg;
    std::vector<uint8_t> V_reg;
    std::vector<bool> key_state;

    std::vector<uint8_t> rom_content;
    std::stack<uint16_t> rom_stack;
    std::vector<std::vector<uint8_t>> screen;
    bool in_wait;
    std::vector<uint8_t>::iterator wait_reg;

    std::unordered_map<int, uint8_t> chip8_key_map;

    std::random_device seed;
    std::ranlux48 engine;
    std::uniform_int_distribution<> distrib;
};

#endif /* __EMULATOR_H */
