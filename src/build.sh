#!/usr/bin/env bash
# Compiles the regex engine to WebAssembly and places the output in docs/,
# ready to be served by GitHub Pages.
#
# Prerequisites: emsdk installed and activated (source emsdk_env.sh first).

# export PATH="$HOME/.pyenv/bin:$PATH"
# eval "$(pyenv init -)"
# source ~/.bashrc
# pyenv local 3.11.9 
# ../../emdsk activate latest
# source ../../emsdk_env.sh
# chmod +x build.sh
# ./build.sh

set -e

em++ AutomataManager.cpp Parser.cpp wasm_bindings.cpp \
    -o ../webass/regex_engine.js \
    -s EXPORTED_FUNCTIONS='["_matchRegex","_getASTDiagram","_getNFADiagram"]' \
    -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s MODULARIZE=1 \
    -s EXPORT_NAME='RegexModule' \
    -O2

echo "Build complete. Output written to docs/regex_engine.js and docs/regex_engine.wasm"
echo "Test locally with:  cd webass && python3 -m http.server 8000"
echo "Then open http://localhost:8000"
