#include <iostream>
#include <string>
#include <memory>
#include <random>
#include "neon_osero.hpp"
#include "GameEngine.hpp"
#include "sound_engine.hpp"

int main(int argc, char* argv[]) {
    // ────── コマンドライン引数の解析 ───────────────────────────────────────┐
    bool verbose = true;
    std::string mode = "normal";       // normal / ai / cheat / rainbow / retro / time_limit

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--ai") {
            mode = "ai";
        } else if (std::string(argv[i]) == "--ai-hard") {
            mode = "ai_hard";
        } else if (std::string(argv[i]) == "--cheat") {
            mode = "cheat";
        } else if (std::string(argv[i]) == "--rainbow") {
            mode = "rainbow";
        } else if (std::string(argv[i]) == "--retro") {
            mode = "retro";
        } else if (std::string(argv[i]) == "--time-limit" && i + 1 < argc) {
            mode = "time_limit";
        } else if (std::string(argv[i]) == "--sound") {
            std::cout << "[main] SE を有効化します！🎵\n";
            // SE を有効にする場合、コマンドラインから音を有効にするフラグも受ける
        }
    }

    // ────── ゲーム設定の作成 ───────────────────────────────────────────────┐
    neon_osero::GameConfig config{};
    
    switch (mode.c_str()[0]) {
        case 'a':  // ai, ai-hard
            config.enable_sound = true;
            break;
        case 't':  // time_limit
            config.limit_time_seconds = 30.0f;     // 制限時間 30 秒（デモ用）
            config.enable_sound = true;
            break;
        default:   // normal, cheat, rainbow, retro
            break;
    }

    if (mode == "rainbow") {
        config.color_mode = neon_osero::ColorMode::RAINBOW;
    } else if (mode == "retro") {
        config.color_mode = neon_osero::ColorMode::RETRO;
    } else if (mode == "time_limit" || mode == "ai") {
        config.enable_sound = true;
        config.flash_burst_available = false;  // タイムリミットモードでは閃光弾不可
    }

    std::cout << "\n╔══════════════════════════════════════════════╗" << std::endl;
    std::cout << "║       🌈 ネオン・オセロ ── 起動！ ───────── ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════╝\n";

    if (verbose) {
        std::cout << "\n【モード】" << mode << std::endl;
        if (!config.enable_sound) {
            std::cout << "  SE: OFF（コンソール演出のみ）" << std::endl;
        } else {
            std::cout << "  SE: ON 🎵\n";
        }
        switch (config.color_mode) {
            case neon_osero::ColorMode::NEON:   std::cout << "  カラーモード：ネオン（デフォルト）" << std::endl; break;
            case neon_osero::ColorMode::RAINBOW:std::cout << "  カラーモード：レインボウ 🌈\n"; break;
            case neon_osero::ColorMode::RETRO: std::cout << "  カラーモード：レトロ 💾\n"; break;
        }
    }

    // ────── ゲームエンジン作成と初期化 ───────────────────────────────────────┐
    auto engine = std::make_unique<neon_osero::GameEngine>(config);

    // 盤面初期表示
    if (verbose) {
        std::cout << "\n┌─────────── 初期盤面 ───────────────────┐" << std::endl;
        std::cout << "  ┌────────────────────────────────────┐\n";
        auto initial_board_str = engine->board_to_string();
        for (const char* line : initial_board_str) {
            if (*line != '\0') std::cout << "  │ " << line;
            else std::cout << "  └──────────────────────────────────┘\n";
        }
        std::cout << "└───────────────────────────────────────┘" << std::endl;
    }

    // ────── ゲームループ ───────────────────────────────────────────────────┐
    int turn = 0;
    auto moves = engine->get_valid_moves(engine->board_);

    while (engine->is_game_over() == false && !moves.empty()) {
        if (turn % 2 == 0) {
            // 黒のターン
            std::cout << "\n┌─────────── 黒のターン ────────────────┐" << std::endl;

            moves = engine->get_valid_moves(engine->board_);
            if (moves.empty()) break;

            // ランダム選択（人間プレイ用）
            int idx = static_cast<int>(std::rand() % moves.size());
            auto move = moves[idx];

            std::cout << "  🖤 黒が (" << move.first << "," << move.second << ") に碁石を置きました！" << std::endl;
            
            // 盤面表示＋スコア表示
            if (verbose) {
                auto board_str = engine->board_to_string();
                for (const char* line : board_str) {
                    if (*line != '\0') std::cout << "  │ " << line;
                    else std::cout << "  └──────────────────────────────────┘\n";
                }

                auto scores = engine->count_pieces(engine->board_);
                std::cout << "  ── 黒：" << scores.first << "   │   白：" << scores.second << std::endl;
            }

        } else {
            // 白のターン
            std::cout << "\n┌─────────── 白のターン ────────────────┐" << std::endl;

            moves = engine->get_valid_moves(engine->board_);
            if (moves.empty()) break;

            int idx = static_cast<int>(std::rand() % moves.size());
            auto move = moves[idx];

            std::cout << "  ⚪ 白が (" << move.first << "," << move.second << ") に碁石を置きました！" << std::endl;

            if (verbose) {
                auto board_str = engine->board_to_string();
                for (const char* line : board_str) {
                    if (*line != '\0') std::cout << "  │ " << line;
                    else std::cout << "  └──────────────────────────────────┘\n";
                }

                auto scores = engine->count_pieces(engine->board_);
                std::cout << "  ── 黒：" << scores.first << "   │   白：" << scores.second << std::endl;
            }
        }

        moves = engine->get_valid_moves(engine->board_);
        turn++;
    }

    // ────── ゲーム終了処理 ───────────────────────────────────────────────────┐
    if (engine->is_game_over()) {
        std::cout << "\n🎉 試合終了！" << std::endl;
        auto scores = engine->count_pieces(engine->board_);
        if (scores.first > scores.second) {
            std::cout << "🏆 黒の勝利です！" << std::endl;
        } else if (scores.second > scores.first) {
            std::cout << "🏆 白の勝利です！" << std::endl;
        } else {
            std::cout << "🤝 ドロキ！引き分けです！" << std::endl;
        }
    }

    std::cout << "\n✅ ゲームが完了しました。";
    return 0;
}
