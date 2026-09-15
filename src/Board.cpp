#include "neon_osero.hpp"
#include <random>
#include <algorithm>

namespace neon_osero {

// 碁石の状態を判定するヘルパー関数
bool is_surrounded(const std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE>& board,
                    int x, int y, Color piece_color) {
    // 8 方向: 上下左右＋斜め 4 つ
    static const int dx[] = {-1,-1,-1,0,0,1,1,1};
    static const int dy[] = {-1,0,1,-1,1,-1,0,1};

    for (int i = 0; i < 8; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        // 盤面の外に出ないかチェック
        if (nx < 0 || nx >= BOARD_SIZE || ny < 0 || ny >= BOARD_SIZE) continue;

        Color neighbor_color = board[ny][nx];
        if (neighbor_color == piece_color) {
            // 同じ色の碁石に囲まれた → この方向で挟める
            return true;
        } else if (neighbor_color == Color::EMPTY) {
            // 空いている → この方向は囲めない
            break;
        }
    }

    return false;
}

// 指定した碁石が置けるかどうかを判定
bool can_place_piece(const std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE>& board,
                     int x, int y) {
    // まず空いているかチェック
    if (board[y][x] != Color::EMPTY) return false;

    const Color current_player = board[0][BOARD_SIZE / 2]; // 現在のプレイヤー（左上から判定）

    // 8 方向に囲い込み判定
    for (int i = 0; i < 8; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx < 0 || nx >= BOARD_SIZE || ny < 0 || ny >= BOARD_SIZE) continue;
        Color neighbor_color = board[ny][nx];

        // 同じ色ならこの方向で囲える → 置ける
        if (neighbor_color == current_player) {
            return true;
        } else if (neighbor_color != Color::EMPTY) {
            // 違う色が挟まっている → この方向は囲えない
            break;
        }
    }

    return false;
}

// 碁石を置く処理
void place_piece(std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE>& board, int x, int y) {
    const Color current_player = board[0][BOARD_SIZE / 2]; // 現在のプレイヤー
    board[y][x] = (current_player == Color::BLACK) ? Color::BLACK : Color::WHITE;

    // 置いた碁石が囲えた方向を特定し、挟める碁石を反転させる
    const int dx[] = {-1,-1,-1,0,0,1,1,1};
    const int dy[] = {-1,0,1,-1,1,-1,0,1};

    for (int i = 0; i < 8; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
            Color neighbor_color = board[ny][nx];
            // 挟める碁石を反転させる（置いた色とは逆の色）
            if (neighbor_color != current_player && neighbor_color != Color::EMPTY) {
                if (board[0][BOARD_SIZE/2] == current_player) {
                    board[ny][nx] = (current_player == Color::BLACK) ? Color::WHITE : Color::BLACK;
                } else {
                    // 囲われた方（相手の碁石）を反転させる
                    if (board[0][BOARD_SIZE/2] == Color::WHITE) {
                        board[ny][nx] = Color::BLACK;
                    } else {
                        board[ny][nx] = Color::WHITE;
                    }
                }
            }
        }
    }
}

// 現在のプレイヤーを取得（盤面が初期化済み）
Color get_current_player(const std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE>& board) {
    return board[0][BOARD_SIZE / 2]; // 左上の碁石の色で現在手番を判定（簡易版）
}

// 有効な手を返す
std::vector<std::pair<int,int>> get_valid_moves(const std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE>& board) {
    const Color current_player = board[0][BOARD_SIZE / 2]; // 現在のプレイヤー
    std::vector<std::pair<int,int>> valid_moves;

    static const int dx[] = {-1,-1,-1,0,0,1,1,1};
    static const int dy[] = {-1,0,1,-1,1,-1,0,1};

    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            if (board[y][x] == Color::EMPTY && can_place_piece(board, x, y)) {
                valid_moves.push_back({x, y});
            }
        }
    }

    return valid_moves;
}

