# olcNES - NES Emulator for macOS

A fully functional NES (Nintendo Entertainment System) emulator adapted for macOS with complete audio support via OpenAL.

[中文版本 (Chinese Version)](#chinese-version)

## Features

- ✅ **Complete NES Hardware Emulation**
  - 6502 CPU emulation
  - 2C02 PPU (Picture Processing Unit)
  - 2A03 APU (Audio Processing Unit)
  - 2KB RAM
  - Controller support

- ✅ **Graphics Rendering**
  - Full PPU implementation
  - Sprite rendering
  - Background rendering
  - Color palette system

- ✅ **Real Audio Playback**
  - OpenAL integration
  - Real-time NES sound generation
  - 44.1 kHz sampling rate
  - All 4 audio channels (Pulse 1, Pulse 2, Triangle, Noise)

- ✅ **Mapper Support**
  - Mapper 000 (NROM)
  - Mapper 001 (MMC1)
  - Mapper 002 (UxROM)
  - Mapper 003 (CNROM)
  - Mapper 004 (MMC3)
  - Mapper 066 (GxROM)

- ✅ **Debug Features**
  - CPU status display
  - Audio waveform visualization
  - Palette preview
  - Pattern table viewer

## Screenshots

### Super Mario Bros Running

![Super Mario Bros gameplay with debug info](screenshots/mario-gameplay.png)

*The emulator running Super Mario Bros with real-time CPU status, audio waveforms, and pattern tables displayed*

### Debug Interface

*Debug panel showing CPU registers, status flags, palette, and pattern tables*

## Quick Start

### Prerequisites

- macOS 10.15+ (Catalina or later)
- Xcode Command Line Tools
- libpng (installed via Homebrew)

```bash
# Install dependencies
brew install libpng
```

### Building

```bash
# Clone or navigate to the repository
cd olcNES

# Build the emulator
make

# Should output: Build successful! Binary: ./nes_emulator
```

### Running

```bash
# Run with a ROM file
./nes_emulator "path/to/your/rom.nes"

# Example with Super Mario Bros
./nes_emulator "Super Mario Bros.nes"
```

## Controls

### Game Controls (Player 1)
| NES Button | Keyboard Key |
|------------|--------------|
| D-Pad Up    | ↑ (Up Arrow) |
| D-Pad Down  | ↓ (Down Arrow) |
| D-Pad Left  | ← (Left Arrow) |
| D-Pad Right | → (Right Arrow) |
| A Button    | X |
| B Button    | Z |
| SELECT      | A |
| START       | S |

### Emulator Controls
| Function | Key |
|----------|-----|
| Reset NES | R |
| Switch Palette | P |

## Audio System

The emulator uses **OpenAL** for real-time audio playback:
- Samples audio from the NES APU at 44.1 kHz
- Generates authentic NES sound effects and music
- Low latency (50-150ms)
- Automatic buffer management

You will hear:
- 🎵 Background music
- 🔊 Sound effects (jump, coin, enemy defeat, etc.)
- 🎼 All 4 NES audio channels

## Project Structure

```
olcNES/
├── Part #7 - Mappers & Basic Sounds/
│   ├── olcNes_Sounds1.cpp      # Main emulator (with OpenAL)
│   ├── Bus.h/cpp                # System bus
│   ├── olc6502.h/cpp            # CPU emulation
│   ├── olc2C02.h/cpp            # PPU emulation
│   ├── olc2A03.h/cpp            # APU emulation
│   ├── Cartridge.h/cpp          # ROM cartridge
│   └── Mapper_*.h/cpp           # Mapper implementations
├── Makefile                     # Build configuration
├── nes_emulator                 # Compiled binary
└── Documentation/
    ├── QUICKSTART.md
    ├── CONTROLS.md
    ├── OPENAL_COMPLETE.md
    └── README_macOS.md
```

## Technical Details

### Audio Implementation
- **Library**: OpenAL (macOS native framework)
- **Sample Rate**: 44100 Hz
- **Format**: 8-bit mono (NES native)
- **Buffer Management**: Dynamic queue (3-10 buffers)
- **Latency**: ~50-150ms

### Emulation Accuracy
- Cycle-accurate CPU emulation
- Scanline-based PPU rendering
- Frame-synchronized audio generation
- ~60 FPS gameplay

## Documentation

- **[QUICKSTART.md](QUICKSTART.md)** - Quick start guide
- **[CONTROLS.md](CONTROLS.md)** - Detailed control reference
- **[README_macOS.md](README_macOS.md)** - macOS-specific information
- **[OPENAL_COMPLETE.md](OPENAL_COMPLETE.md)** - Audio implementation details
- **[CLAUDE.md](CLAUDE.md)** - Code architecture for AI assistants

## Troubleshooting

### No Audio?
1. Check system volume
2. Verify output device in System Preferences → Sound
3. Look for "OpenAL audio initialized successfully" message

### Game Runs Too Fast/Slow?
- Frame rate should be ~60 FPS
- Audio syncs automatically

### ROM Won't Load?
- Ensure ROM is valid .nes (iNES) format
- Check file path is correct
- Verify mapper is supported (000-004, 066)

## Known Issues

- OpenAL is deprecated by Apple (but still works perfectly)
- Compilation shows deprecation warnings (safe to ignore)
- Some advanced mappers not supported

## Credits

- **Original Author**: javidx9 (OneLoneCoder)
- **Original Project**: [olcNES](https://github.com/OneLoneCoder/olcNES)
- **macOS Adaptation**: Audio system and build configuration
- **Reference**: FCMac project for OpenAL implementation

## License

OLC-3 License (see source files for details)

---

<a name="chinese-version"></a>

# olcNES - macOS NES 模拟器

完整功能的 NES（任天堂红白机）模拟器，已适配 macOS 并通过 OpenAL 实现完整音频支持。

## 功能特性

- ✅ **完整的 NES 硬件模拟**
  - 6502 CPU 模拟
  - 2C02 PPU（图形处理单元）
  - 2A03 APU（音频处理单元）
  - 2KB RAM
  - 手柄支持

- ✅ **图形渲染**
  - 完整的 PPU 实现
  - 精灵渲染
  - 背景渲染
  - 调色板系统

- ✅ **真实音频播放**
  - OpenAL 集成
  - 实时 NES 声音生成
  - 44.1 kHz 采样率
  - 所有 4 个音频通道（脉冲波1、脉冲波2、三角波、噪声）

- ✅ **Mapper 支持**
  - Mapper 000 (NROM)
  - Mapper 001 (MMC1)
  - Mapper 002 (UxROM)
  - Mapper 003 (CNROM)
  - Mapper 004 (MMC3)
  - Mapper 066 (GxROM)

- ✅ **调试功能**
  - CPU 状态显示
  - 音频波形可视化
  - 调色板预览
  - 图案表查看器

## 截图

### 超级马里奥运行中

![Super Mario Bros gameplay with debug info](screenshots/mario-gameplay.png)

*模拟器运行超级马里奥，显示实时 CPU 状态、音频波形和图案表*

### 调试界面

*调试面板显示 CPU 寄存器、状态标志、调色板和图案表*

## 快速开始

### 系统要求

- macOS 10.15+ (Catalina 或更高)
- Xcode 命令行工具
- libpng (通过 Homebrew 安装)

```bash
# 安装依赖
brew install libpng
```

### 编译

```bash
# 进入项目目录
cd olcNES

# 编译模拟器
make

# 应该输出: Build successful! Binary: ./nes_emulator
```

### 运行

```bash
# 使用 ROM 文件运行
./nes_emulator "ROM文件路径.nes"

# 示例：超级马里奥
./nes_emulator "Super Mario Bros.nes"
```

## 控制说明

### 游戏控制（玩家1）
| NES 按钮 | 键盘按键 |
|---------|---------|
| 方向键上 | ↑ (上方向键) |
| 方向键下 | ↓ (下方向键) |
| 方向键左 | ← (左方向键) |
| 方向键右 | → (右方向键) |
| A 按钮  | X |
| B 按钮  | Z |
| SELECT | A |
| START  | S |

### 模拟器控制
| 功能 | 按键 |
|-----|-----|
| 重置 NES | R |
| 切换调色板 | P |

## 音频系统

模拟器使用 **OpenAL** 进行实时音频播放：
- 以 44.1 kHz 从 NES APU 采样音频
- 生成真实的 NES 音效和音乐
- 低延迟（50-150ms）
- 自动缓冲区管理

你将听到：
- 🎵 背景音乐
- 🔊 音效（跳跃、金币、击败敌人等）
- 🎼 所有 4 个 NES 音频通道

## 项目结构

```
olcNES/
├── Part #7 - Mappers & Basic Sounds/
│   ├── olcNes_Sounds1.cpp      # 主模拟器（含 OpenAL）
│   ├── Bus.h/cpp                # 系统总线
│   ├── olc6502.h/cpp            # CPU 模拟
│   ├── olc2C02.h/cpp            # PPU 模拟
│   ├── olc2A03.h/cpp            # APU 模拟
│   ├── Cartridge.h/cpp          # ROM 卡带
│   └── Mapper_*.h/cpp           # Mapper 实现
├── Makefile                     # 构建配置
├── nes_emulator                 # 编译后的可执行文件
└── 文档/
    ├── QUICKSTART.md
    ├── CONTROLS.md
    ├── OPENAL_COMPLETE.md
    └── README_macOS.md
```

## 技术细节

### 音频实现
- **库**: OpenAL（macOS 原生框架）
- **采样率**: 44100 Hz
- **格式**: 8位单声道（NES 原生）
- **缓冲管理**: 动态队列（3-10 个缓冲区）
- **延迟**: 约 50-150ms

### 模拟精度
- 周期精确的 CPU 模拟
- 基于扫描线的 PPU 渲染
- 帧同步音频生成
- 约 60 FPS 游戏速度

## 文档

- **[QUICKSTART.md](QUICKSTART.md)** - 快速开始指南
- **[CONTROLS.md](CONTROLS.md)** - 详细控制参考
- **[README_macOS.md](README_macOS.md)** - macOS 特定信息
- **[OPENAL_COMPLETE.md](OPENAL_COMPLETE.md)** - 音频实现细节
- **[CLAUDE.md](CLAUDE.md)** - 代码架构（AI 助手参考）

## 故障排除

### 没有声音？
1. 检查系统音量
2. 在"系统偏好设置 → 声音"中验证输出设备
3. 查看是否有"OpenAL audio initialized successfully"消息

### 游戏运行太快/太慢？
- 帧率应该在 60 FPS 左右
- 音频会自动同步

### ROM 加载失败？
- 确保 ROM 是有效的 .nes (iNES) 格式
- 检查文件路径是否正确
- 验证 mapper 是否受支持（000-004, 066）

## 已知问题

- OpenAL 被 Apple 标记为弃用（但仍完美工作）
- 编译时显示弃用警告（可安全忽略）
- 部分高级 mapper 不支持

## 致谢

- **原作者**: javidx9 (OneLoneCoder)
- **原项目**: [olcNES](https://github.com/OneLoneCoder/olcNES)
- **macOS 适配**: 音频系统和构建配置
- **参考**: FCMac 项目的 OpenAL 实现

## 许可证

OLC-3 许可证（详见源文件）

---

**状态**: ✅ 生产就绪  
**测试**: ✅ 通过  
**音频**: ✅ 完整支持  
**平台**: macOS 10.15+
