#include "GameEngine.hpp"
#include "neon_osero.hpp"
#include <iostream>
#include <random>
#include <chrono>

namespace neon_osero {

// ────── GameEngine 実装 ───────────────────────────────────────────────────┐

// コンストラクタ（GameConfig を受け取る）
GameEngine::GameEngine(GameConfig config) :
    config_(config),
    board_(create_initial_board()),
    game_running_(true),
    limit_gauge_(0.0f),
    flash_burst_available_(false),
    flash_burst_triggered_(false)
{
}

// デストラクタ（SE のクリーンアップ）
GameEngine::~GameEngine() {
    sound_engine_.reset();
}

// 初期化（盤面作成・設定適用・SE の初期化）
void GameEngine::init(GameConfig config) {
    config_ = config;
    game_running_ = true;
    board_ = create_initial_board();
    limit_gauge_ = 0.0f;
    flash_burst_available_ = false;
    flash_burst_triggered_ = false;

    if (config.enable_sound) {
        sound_engine_ = std::make_unique<SoundEngine>();
        sound_engine_->init();
    } else {
        sound_engine_.reset();
    }
}

// リミットゲージの計算と更新（置かれた碁石の数に応じて増加）
void GameEngine::calculate_limit_gauge() {
    if (!config_.enable_sound || config_.limit_time_seconds <= 0.0f) return;

    int total_pieces = count_pieces(board_);
    limit_gauge_ += static_cast<float>(total_pieces) * 0.15f;

    if (limit_gauge_ > 1.0f) {
        limit_gauge_ = 1.0f;
    }

    // ゲージが 95% を超えたとき、閃光弾を使用可能にする
    if (!flash_burst_available_ && limit_gauge_ >= 0.95f) {
        flash_burst_available_ = true;
        std::cout << "[GameEngine] ⚡ 閃光弾が使用可能になりました！⚡\n";

        // SE を有効にする（音ありモード）
        if (!sound_engine_) {
            sound_engine_ = std::make_unique<SoundEngine>();
            sound_engine_->init();
        }
    }
}

// 碁石を置く処理（新ルール：リミットゲージ更新・閃光弾発動判定）
bool GameEngine::make_move(int x, int y) {
    if (!game_running_) return false;

    auto valid_moves = get_valid_moves(board_);

    if (valid_moves.empty()) {
        std::cout << "\n🚫 置ける碁石がありません！ゲーム終了です。\n";
        game_running_ = false;
        SoundEngine::play_sound(SoundId::SFX_GAME_OVER);
        return true;
    }

    // 指定された座標に置くか、ランダム選択（デモ用）
    bool found = false;
    for (const auto& move : valid_moves) {
        if (move.first == x && move.second == y) {
            place_piece(board_, x, y);

            // リミットゲージの更新
            calculate_limit_gauge();

            // 盤面表示＋スコア表示
            std::cout << "┌─────────── 黒のターン ────────────────┐" << std::endl;

            auto scores = count_pieces(board_);
            std::cout << "  🖤 黒が (" << x << "," << y << ") に碁石を置きました！" << std::endl;

            if (flash_burst_available_) {
                int gauge_int = static_cast<int>(limit_gauge_ * 100);
                std::cout << "  🌈【閃光弾】リミットゲージ：" << gauge_int << "%\n";
            }

            auto board_str = board_to_string();
            for (const char* line : board_str) {
                if (*line != '\0') std::cout << "  │ " << line;
                else std::cout << "  └──────────────────────────────────┘\n";
            }

            std::cout << "  ── 黒：" << scores.first << "   │   白：" << scores.second << "\n\n";
            found = true;
            break;
        }
    }

    // 指定された座標に置けない場合、ランダム選択（デモ用）
    if (!found) {
        auto random_move = get_random_move(board_);
        place_piece(board_, random_move.first, random_move.second);

        calculate_limit_gauge();

        std::cout << "┌─────────── 黒のターン ────────────────┐" << std::endl;
        auto scores = count_pieces(board_);
        std::cout << "  🖤 黒がランダムに碁石を置きました！(" << random_move.first << "," << random_move.second << ")" << std::endl;

        if (flash_burst_available_) {
            int gauge_int = static_cast<int>(limit_gauge_ * 100);
            std::cout << "  🌈【閃光弾】リミットゲージ：" << gauge_int << "%\n";
        }

        auto board_str = board_to_string();
        for (const char* line : board_str) {
            if (*line != '\0') std::cout << "  │ " << line;
            else std::cout << "  └──────────────────────────────────┘\n";
        }

        std::cout << "  ── 黒：" << scores.first << "   │   白：" << scores.second << "\n\n";
    }

    // ゲーム終了チェック
    if (is_game_over(board_)) return true;

    return false;
}

// 碁石を置く処理（囲い込み）
void GameEngine::place_piece(std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE>& board, int x, int y) {
    const Color current_player = get_current_player(board);
    board[y][x] = (current_player == Color::BLACK) ? Color::BLACK : Color::WHITE;

    // 囲い込み判定と反転処理
    const int dx[] = {-1,-1,-1,0,0,1,1,1};
    const int dy[] = {-1,0,1,-1,1,-1,0,1};

    for (int i = 0; i < 8; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
            Color neighbor_color = board[ny][nx];
            // 違う色の碁石がある場合、その方向で挟めるかチェック
            if (neighbor_color != current_player && neighbor_color != Color::EMPTY) {
                int cx = nx + dx[i], cy = ny + dy[i];
                while (cx >= 0 && cx < BOARD_SIZE && cy >= 0 && cy < BOARD_SIZE) {
                    if (board[cy][cx] == current_player) {
                        // この方向で挟める → 囲える碁石を反転
                        cx += dx[i];
                        cy += dy[i];
                        while (cx >= 0 && cx < BOARD_SIZE && cy >= 0 && cy < BOARD_SIZE) {
                            if (board[cy][cx] == current_player) {
                                board[cy][cx] = (current_player == Color::BLACK) ? Color::WHITE : Color::BLACK;
                            } else if (board[cy][cx] != Color::EMPTY) {
                                break; // 違う色の碁石 → この方向は囲えない
                            }
                        }
                    } else if (board[cy][cx] == Color::EMPTY || board[cy][cx] == neighbor_color) {
                        cx += dx[i];
                        cy += dy[i];
                    } else {
                        break; // 違う色の碁石 → この方向は囲えない
                    }
                }
            }
        }
    }
}

