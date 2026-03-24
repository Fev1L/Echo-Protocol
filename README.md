# Echo Protocol

**Echo Protocol** is a 2D post-apocalyptic horror game built in **C++ with SDL3**.

This project was created as a personal low-level game development project with a focus on understanding how game systems work without using a traditional game engine.

## Alpha 1 Release

**Current version:** `Alpha 1`  
**Status:** Playable early release
https://fev1l.itch.io/echo-protocol

Alpha 1 is the first public milestone of the project.  
The core gameplay loop is already implemented and playable: survive inside the bunker, monitor enemy movement, use the echo system, and place decoys strategically to stay alive through the night.

## Game Premise

The world has collapsed under the control of hostile AI systems.  
Most of humanity is gone, and the few survivors left are hiding underground.

You play as a survivor trapped inside an old Soviet bunker.

Your objective is to **survive for 7 nights until a rescue team arrives**.

The bunker is equipped with a monitoring system that periodically emits an **echo pulse**.  
This pulse scans the surrounding area and reveals enemy positions for a short time.

Using the information from the scan, you must react quickly, track enemy movement, and **deploy bait to distract threats away from the bunker**.

The game is built around tension, limited information, timing, and survival under pressure.

## Core Gameplay

- Survive night by night inside the bunker
- Track hostile entities through the monitoring system
- Use the **echo pulse** to reveal enemy positions
- Deploy **bait** to manipulate enemy movement
- Manage threats before they reach you
- Progress through multiple nights until extraction

## Features in Alpha 1

- 2D game built fully in **C++**
- Rendering and gameplay systems implemented with **SDL3**
- Custom game loop
- Scene-based project structure
- Real-time enemy tracking
- Echo scan mechanic
- Bait / decoy interaction system
- Basic enemy AI behavior
- Menu system
- Audio support
- Cross-platform builds for **Windows**, **macOS**, and **Linux**

## Technologies Used

- **C++17**
- **SDL3**
- **SDL3_image**
- **SDL3_ttf**
- **CMake**
- **GitHub Actions** for automated builds and packaging

## Why This Project Exists

Echo Protocol started as a personal learning project focused on low-level game development.

The goal was to learn how core game systems work in practice, including:

- game loop architecture
- rendering pipelines
- entity management
- scene handling
- input processing
- simple AI behavior
- asset loading
- cross-platform build setup

Instead of using a full game engine, I wanted to build the logic and structure manually to better understand what happens under the hood.

## Installation

### Windows
Download the Windows build from the project releases and run the installer or executable package.

### macOS
Download the `.dmg` file from the releases page, open it, and drag **EchoProtocol.app** into **Applications**.

### Linux !NOT TESTED!
Download the `.AppImage` build for your architecture.

## Current Development State

Alpha 1 is a playable early version, which means the game already contains the main idea and core systems, but it is still not final.

At this stage, the project should be treated as:
- feature-complete enough to demonstrate the core concept
-	actively evolving
-	open to balancing, polish, bug fixes, and visual/audio improvements

## Planned Improvements

Future updates may include:
-	improved enemy AI
-	better balancing and pacing
-	more polished UI
-	stronger visual atmosphere
-	expanded sound design
-	more survival mechanics
-	additional night variety
-	improved feedback for player actions

## Screenshots
<img width="2560" height="1440" alt="Знімок екрана 2026-03-22 о 18 55 59" src="https://github.com/user-attachments/assets/359e860c-8558-47c8-af91-ce7d71ac45cf" />
<img width="2560" height="1440" alt="Знімок екрана 2026-03-22 о 18 56 23" src="https://github.com/user-attachments/assets/a9e6a1c0-379a-452b-84a5-e1240dacd533" />
<img width="2560" height="1440" alt="Знімок екрана 2026-03-22 о 18 56 30" src="https://github.com/user-attachments/assets/5b8dbdc2-3ebb-4220-a9f9-0bd02158a503" />
<img width="2560" height="1440" alt="Знімок екрана 2026-03-22 о 18 56 27" src="https://github.com/user-attachments/assets/58204f6e-2803-47d1-9ee4-8128b5ba831b" />

## Autor
Developed by Fev1L
GitHub: github.com/Fev1L￼
