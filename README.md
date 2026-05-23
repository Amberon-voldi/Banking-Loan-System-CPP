
# Banking Loan System (C++)

Simple, minimal bank loan system written in C++ with a custom, lightweight GUI. The project is intentionally small and avoids external GUI libraries — the GUI is implemented with a tiny custom event loop and basic drawing primitives. Keep it straightforward: one executable, minimal files, and just the necessary separation of concerns.

**Features**
- **Core:** Basic customer and loan management (create, view, calculate payments).
- **Loan calc:** Simple amortization and monthly payment calculator.
- **GUI:** Custom minimal GUI for input forms and simple displays (no external frameworks).

**Design Notes**
- **Keep it small:** Only the necessary files and modules are used.
- **Single executable:** Build all sources into one binary; no plugins or dynamic modules.
- **Custom GUI:** Implemented in `src/gui.*` with a simple event loop and drawing functions.

**Requirements**
- **Compiler:** Clang or GCC supporting C++17.
- **OS:** Cross-platform C++ should work, but GUI uses basic OS primitives — test on your platform.

**Build**
If the project is a single source file in `src/` (or simple set), compile with:

```bash
clang++ -std=c++17 -O2 src/*.cpp -o bankloan
```

Or compile a single main file:

```bash
clang++ -std=c++17 -O2 src/main.cpp -o bankloan
```

In VS Code you can use the provided task `C/C++: clang build active file` to build the active source file.

**SDL2 GUI build (macOS / Linux)**
Install dependencies (Homebrew on macOS):

```bash
brew install sdl2 sdl2_ttf
```

Then build:

```bash
clang++ -std=c++17 -O2 src/main.cpp -o bankloan_gui -lSDL2 -lSDL2_ttf
```

If `clang++` can't find libraries, use `sdl2-config --cflags --libs` and `pkg-config --cflags --libs SDL2_ttf` to add flags.

**Run**

```bash
./bankloan
```

**Usage (GUI)**
- Launch the program and use the simple form to enter customer details and loan parameters.
- Use the "Calculate" action to get monthly payment and amortization summary.
- The GUI is intentionally minimal: no complex widgets, just input fields and simple lists/tables.

**Minimal File Layout**
- `src/main.cpp`: Program entry, init, main loop.
- `src/bank.cpp`, `src/bank.h`: Core loan/customer logic and calculations.
- `src/gui.cpp`, `src/gui.h`: Very small custom GUI (event loop, input fields, rendering).
- `README.md`: This file.

**Notes & Tips**
- Keep the GUI code simple — focus on clarity over features.
- Avoid heavy third-party libraries; if later needed, add them deliberately.

**Contributing**
- Keep changes minimal and focused. Prefer small, reviewable commits.

**License**
- MIT (or choose your preferred license).