// ゲームオーバー時の勝敗表示
void GameEngine::show_game_over() const {
    int black_count = 0, white_count = 0;
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            if (board_[y][x] == Color::BLACK) black_count++;
            else if (board_[y][x] == Color::WHITE) white_count++;
        }
    }

    std::cout << "┌─────────── ゲーム終了 ───────────────────┐" << std::endl;
    std::cout << "  黒：" << black_count << "   │   白：" << white_count << std::endl;

    if (black_count > white_count) {
        std::cout << "  🏆 黒が勝利しました！\n";
    } else if (white_count > black_count) {
        std::cout << "  🏆 白が勝利しました！\n";
    } else {
        std::cout << "  🤝 ドロキ！引き分けです！\n";
    }

    SoundEngine::play_sound(SoundId::SFX_GAME_OVER);
}

// ランダムで有効な手を取得（AI 用）
std::pair<int,int> GameEngine::get_random_move(const std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE>& board) {
    auto moves = get_valid_moves(board);
    if (moves.empty()) return {-1, -1};

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, static_cast<int>(moves.size()) - 1);

    int idx = dis(gen);
    return moves[idx];
}

// コンソール用の盤面表示（カラーモード対応）
std::string GameEngine::board_to_string() const {
    if (config_.color_mode == ColorMode::RAINBOW) {
        return board_to_rainbow_string();
    } else if (config_.color_mode == ColorMode::RETRO) {
        return board_to_retro_string();
    } else {
        // NEON デフォルト
        std::string result = "   ";
        for (int x = 0; x < BOARD_SIZE; ++x) result += "-------";
        result += "\n";

        for (int y = 0; y < BOARD_SIZE; ++y) {
            result += std::to_string(y);
            result += "| ";
            for (int x = 0; x < BOARD_SIZE; ++x) {
                if (board_[y][x] == Color::EMPTY) {
                    result += " . ";
                } else if (board_[y][x] == Color::BLACK) {
                    // ネオンエフェクト：黒＝●（点付き）
                    result += " ● ";
                } else if (board_[y][x] == Color::WHITE) {
                    // ネオンエフェクト：白＝○（輪っか）
                    result += " ○ ";
                }
            }
            result += "|";
            result += "\n";
        }
        return result;
    }
}

