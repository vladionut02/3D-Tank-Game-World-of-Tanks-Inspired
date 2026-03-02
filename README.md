# 3D-Tank-Game-World-of-Tanks-Inspired

This project is a single-player 3D tank game inspired by *World of Tanks*, developed using the OpenGL-based GFX Framework. 
To run it, integrate the source files into the official GFX Framework project.

It was implemented as part of the Computer Graphics course at Politehnica University of Bucharest.

The game features a controllable tank in a third-person perspective, projectile shooting mechanics, randomly placed buildings, and camera interaction.

---

## Game Overview

The player controls a tank placed on a flat terrain populated with buildings.

The tank supports:

- Forward and backward movement
- Left and right rotation (tank-style controls)
- Independent turret rotation using mouse movement
- Third-person camera rotation (right mouse button)
- Projectile shooting

Buildings are randomly generated on the map and rendered as axis-aligned boxes.

---

## Implemented Features

### Tank Construction
The tank is built from separate components:
- Body
- Turret
- Cannon
- Tracks

Each component is rendered independently, allowing proper rotation of the turret and cannon relative to the body.

### Third-Person Camera
The camera:
- Is attached behind the tank
- Follows tank movement
- Rotates around the tank when right-click is pressed

### Tank Controls (WASD)
- **W** – Move forward
- **S** – Move backward
- **A** – Rotate left
- **D** – Rotate right

Movement is relative to the tank's orientation (classic “tank controls”).

### Turret Rotation
- Mouse movement rotates the turret independently of the body.
- The camera does not rotate when the turret rotates.

### Shooting Mechanic
- Left mouse click fires a projectile.
- Projectiles spawn at the tip of the cannon.
- Projectiles move in a straight line in the direction of the turret.

### Environment
- Flat ground plane.
- Randomly generated buildings with random sizes.
- Buildings are placed at a minimum distance from the player spawn point.

---

## Technical Details

- Language: C++
- Graphics API: OpenGL
- Framework: GFX Framework
- Custom third-person camera implementation
- MVP (Model–View–Projection) transformation pipeline
- Real-time input handling
- Object-oriented scene structure
- Shader-based rendering (Vertex + Fragment shaders)

---

## Not Fully Implemented

The following features are not fully implemented and will be implemented in the future:

- Enemy tanks with AI behavior
- Collision detection (projectile–tank, tank–tank, tank–building)
- HP system and damage-based deformation
- Color modification based on damage
- Projectile lifetime / cleanup
- Shooting cooldown
- Game timer and end-game logic
- Score tracking

---

## Build & Run

The project uses CMake.

Windows:
- Generate the project using CMake
- Open `build/GFXFramework.sln`
- Build and run
- The Tema2 scene launches by default

Linux / macOS:

    mkdir -p build
    cd build
    cmake ..
    cmake --build . -j
    ./GFXFramework

---

## Author

Vlad Varzaru
