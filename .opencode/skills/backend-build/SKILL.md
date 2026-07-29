---
name: backend-build
description: CMake preset build commands for the C++ backend
---

## Build from `backend/`

```bash
# Configure
cmake --preset x64-debug       # Windows MSVC + Ninja
cmake --preset linux-debug     # Linux GCC/Clang + Ninja

# Build
cmake --build --preset x64-debug
```

## Notes
- vcpkg manifest mode — deps fetched automatically
- Binary lands in `out/build/<preset>/`
- No linter, formatter, typecheck, or test framework
- `.env` is copied post-build; ensure it exists at backend root
