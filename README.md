##  Features

### Features
- [ ] Open and display PDF files
- [ ] Navigate between pages (Next / Previous / Jump to page)
- [ ] Zoom controls (Zoom in / out / Fit to window)
- [ ] Annotation tools (highlight, underline, notes)
- [ ] PDF export with JSON annotations
- [ ] Basic toolbar and status bar integration

---
### Prerequisites
- Qt 6 (with `PdfWidgets` module recommended; Qt 6.5+ preferred)
- CMake 3.21 or newer
- Ninja (optional, but recommended for fast parallel builds)
- A C++17-compatible compiler (e.g. GCC, Clang, MSVC)


<img width="1372" height="1007" alt="image" src="https://github.com/user-attachments/assets/41c2ab4d-acc9-4b98-9626-7df825650c92" />


### Want to Build? Follow these steps

```bash
# Navigate to project root (outside existing build/ directory)
cmake -S . -B build -G "Ninja" \\
      -DCMAKE_PREFIX_PATH="<path-to-Qt-6>/lib/cmake"
cmake --build build --config Debug

# Running the app (depending on the system/config):
./build/pdf_editor        # on Linux/macOS
build\\Debug\\pdf_editor.exe  # on Windows (Ninja + Debug)
