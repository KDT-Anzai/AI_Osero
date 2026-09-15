#pragma once

#include <array>
#include <string>
#include <functional>
#include <memory>

namespace neon_osero {

// ────── 基本定数と型 ───────────────────────────────────────────────────┐
constexpr int BOARD_SIZE = 8;
enum class Color : unsigned char { EMPTY, BLACK, WHITE };
enum class ColorMode { NEON, RAINBOW, RETRO };
// ───────────────────────────────────────────────────────────────────────┘

struct GameConfig {
    bool enable_sound{false};           // SE を有効にするか
    float limit_time_seconds{0.0f};     // タイムリミットモードの制限時間（秒）
    bool flash_burst_available{true};    // 閃光弾の使用可否
    ColorMode color_mode{ColorMode::NEON}; // 視覚エフェクトモード

    float limit_gauge = 0.0f;           // リミットゲージ（0.0〜1.0）

    void reset_limit_gauge() {
        limit_gauge = 0.0f;
        flash_burst_available = false;
    }
};

// ────── Sound Engine ───────────────────────────────────────────────────┐
class SoundEngine {
public:
    enum class SoundId : uint32_t {
        SFX_PLACE,      // 碁石を置く音
        SFX_FLIP,       // 相手の碁を反転させる音
        SFX_LIMIT_BURST,// 閃光弾発動の演出音
        SFX_GAME_OVER,  // ゲーム終了演出音
        SFX_WIN,        // 勝利演出音
    };

    void init();
    void play_sound(SoundId id) const;
    void set_event_callback(std::function<void(const SoundId&)> callback);

private:
    bool enabled_ = false;
    std::function<void(const SoundId&)> event_callback_;
};

// ────── Game Engine ───────────────────────────────────────────────────┐
class GameEngine {
public:
    // コンストラクタ（GameConfig を受け取る）
    explicit GameEngine(GameConfig config = {});
    
    // デストラクタ
    ~GameEngine();

    // 初期化（盤面作成・設定適用・SE の初期化）
    void init(GameConfig config);

    // リミットゲージの計算と更新
    void calculate_limit_gauge();

    // 碁石を置く処理（新ルール：リミットゲージ更新・閃光弾発動判定）
    bool make_move(int x, int y);

    // 有効な手を取得
    std::vector<std::pair<int,int>> get_valid_moves(const std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE>& board) const;

    // ランダム手（AI 用）
    std::pair<int,int> get_random_move(const std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE>& board);

    // ゲーム進行ループ（デモ用）
    void run_demo();

    // 盤面の文字列出力
    std::string board_to_string() const;
    std::string board_to_rainbow_string() const;
    std::string board_to_retro_string() const;

    // ゲームオーバー時の勝敗表示
    void show_game_over() const;

    // 碁石の配置数を返す（黒・白の数をペアで）
    std::pair<int,int> count_pieces(const std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE>& board) const;

private:
    GameConfig config_;
    std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE> board_;
    bool game_running_ = true;
    float limit_gauge_ = 0.0f;
    bool flash_burst_available_ = false;
    bool flash_burst_triggered_ = false;

    std::unique_ptr<SoundEngine> sound_engine_;
};

// ────── ヘルパー関数 ───────────────────────────────────────────────────┐
Color get_current_player(const std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE>& board);
bool can_place_piece(const std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE>& board, int x, int y);
std::pair<int,int> get_random_move_with_difficulty(const std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE>& board, float difficulty);

} // namespace neon_osero
