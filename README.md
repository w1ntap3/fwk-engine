# FWK Engine
This is a Firework Engine written in raylib C library that implements a simulation of fireworks that can be controlled by the user and explode. The purpose of the project is to study graphics programming in C, basics of physics simulation and creating an interactive and genuinely satisfying simulation.

# Installation
> [!NOTE]
> The project has only been tested on a Linux desktop with raylib installed from pacman. You might have a harder or just different way to set up the engine on your system.
0. The project expects you to have [raylib](https://github.com/raysan5/raylib) library set-up on your system.
1. Go to `Makefile` and configure it according to your system.
2. Build and run it
```bash
make
```

# Controls
- `-`/`=` to zoom in/out respectively.
- `Left Mouse Click` to summon a firework. Hold the button to keep the firework following your mouse. The moment you release it it launches independently at a constant speed.
- `Q` to close the window
