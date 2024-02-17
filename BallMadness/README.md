# Ball Madness

<p align="left">
  <img src="/BallMadness/assets/branding/BallMadness.gif" alt="InfernoEngine Logo" width="65%"/>
</p>

## Details
"Ball Madness" is a ball simulation developed using the Inferno Engine that renders up to 40,000 spheres maintaining a frame rate of 55-60 fps.

## Motivation
The idea behind "Ball Madness" was to create a game that could render a large number of balls using spatial partitioning to optimize the ball collision algorithm and minimize the CPU overhead.

## Installation
To get started with Vision Forge, follow these steps:

1. **Clone the Inferno Engine** to your computer or download the ZIP file from its repository.
    - If you're cloning, use the following command:
        ```
        git clone https://github.com/antoniukoff/InfernoEngine
        ```

2. **Set Ball Madness as the Startup Project** in Visual Studio.

3. **Run Ball Madness** in Release mode (Debug mode is quite slow).

4. **Toggle shaders (Optional)**: If you want to see different VFX levels, press `1` on the keyboard you should switch between different shading algorithms.

## Features
- **Physics-Based Simulation:** Utilizes deflection physics to simulate ball collision based on the mass of the ball.
- **High Performance:** Supports up to 40,000 balls with minimal performance impact.
- **Grid Spatial Partitioning:** Implements adjacent cell collision checking, significantly reducing CPU load and maintaining high fps.
- **Dynamic Rendering:** Five shading algorithms, each having a unique visual representation.
