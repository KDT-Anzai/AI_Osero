# 🌈 ネオン・オセロ (Neon Osero)

> 暗闇に浮かぶネオンの碁石が輝く、派手なオセロゲームです！🎮✨

## ゲームの特徴

- **3 つのカラーモード**：ネオン（デフォルト）／レインボウ／レトロ
- **シンメトリックプレイモード**：黒と白が常に対称的な碁配置で戦う
- **AI 相手モード**：難易度調整付きの AI が白を動かします
- **C++17 + CMake** で構成されたモダンなプロジェクト

## システム要件

- Windows 10 / 11（または Linux、macOS）
- C++17 対応のコンパイラ（MSVC / Clang / GCC 7+ など）
- [DX ライブラリ] - OpenGL / GLFW を使用した GPU アクセラレーション

## ビルド方法

### Windows (Visual Studio)

```bash
cmake .. -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

### Linux / macOS

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
```

### Visual Studio プロジェクトファイル生成

```bash
cmake .. -G "Visual Studio 17 2022"
```

VS でビルドすると、`build/NeonOsero.exe` が生成されます。

## 実行方法

```bash
# 通常プレイ（人間同士）
./build/Release/NeonOsero.exe

# AI と遊ぶ（白が AI）
./build/Release/NeonOsero.exe --ai

# 難易度高めな AI
./build/Release/NeonOsero.exe --ai-hard

# シンメトリックプレイモード
./build/Release/NeonOsero.exe --cheat
```

## クイックスタート

1. `CMakeLists.txt` を確認して依存ライブラリのパスを調整
2. `cmake .. -G "Visual Studio 17 2022"` でプロジェクト生成
3. ビルド＆実行！

## ゲームプレイの仕組み

- **黒が先手**で開始（通常ルール）
- 碁石を置くと、挟める方向にある相手の碁石が反転する
- リミットゲージ満タン時は「閃光弾」で相手の碁を一斉に反転できる！
- ゲーム終了：両者ともに手がない状態

## 開発環境

```yaml
CMake:    ≥3.20
GLFW:     ≥3.3
OpenGL:   ≥4.5 (Vulkan also supported)
C++17:    MSVC / Clang / GCC 7+
```

## ライセンス

MIT License — 自由に使って、改変して、公開してください！🎉

---

*Neon Osero © 2026 | Created with ❤️ by the Herme s team.*