std::string GameEngine::board_to_rainbow_string() const {
    // レインボウモード：碁石を虹色のグラデーションで表示
    std::string result = "   ";
    for (int x = 0; x < BOARD_SIZE; ++x) result += "-------";
    result += "\n";

    static const char* rainbow_chars[8] = {".", ".", ".", ".", ".", ".", ".", "."};

    for (int y = 0; y < BOARD_SIZE; ++y) {
        result += std::to_string(y);
        result += "| ";
        for (int x = 0; x < BOARD_SIZE; ++x) {
            if (board_[y][x] == Color::EMPTY) {
                result += " . ";
            } else if (board_[y][x] == Color::BLACK) {
                // レインボウ：黒＝虹色グラデーション（位置による）
                int pattern = ((y + x) % 8);
                switch (pattern) {
                    case 0: result += " 🟣 "; break; // Purple
                    case 1: result += " 🟠 "; break; // Orange
                    case 2: result += " 🔵 "; break; // Blue
                    case 3: result += " 🟢 "; break; // Green
                    case 4: result += " 🔴 "; break; // Red
                    case 5: result += " 🟡 "; break; // Yellow
                    case 6: result += " 🟣 "; break;
                    default:   result += " 🌈 "; break;
                }
            } else if (board_[y][x] == Color::WHITE) {
                int pattern = ((y - x + BOARD_SIZE) % 8);
                switch (pattern) {
                    case 0: result += " 🟣 "; break;
                    case 1: result += " 🟠 "; break;
                    case 2: result += " 🔵 "; break;
                    case 3: result += " 🟢 "; break;
                    case 4: result += " 🔴 "; break;
                    case 5: result += " 🟡 "; break;
                    case 6: result += " 🟣 "; break;
                    default:   result += " 🌈 "; break;
                }
            }
        }
        result += "|";
        result += "\n";
    }

    return result;
}

std::string GameEngine::board_to_retro_string() const {
    // レトロモード：ASCII アート風、8 ビット風
    std::string result = "   ";
    for (int x = 0; x < BOARD_SIZE; ++x) result += "-------";
    result += "\n";

    static const char* retro_black[]  = {".", "B", "█", "░", "▒", "▓", "█", "█"};
    static const char* retro_white[]  = {".", "o", "O", "0", "()", "⊙", "@", "*"};

    for (int y = 0; y < BOARD_SIZE; ++y) {
        result += std::to_string(y);
        result += "| ";
        for (int x = 0; x < BOARD_SIZE; ++x) {
            if (board_[y][x] == Color::EMPTY) {
                result += "   ";
            } else if (board_[y][x] == Color::BLACK) {
                int pattern = ((y + x) % 8);
                result += retro_black[pattern];
            } else if (board_[y][x] == Color::WHITE) {
                int pattern = ((y - x + BOARD_SIZE) % 8);
                result += retro_white[pattern];
            }
        }
        result += "|";
        result += "\n";
    }

    return result;
}

// ゲームオーバー判定＋勝敗表示
bool GameEngine::is_game_over() const {
    auto valid_moves = get_valid_moves(board_);

    if (valid_moves.empty()) {
        show_game_over();
        game_running_ = false;
        SoundEngine::play_sound(SoundId::SFX_GAME_OVER);
        return true;
    }

    return false;
}

// 碁石の配置数を返す（黒・白の数をペアで）
std::pair<int,int> GameEngine::count_pieces(const std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE>& board) const {
    int black_count = 0, white_count = 0;
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            if (board[y][x] == Color::BLACK) black_count++;
            else if (board[y][x] == Color::WHITE) white_count++;
        }
    }
    return std::make_pair(black_count, white_count);
}

// ゲーム進行ループ（デモ用）
void GameEngine::run_demo() {
    int turn = 0;
    while (game_running_) {
        if (turn % 2 == 0) {
            // 黒のターン：ランダム選択
            auto moves = get_valid_moves(board_);
            if (!moves.empty()) {
                int idx = static_cast<int>(std::rand() % moves.size());
                make_move(moves[idx].first, moves[idx].second);
            } else {
                break;
            }
        } else {
            // 白のターン：ランダム選択
            auto moves = get_valid_moves(board_);
            if (!moves.empty()) {
                int idx = static_cast<int>(std::rand() % moves.size());
                make_move(moves[idx].first, moves[idx].second);
            } else {
                break;
            }
        }

        turn++;
    }
}

} // namespace neon_osero
