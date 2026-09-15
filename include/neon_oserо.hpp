#pragma once

#include <vector>
#include <array>
#include <string>

namespace neon_osero {

constexpr int BOARD_SIZE = 8;
enum class Color : unsigned char { EMPTY, BLACK, WHITE };
enum class Mode : uint32_t { NEON, RAINBOW, RETRO, GLITCH };

struct GameConfig {
    bool enable_sound{true};
    bool enable_effects{true};
    bool cheat_mode{false};  // シンメトリックプレイモード
    Mode color_mode{Mode::NEON};
    float difficulty{1.0f};   // AI の難易度（0.0〜2.0）
};

class Board;

} // namespace neon_osero
