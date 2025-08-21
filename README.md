##  Features

### Features
- [ ] Open and display PDF files
- [ ] Navigate between pages (Next / Previous / Jump to page)
- [ ] Zoom controls (Zoom in / out / Fit to window)
- [ ] Annotation tools (highlight, underline, notes)
- [ ] PDF export with embedded annotations
- [ ] Page operations: rotate, delete, reorder, extract
- [ ] Basic toolbar and status bar integration

---
### Prerequisites
- Qt 6 (with `PdfWidgets` module recommended; Qt 6.5+ preferred)
- CMake 3.21 or newer
- Ninja (optional, but recommended for fast parallel builds)
- A C++17-compatible compiler (e.g. GCC, Clang, MSVC)

### Want to Build? Follow these steps

```bash
# Navigate to project root (outside existing build/ directory)
cmake -S . -B build -G "Ninja" \\
      -DCMAKE_PREFIX_PATH="<path-to-Qt-6>/lib/cmake"
cmake --build build --config Debug

# Running the app (depending on your system/config):
./build/pdf_editor        # on Linux/macOS
build\\Debug\\pdf_editor.exe  # on Windows (Ninja + Debug)
