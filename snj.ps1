
function setup {
    echo ""
    echo "========== Starting Setup ============================================"
    if (-not (Test-Path ./deps -PathType Container)) {
        New-Item -ItemType Directory -Path ./deps | Out-Null
        echo "> created deps/ directory"
    } else {
        echo "> deps/ directory already exists"
    }

    if (-not (Test-Path ./bin -PathType Container)) {
        New-Item -ItemType Directory -Path ./bin | Out-Null
        echo "> created bin/ directory"
    } else {
        echo "> bin/ directory already exists"
    }

    echo ""
    echo "========== Setting up emscripten toolchain ==========================="
    if (Get-Command emsdk -ErrorAction SilentlyContinue) {
        "> emsdk already installed"
    } else {
        git clone https://github.com/emscripten-core/emsdk.git C:\emsdk
        pushd C:\emsdk\
        # emsdk update ; git pull
        ./emsdk install latest
        ./emsdk activate latest --permanent
        popd
    }

    echo ""
    echo "========== Setting up Raylib for Web ================================="
    if (-not (Test-Path ./deps/libraylib.a -PathType Leaf)) {
        git clone --branch 5.5 --depth 1 https://github.com/raysan5/raylib.git C:\raylib-web
        pushd C:\raylib-web\src

        emcc -c rcore.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
        emcc -c rshapes.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
        emcc -c rtextures.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
        emcc -c rtext.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
        emcc -c rmodels.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
        emcc -c utils.c -Os -Wall -DPLATFORM_WEB
        emcc -c raudio.c -Os -Wall -DPLATFORM_WEB
        emar rcs libraylib.a rcore.o rshapes.o rtextures.o rtext.o rmodels.o utils.o raudio.o

        popd
        cp C:\raylib-web\src\libraylib.a .\deps\libraylib.a
        cp C:\raylib-web\src\raylib.h .\deps\raylib.h
        cp C:\raylib-web\src\rlgl.h .\deps\rlgl.h
    } else {
        "> raylib dependancies already in place"
    }

    echo ""
    echo "========== Setting up Raygui ========================================="
    if (-not (Test-Path ./deps/raygui.h -PathType Leaf)) {
        Invoke-WebRequest `
          -Uri "https://raw.githubusercontent.com/raysan5/raygui/25c8c65a6e5f0f4d4b564a0343861898c6f2778b/src/raygui.h" `
          -OutFile "deps/raygui.h"
    } else {
        "> raygui in place"
    }

    echo ""
    echo "========== Setting up Raymath ========================================="
    if (-not (Test-Path ./deps/raymath.h -PathType Leaf)) {
        Invoke-WebRequest `
          -Uri "https://raw.githubusercontent.com/raysan5/raylib/c1ab645ca298a2801097931d1079b10ff7eb9df8/src/raymath.h" `
          -OutFile "deps/raymath.h"
    } else {
        "> raymath in place"
    }

    echo ""
    echo "---------- Finished Setup --------------------------------------------"
    echo ""
}

function compile {
    # -L isn't required right now but did point at C:/raylib-web/src
    # was> -L"C:/raylib-web/src"
    
    emcc -o bin/game.html game.c `
      -Os `
      -Wall deps/libraylib.a `
      -I"src/" `
      -I"deps/" `
      -L"deps/" `
      -s USE_GLFW=3 `
      --shell-file C:\raylib-web\src\shell.html `
      -DPLATFORM_WEB `
      -sASSERTIONS=1 `
      -s ASYNCIFY=1 `
      -s EXPORTED_RUNTIME_METHODS=ccall,cwrap
}

function run {
    echo "Serving game. Can recompile and refresh page to view changes."
    emrun bin/game.html
}

switch ($args[0]) {
    "setup"   { setup }
    "compile" { compile }
    "run"     { run }
    default   { Write-Host "Usage: snj.ps1 [setup|compile|run]" }
}
