#include <iostream>
#include <string>
#include <memory>
#include "neon_osero.hpp"
#include "GameEngine.hpp"

int main(int argc, char* argv[]) {
    std::cout << "╔══════════════════════════════════════════════╗" << std::endl;
    std::cout << "║       🌈 ネオン・オセロ ── 起動！ ───────── ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════╝" << std::endl;

    // ゲーム設定
    neon_osero::GameConfig config{};
    config.enable_sound = true;
    config.enable_effects = true;
    config.cheat_mode = false;
    
    if (argc > 1 && std::string(argv[1]) == "--rainbow") {
        config.color_mode = neon_osero::Mode::RAINBOW;
    } else if (argc > 1 && std::string(argv[1]) == "--retro") {
        config.color_mode = neon_osero::Mode::RETRO;
    } else if (argc > 1 && std::string(argv[1]) == "--glitch" || 
               std::string(argv[1]).find("cheat") != std::string::npos) {
        config.cheat_mode = true;
        config.color_mode = neon_osero::Mode::RETRO;
    }

    // ゲームエンジン作成・初期化
    auto engine = std::make_unique<neon_osero::GameEngine>();
    engine->init(config);

    while (engine->is_game_over() == false) {
        std::cout << "\n┌─────────── 黒のターン ────────────────┐" << std::endl;
        
        // 有効な手を全て表示
        auto valid_moves = neon_osero::get_valid_moves(engine->board_);
        
        if (valid_moves.empty()) {
            break;
        }

        // プレイヤーの手入力（簡易版：ランダム選択）
        std::cout << "  有効な手：" << valid_moves.size() << "個" << std::endl;
        
        // ランダムに手を選ぶ（人間プレイ用）
        auto random_move = neon_osero::get_random_move(engine->board_);
        engine->make_move(random_move.first, random_move.second);

        // 盤面を表示
        std::cout << "  ┌─────────── 盤面 ───────────────────┐" << std::endl;
        std::string board_str = engine->board_to_string();
        for (const auto& line : board_str) {
            if (line != '\n') {
                std::cout << "  │ " << line;
            } else {
                std::cout << "  └─────────────────────────────────┘";
            }
        }
        std::cout << "\n" << std::endl;

        // ゲーム終了チェック
        if (engine->is_game_over()) break;

        std::cout << "\n┌─────────── 白のターン ────────────────┐" << std::endl;
        
        // AI（白）の手を選択
        auto ai_move = engine->ai_move(config.difficulty);
        if (ai_move.first != -1) {
            engine->make_move(ai_move.first, ai_move.second);

            // 盤面を表示
            std::cout << "  ┌─────────── 盤面 ───────────────────┐" << std::endl;
            board_str = engine->board_to_string();
            for (const auto& line : board_str) {
                if (line != '\n') {
                    std::cout << "  │ " << line;
                } else {
                    std::cout << "  └─────────────────────────────────┘";
                }
            }
            std::cout << "\n" << std::endl;

            // ゲーム終了チェック
            if (engine->is_game_over()) break;
        }
    }

    std::cout << "\n🎮 ゲームを終了します。" << std::endl;
    return 0;
}
