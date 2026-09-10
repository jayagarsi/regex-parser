# NFA/DFA Regex Engine — WASM Demo

This packages your regex engine (recursive-descent parser → Thompson's
construction NFA → simulation) for the browser via WebAssembly, so it can be
hosted as a static GitHub Pages site.

## What's in here

```
regex-wasm/
├── AutomataManager.cpp / .hh   your engine (unchanged)
├── Parser.cpp / .hh            your parser (unchanged)
├── wasm_bindings.cpp           thin extern "C" wrapper exposing matchRegex()
├── build.sh                    compiles everything to WASM via emcc
├── test_main.cpp               native test harness (already verified: 11/11 pass)
└── docs/
    └── index.html              the demo page (loads regex_engine.js/.wasm once built)
```

Note: `wasm_bindings.cpp` was compiled against your actual `Parser.hh` /
`AutomataManager.hh` headers and verified to compile cleanly (only the same
pre-existing warnings your own files already produce). The only thing not yet
done is the actual `emcc` → WASM step, which requires the Emscripten SDK —
that has to run on your machine, not in this sandbox (see below for why).

## 1. Install Emscripten (if you haven't already)

```bash
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh      # re-run this in every new terminal session
```

Verify: `emcc --version`

## 2. Build

From the `regex-wasm/` directory:

```bash
chmod +x build.sh
./build.sh
```

This runs:

```bash
emcc AutomataManager.cpp Parser.cpp wasm_bindings.cpp \
    -o docs/regex_engine.js \
    -s EXPORTED_FUNCTIONS='["_matchRegex","_getLastError"]' \
    -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s MODULARIZE=1 \
    -s EXPORT_NAME='RegexModule' \
    -O2
```

producing `docs/regex_engine.js` and `docs/regex_engine.wasm`.

## 3. Test locally

```bash
cd docs
python3 -m http.server 8000
```

Open `http://localhost:8000` — you should see the demo page with live
matching. (Opening `index.html` directly via `file://` will NOT work —
browsers block WASM loading from the filesystem for security reasons; it
must be served over http.)

## 4. Deploy to GitHub Pages

Commit the whole `regex-wasm/` directory (or just merge its contents) into
your repo, then:

1. Push to GitHub.
2. Go to **Settings → Pages**.
3. Under **Source**, choose **Deploy from a branch**.
4. Branch: `main` (or whichever), folder: **`/docs`**.
5. Save. After a minute or two, your demo will be live at
   `https://<your-username>.github.io/<repo-name>/`.

## API exposed to JavaScript

```js
RegexModule().then((Module) => {
    const matchRegex = Module.cwrap('matchRegex', 'number', ['string', 'string']);
    const getLastError = Module.cwrap('getLastError', 'string', []);

    const result = matchRegex('(a|b)*', 'aba');
    // result === 1  -> match
    // result === 0  -> no match
    // result === -1 -> parse/runtime error; call getLastError() for the message
});
```

## Troubleshooting

- **"Incorrect response MIME type" for .wasm**: GitHub Pages serves the
  correct `application/wasm` type by default, so this is unlikely there —
  but if testing on a different static host, check its MIME type config.
- **Blank page / console error about `RegexModule is not defined`**: make
  sure `regex_engine.js` actually built (check `docs/` for both the `.js`
  and `.wasm` files) and that `index.html`'s `<script src="regex_engine.js">`
  path matches where you placed it.
- **CORS errors when opening via `file://`**: expected — always serve over
  `http://` locally (see step 3) or via GitHub Pages itself.
