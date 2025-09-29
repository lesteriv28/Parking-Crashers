# 🚗 SFU Parking Crashers Game

A **2D FPGA-based driving game** with a **one-point perspective depth effect**, developed as part of **ENSC 452 (Design Project)** at Simon Fraser University.  

The project leverages the **ZedBoard FPGA** with **Xilinx Vivado/SDK tools**, enabling:
- Smooth **VGA graphics rendering** at 640×480 resolution  
- Hardware-based **random number generation** for obstacles  
- Support for **audio playback** (under development)  
- Dual-core ARM Cortex-A9 coordination (graphics + audio)  

---

## 📖 Table of Contents
1. [Introduction](#-introduction)  
2. [Background](#️-background)  
3. [System Overview](#️-system-overview)  
4. [User Manual](#️-user-manual)  
5. [IP Used](#-ip-used)  
6. [Results](#-results)  
7. [Limitations](#️-limitations)  
8. [Future Work](#-future-work)  
9. [Design Tree](#-design-tree)  
10. [Software Hierarchy](#-software-hierarchy)  
11. [Setup & Build Instructions](#️-setup--build-instructions)  
12. [Authors](#️-authors)   

---

## 📖 Introduction
The project’s aim was to design an **immersive 2D racing game** that simulates depth using **one-point perspective graphics**.  
Key goals:
- Implement the one-point perspective depth effect  
- Develop gameplay with **obstacles, acceleration, and collision detection**  
- Optimize FPGA resources for smooth gameplay and audio handling  

---

## 🖼️ Background
- **One-point perspective** is a technique where objects recede towards a vanishing point, simulating depth in a flat 2D plane. This was used to draw roads, obstacles, and gameplay elements.  
- **Sprites** were stored as **32-bit RGBA unsigned integer arrays** (auto-generated via a Python PIL script converting PNG → C headers).  
- **Audio**: Stereo WAV files were converted into hexadecimal arrays for direct embedding into C code, allowing playback without external file systems.  

---

## 🛠️ System Overview
The ZedBoard FPGA was used with a hardware/software co-design approach:

- **Dual ARM Cortex-A9 cores**  
  - Core 1: Handles drawing, game logic, collision detection  
  - Core 2: Dedicated to audio playback  
- **AXI SmartConnect**: Synchronizes modules, manages resets and communication  
- **DDR memory**: Shared between both cores  
- **UART**: Provides real-time debug messages to terminal  

### 🔧 Block Diagram
```
        ┌───────────────────────────────┐
        │          ZedBoard FPGA        │
        │                               │
        │  ┌───────────────┐            │
        │  │  Cortex-A9 #1 │── Graphics │
        │  │  (Game Logic) │            │
        │  └───────────────┘            │
        │                               │
        │  ┌───────────────┐            │
        │  │  Cortex-A9 #2 │── Audio    │
        │  │   (Playback)  │            │
        │  └───────────────┘            │
        │                               │
        │  VGA  ─── Road/Obstacles      │
        │  UART ─── Debugging           │
        │  DDR  ─── Shared Memory       │
        └───────────────────────────────┘
```

---

## 🕹️ User Manual
### Controls (ZedBoard push buttons)
- **Start Game** → Any button  
- **Pause Menu** → Center (BTNC)  
  - Resume: Up (BTNU)  
  - Quit to Game Over: Down (BTND)  
- **Game Over Menu** → Any button returns to Main Menu  

### Game Flow
1. **Main Menu → Gameplay Loop**  
2. Exit Gameplay when:  
   - Collision with obstacle → Game Over  
   - BTNC pressed → Pause Menu  
3. From Pause:  
   - BTNU resumes game  
   - BTND ends game → Game Over  

---

## 📦 IP Used
| IP Block | Source | Modified |
|----------|--------|----------|
| Linear Feedback Shift Register RNG | Maxim Integrated | ✔ |
| VGA Controller | Zavier Aguila | ✘ |
| Raccoon Sprite | Shutterstock (derGriza) | ✔ |
| Car Sprite | DeviantArt (MtnDogeDew) | ✔ |
| Game Over Car Sprite | Alamy (Saphatthachat Sunchoote) | ✔ |
| Animated Coin Sprite | OpenGameArt (irmirx) | ✔ |
| Zed_audio_ctrl | Xilinx | ✘ |

---

## ✅ Results
- Achieved **immersive gameplay** with realistic depth rendering.  
- Obstacles generated dynamically with RNG logic.  
- Optimized VGA pipeline ensured **smooth frame rendering**.  
- Game was **challenging and replayable**.  

---

## ⚠️ Limitations
- VGA resolution reduced to **640×480** (initial design was 1280×1024).  
- Car movement tied to fixed pixel shift (cannot exceed default).  
- Maximum **4 obstacles** on screen at a time.  
- **Audio playback incomplete** — limited due to memory handling.  

---

## 🔮 Future Work
- Improve **memory management** for simultaneous audio + rendering.  
- Expand **game mechanics**: new obstacles, power-ups, scoring.  
- Optimize sprite conversion scripts (faster + multi-format).  
- Explore **higher VGA resolution** if FPGA performance allows.  

---

## 📂 Design Tree
```
project/
├── src/                # C source files
│   ├── main.c          # Core game loop
│   ├── graphics.c      # Rendering logic
│   ├── audio.c         # Audio playback logic
│   ├── rng.c           # Random number generator
│   └── assets/         # Sprites (32-bit RGBA arrays)
│       ├── car.h
│       ├── raccoon.h
│       ├── coin.h
│       └── ...
├── include/            # Header files
│   ├── sprite_ctrl.h   # Imports all assets
│   ├── game.h
│   └── vga.h
├── ip/                 # Custom & third-party IP blocks
├── docs/               # Report and documentation
│   └── images/         # Screenshots (see below)
└── README.md           # This file
```

---

## 📐 Software Hierarchy
```
main.c
 ├── game.c
 │    ├── sprite_ctrl.h (imports assets)
 │    └── rng.c
 ├── graphics.c (VGA drawing)
 └── audio.c (WAV playback via arrays)
```

---

## ⚙️ Setup & Build Instructions
### Requirements
- ZedBoard FPGA  
- Xilinx Vivado + SDK (2019.1 or compatible)  
- UART terminal software (e.g., PuTTY, minicom)  
- VGA Monitor + Audio Out  

### Build & Run
1. Clone repo and import project into **Vivado**.  
2. Generate bitstream and program FPGA.  
3. Open SDK, compile software sources (`src/`).  
4. Load compiled ELF onto Cortex-A processors.  
5. Connect VGA & audio cables, open UART for debug logs.  
6. Reset ZedBoard → Game starts at **Main Menu**.  

---

## 👨‍💻 Authors
- **Lester Pino**   
- **Richard Wang** 

ENSC 452 Design Project – April 2024  
Simon Fraser University  
