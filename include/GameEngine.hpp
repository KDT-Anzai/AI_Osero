#pragma once

#include "neon_osero.hpp"
#include <string>
#include <vector>

namespace neon_osero {

// ゲームの進行管理クラス
class GameEngine {
public:
    // コンストラクタ（初期化）
    void init(GameConfig config);

    // 碁石を置く（指定された座標に置けるかチェックしてから）
    bool make_move(int x, int y);

    // ランダムで有効な手を取得（AI 用）
    std::pair<int,int> ai_move(float difficulty = 1.0f) const;

    // 現在のプレイヤーの碁石数を表示
    void show_score() const;

    // ゲーム終了判定と勝敗表示
    bool is_game_over() const;

    // 盤面の文字列出力（デバッグ用）
    std::string board_to_string() const;

private:
    GameConfig config_;
    std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE> board_;

    // ゲームの状態フラグ
    bool game_running_ = true;
};

} // namespace neon_osero
