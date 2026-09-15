@echo off
REM ──── 🌈 ネオン・オセロ ビルドスクリプト ───────┐
REM           Visual Studio プロジェクト生成          │
REM ───────────────────────────────────────────────────┘

setlocal EnableDelayedExpansion

REM 色付き出力用の ANSI コード（Windows 10+ で有効）
echo [🌈] ネオン・オセロ ビルドスクリプトが起動しました！
echo      現在ディレクトリ: %CD%

REM フォルダパスの取得
set "PROJECT_DIR=%~dp0"
set "BUILD_DIR=%PROJECT_DIR%\build"
set "INCLUDE_DIR=%PROJECT_DIR%\include"
set "SRC_DIR=%PROJECT_DIR%\src"

REM 既存のビルドディレクトリを削除（クリーン）
if exist "%BUILD_DIR%" (
    echo [🧹] ビルドディレクトリをクリーンアップ中...
    rmdir /s /q "%BUILD_DIR%"
)

REM ビルドディレクトリの作成
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

REM CMake のパス変数を設定
echo.
echo ──────────────────────────────────────────────────
echo [🔧] CMake 初期化中...
echo ──────────────────────────────────────────────────

set "CMAKE_ARGS=-DGLFW_VERSION=3"
if "%1=="Visual Studio 17 2022" (
    set "GENERATOR=Visual Studio 17 2022"
) else if "%1=="Visual Studio 16 2019" (
    set "GENERATOR=Visual Studio 15 2017"
) else (
    set "GENERATOR=Ninja Multi-Config"
)

set "CMAKE_ARGS=%CMAKE_ARGS% -G"%GENERATOR%" -DCMAKE_BUILD_TYPE=Release %~1"

echo [✅] CMake を実行中...
cd /d "%PROJECT_DIR%"
cmake .. %CMAKE_ARGS% 2>&1 || (
    echo.
    echo [❌] ビルドに失敗しました。
    echo     - Visual Studio のインストールが必要です。
    echo     - または、CMake を手動で実行してください：
    echo       cmake .. -G"%GENERATOR%" -DCMAKE_BUILD_TYPE=Release
    goto :EOF
)

if %ERRORLEVEL% neq 0 (
    echo [❌] CMake が失敗しました。
    goto :EOF
)

REM ビルド開始
echo.
echo ──────────────────────────────────────────────────
echo [🔨] ビルド中... (%GENERATOR%)
echo ──────────────────────────────────────────────────
cmake --build "%BUILD_DIR%" --config Release /m /v:minimal || (
    echo.
    echo [❌] ビルドが失敗しました。エラーログ：
    dir "%BUILD_DIR%\*.log" 2>nul | findstr /N "."
    goto :EOF
)

REM 実行ファイルの存在確認
set "EXE_PATH=%BUILD_DIR%\Release\NeonOsero.exe"
if not exist "%EXE_PATH%" (
    echo [❌] 実行ファイルが見つかりません："%EXE_PATH%"
    goto :EOF
)

echo.
echo ──────────────────────────────────────────────────
echo [🎉] ビルド完了！実行ファイルの場所：
echo       %EXE_PATH%
echo ──────────────────────────────────────────────────
echo.
echo 🚀 すぐに試したいですか？
echo    type "%EXE_PATH%" ^&^& pause
goto :EOF

REM メイン処理
cd /d "%PROJECT_DIR%\build"
cmake .. -G"%GENERATOR%" -DCMAKE_BUILD_TYPE=Release %*
if not "%ERRORLEVEL%"=="0" (
    echo CMake failed. Check the error output above.
    exit /b 1
)

set "BUILD_CMD=cmake --build . --config Release /m /v:minimal"
%BUILD_CMD%
exit /b %ERRORLEVEL%
