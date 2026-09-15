#include "GameEngine.hpp"
#include <random>
#include <algorithm>
#include <iostream>

namespace neon_osero {

// コンストラクタ（初期化）
void GameEngine::init(GameConfig config) {
    config_ = config;
    game_running_ = true;

    // 盤面の初期状態を作成
    board_ = create_initial_board();
}

// 碁石を置く処理
bool GameEngine::make_move(int x, int y) {
    if (!game_running_) return false;

    // 有効な手かチェック
    auto valid_moves = get_valid_moves(board_);
    
    if (valid_moves.empty()) {
        std::cout << "🚫 置ける碁石がありません！ゲーム終了です。" << std::endl;
        game_running_ = false;
        return false;
    }

    // 指定された座標に置くか、有効な手の一つを選ぶ（デモ用）
    bool found = false;
    for (auto& move : valid_moves) {
        if (move.first == x && move.second == y) {
            place_piece(board_, x, y);
            game_running_ = true;
            show_score();
            std::cout << "✓ 碁石を置きました！" << std::endl;
            return true;
        }
    }

    // 指定された座標に置けない場合、ランダムな手を打つ（デモ用）
    if (!found && !config_.cheat_mode) {
        auto random_move = get_random_move(board_);
        place_piece(board_, random_move.first, random_move.second);
        game_running_ = true;
        show_score();
        std::cout << "✓ ランダムに碁石を置きました！" << std::endl;
    }

    return true;
}

// AI が動かす処理（難易度付き）
std::pair<int,int> GameEngine::ai_move(float difficulty) const {
    auto moves = get_valid_moves(board_);
    
    if (moves.empty()) {
        std::cout << "🤖 AI: 手が見つかりません。" << std::endl;
        return {-1, -1};
    }

    if (config_.cheat_mode) {
        // シンメトリックプレイ（完全な対称性維持）の場合、対称な座標を選ぶ
        for (auto& move : moves) {
            int sx = BOARD_SIZE - 1 - move.first;
            int sy = BOARD_SIZE - 1 - move.second;
            if (board_[sy][sx] == Color::EMPTY) {
                return {sx, sy};
            }
        }
    }

    // 難易度に基づいて手を選ぶ
    auto random_move = get_random_move_with_difficulty(board_, difficulty);
    return random_move;
}

// 現在のプレイヤーの碁石数を表示
void GameEngine::show_score() const {
    int black_count = 0;
    int white_count = 0;
    
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            if (board_[y][x] == Color::BLACK) black_count++;
            else if (board_[y][x] == Color::WHITE) white_count++;
        }
    }

    std::cout << "━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
    std::cout << "  黒：" << black_count << "   │   白：" << white_count << std::endl;
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
}

// ゲーム終了判定と勝敗表示
bool GameEngine::is_game_over() const {
    auto valid_moves = get_valid_moves(board_);
    
    if (valid_moves.empty()) {
        int black_count = 0, white_count = 0;
        for (int y = 0; y < BOARD_SIZE; ++y) {
            for (int x = 0; x < BOARD_SIZE; ++x) {
                if (board_[y][x] == Color::BLACK) black_count++;
                else if (board_[y][x] == Color::WHITE) white_count++;
            }
        }

        std::cout << "🎉 ゲーム終了！" << std::endl;
        std::cout << "  黒：" << black_count << " vs 白：" << white_count << std::endl;

        if (black_count > white_count) {
            std::cout << "🏆 黒が勝利しました！" << std::endl;
        } else if (white_count > black_count) {
            std::cout << "🏆 白が勝利しました！" << std::endl;
        } else {
            std::cout << "🤝 ドロキ！引き分けです！" << std::endl;
        }

        return true;
    }

    return false;
}

// 盤面の文字列出力（デバッグ用）
std::string GameEngine::board_to_string() const {
    std::string result = "   ";
    for (int x = 0; x < BOARD_SIZE; ++x) result += "-------";
    result += "\n";

    for (int y = 0; y < BOARD_SIZE; ++y) {
        result += std::to_string(y);
        result += "| ";
        for (int x = 0; x < BOARD_SIZE; ++x) {
            switch (board_[y][x]) {
                case Color::EMPTY:   result += " . "; break;
                case Color::BLACK:   result += " B "; break;
                case Color::WHITE:   result += " W "; break;
                default:             result += "?  "; break;
            }
        }
        result += "|\n";
    }

    return result;
}

// コンストラクタ（デフォルト）
GameEngine::GameEngine() {
    board_ = create_initial_board();
    game_running_ = true;
}

// デストラクタ
GameEngine::~GameEngine() {}

} // namespace neon_osero
