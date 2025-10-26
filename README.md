# KFTLogger Features ☠️

A single-header, single-object, cross-platform C logging library designed to outclass anything from Microsoft or anyone else.  
Fully open source. Fully chaotic. Fully yours.

## ✅ Core Features
- [x] Simple mode with 4 log levels: `LOG`, `WARN`, `SUCCESS`, and `ERROR`
- [x] Selectable file descriptor (prints to any `FILE*`)
- [x] Automatic TTY detection (colored output for terminals, no color for non-TTY)
- [x] Works directly with `stderr`, `stdout`, custom files, or multiple files
- [x] Auto fallback to `stderr` if no logger is initialized
- [ ] Supports Windows, macOS, Linux, and BSD
- [x] Single header + object file implementation

## 🧩 Advanced Logging
- [ ] Multi-file logger (unlimited log targets)
- [ ] Configurable locking (lock/unlock before writing)
- [ ] Configurable threading model (main thread or dedicated logging thread)
- [ ] Optional buffering for efficiency
- [ ] Advanced routing (e.g. errors in one file, info logs in another)
- [ ] Customizable log format: choose which details to include (time, date, file, line, function)
- [ ] Bypass buffering: instant write mode
- [ ] Parallel logging mode — if two logs called simultaneously, new thread for each

## 🧠 Customization
- [ ] Add custom log levels (e.g. `addLevel("DEBUG")`)
- [ ] Add custom modes (e.g. `addMode("QUIET")`)
- [ ] Create custom loggers and define how they behave
- [ ] Custom log functions (user-defined behavior for log calls)
- [x] Configurable log modes:
  - [x] `ON` — log everything
  - [x] `OFF` — only errors log
  - [x] `FORCE_OFF` — nothing logs (useful for silencing output completely)
  - [ ] Custom user-defined modes

## 💾 File & Size Management
- [ ] Set max file size, max lines, or max characters
- [ ] Combine limits (e.g. both file size and line count)
- [ ] Automatic rollover or truncation (optional)

## ⚡ Performance Features
- [ ] Thread-safe with optional lock-free mode
- [ ] Asynchronous logging support
- [ ] Batch logging (`plog()` … `plogend()`)
- [ ] Optimized for both interactive (TTY) and non-interactive environments
- [x] Lightweight fallback system if file descriptors are unavailable

## 🧰 Utility
- [x] Direct logging API: `logf("format", ...)` works instantly with no setup
- [ ] Easy multi-target setup