// 碁石の配置数を返す（評価用）
int count_pieces(const std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE>& board) {
    int black_count = 0;
    int white_count = 0;
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            if (board[y][x] == Color::BLACK) black_count++;
            else if (board[y][x] == Color::WHITE) white_count++;
        }
    }
    return black_count + white_count; // 置かれた碁石の総数
}

// ゲーム終了判定（両者ともに手がない場合）
bool is_game_over(const std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE>& board) {
    auto valid_moves = get_valid_moves(board);
    return (valid_moves.empty()); // 有効な手がなければゲーム終了
}

// 現在のプレイヤーのターン数（評価用）
int get_turn_count(const std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE>& board) {
    int black_turns = 0;
    int white_turns = 0;
    auto valid_moves = get_valid_moves(board);
    
    // これまで置かれた碁石の数を返す（簡易的なターン数）
    return count_pieces(board);
}

// ランダムで有効な手を取得（AI 用）
std::pair<int,int> get_random_move(const std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE>& board) {
    auto moves = get_valid_moves(board);
    if (moves.empty()) return {-1, -1};

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, static_cast<int>(moves.size()) - 1);

    int idx = dis(gen);
    return moves[idx];
}

// ランダムで有効な手を取得（AI の難易度調整付き）
std::pair<int,int> get_random_move_with_difficulty(const std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE>& board, float difficulty) {
    auto moves = get_valid_moves(board);
    if (moves.empty()) return {-1, -1};

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    // 難易度が高いほど良い手を優先
    float rand_val = dis(gen) * difficulty;
    
    if (rand_val < 0.2f) {
        return {-1, -1}; // 手がない
    } else if (rand_val < 0.7f) {
        int idx = static_cast<int>(moves.size() * rand_val);
        return moves[idx];
    } else {
        int best_move_idx = 0;
        int max_score = -1;

        for (size_t i = 0; i < moves.size(); ++i) {
            // 簡易評価：空いている場所が多い方がよい（攻めやすい）
            int empty_count = 0;
            int other_count = 0;
            for (int y = 0; y < BOARD_SIZE; ++y) {
                for (int x = 0; x < BOARD_SIZE; ++x) {
                    if (board[y][x] == Color::EMPTY) empty_count++;
                    else if (board[y][x] != Color::WHITE && board[y][x] != Color::BLACK) other_count++;
                }
            }
            int score = empty_count - other_count;
            if (score > max_score) {
                max_score = score;
                best_move_idx = static_cast<int>(i);
            }
        }

        return moves[best_move_idx];
    }
}

// 盤面の初期化（初期状態）
std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE> create_initial_board() {
    std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE> board;
    
    // 初期配置：左上から (2,3) に黒、(2,4) に白、(3,3) に白、(3,4) に黒
    board[3][2] = Color::BLACK;   // y=3, x=2
    board[3][4] = Color::WHITE;   // y=3, x=4
    board[4][3] = Color::WHITE;   // y=4, x=3
    board[4][4] = Color::BLACK;   // y=4, x=4

    return board;
}

// 碁石の色を文字列に変換
std::string color_to_string(Color c) {
    switch (c) {
        case Color::EMPTY:      return "empty";
        case Color::BLACK:     return "black";
        case Color::WHITE:     return "white";
        default:               return "unknown";
    }
}

// 盤面を文字列で表示（デバッグ用）
std::string board_to_string(const std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE>& board) {
    std::string result;
    result += "   ";
    for (int x = 0; x < BOARD_SIZE; ++x) result += "-------";
    result += "\n";

    for (int y = 0; y < BOARD_SIZE; ++y) {
        result += std::to_string(y);
        result += "|";
        for (int x = 0; x < BOARD_SIZE; ++x) {
            switch (board[y][x]) {
                case Color::EMPTY:   result += " . "; break;
                case Color::BLACK:   result += " B "; break;
                case Color::WHITE:   result += " W "; break;
                default:             result += "?  "; break;
            }
        }
        result += "|";
        result += "\n";
    }

    return result;
}

} // namespace neon_osero
