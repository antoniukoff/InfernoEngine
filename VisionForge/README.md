# Vision Forge

<p align="left">
  <img src="/VisionForge/assets/branding/VisionForge.gif" alt="Vision Forge" width="60%">
</p>


## Introduction
A sophisticated level editor designed for creating prototyping and designing levels in 2D.

## Motivation
The motivation behind Vision Forge was to make a user-friendly playground for 2D game building. I integrated design tools with a straightforward GUI to make the level design process as seamless as possible, allowing me to focus on fast prototyping.

## Installation
To get started with Vision Forge, follow these steps:

1. **Clone the Inferno Engine** to your computer or download the ZIP file from its repository.
    - If you're cloning, use the following command:
        ```
        git clone https://github.com/antoniukoff/InfernoEngine
        ```

2. **Set Vision Forge as the Startup Project** in Visual Studio.

3. **Run Vision Forge** in either Debug or Release mode.

4. **Access the Level Editor**: Once Vision Forge is running, click on the "Level Editor Button" to enter the level editor. Here, you can start playing around with the features within the editor.

5. **Testing (Optional)**: If you want to test the levels, go into the sandbox mode by clicking on the "Test" button within the editor.


## Features

### GUI-Based Design Tools
- Debug drawing, light blending modes, and manipulation of objects like placement, scaling, rotation, and translation.
- Uses Inferno Engine's GUI system for an improved design interaction.

### Level Loading and Saving System
- Supports both text and binary serialization for fast level data saving and loading.
- Simple 'Save' button functionality with options for new file creation or overriding existing files.

### Box2D Physics Engine
- Dynamic and static physics integration to simulate things like gravity and player-object interactions.
- Collision shapes (Box, Capsule, Circle) for entities and collision detection and response system.

## Usage

### Controls

- **Rotation**:
  - Press `E` to rotate the selected object to the right.
  - Press `Q` to rotate the selected object to the left.

- **Resizing Objects**:
  - Use the **Right Arrow Key** to increase the width of the selected object.
  - Use the **Left Arrow Key** to decrease the width of the selected object.
  - Use the **Up Arrow Key** to increase the height of the selected object.
  - Use the **Down Arrow Key** to decrease the height of the selected object.

<p align="left">
  <img src="/VisionForge/assets/branding/transform.gif" alt="Vision Forge" width="50%">
</p>

### Editor Modes

- **Select Mode**: Select existing objects in the scene and change their properties.

- **Place Mode**: You can add new objects to the scene; choose the type of object you want and click on the scene to position it.

- **Debug Mode**: Visualizes the collision boxes around objects.

<p align="left">
  <img src="/VisionForge/assets/branding/modes.gif" alt="Vision Forge" width="50%">
</p>

### Adjusting Object Properties

In the Details Panel, you can customize the selected object's properties:

- **Size**:
  - For spheres, you can adjust the radius.
  - For boxes, you can set the width and height.

- **Physics Parameters**:
  - **Static**: The object is unaffected by physics.
  - **Dynamic**: The object is subject to physics, including gravity and collisions.

