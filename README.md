# Cub3D

![alt text](/Textures/cub.png)

Cub3D is a 42 project that involves creating a 3D graphical application inspired by the game Wolfenstein 3D. This project demonstrates a basic understanding of raycasting techniques, handling textures, and creating a real-time rendering engine.

---

## Features

- **Raycasting**: Implements a raycasting algorithm to simulate a 3D perspective.
- **Textured Walls**: Displays walls with textures for a realistic effect.
- **Player Movement**: Allows the player to move around and look in different directions.
- **Minimap**: Optional feature to display a 2D representation of the map.
- **Dynamic Lighting**: Basic lighting effects for improved realism.
- **Error Handling**: Validates input files and handles edge cases gracefully.

---

## Installation

1. Clone the repository:
   ```bash
   git clone git@github.com:Mohamedsarda/cub3d.git
   cd cub3d
   ```
2. Compile the program:
   ```bash
   make
   ```
3. Run the application:
   ```bash
   ./cub3D path/to/map.cub
   ```

---

## Usage

- Launch the program by providing a valid `.cub` file containing the map configuration.
- Use the following controls to navigate:
  - `W/A/S/D`: Move forward, left, backward, and right.
  - Arrow keys or mouse: Look around.
  - `ESC`: Exit the program.
- Example map file format:
  ```
  R 800 600
  NO ./textures/wall_north.xpm
  SO ./textures/wall_south.xpm
  WE ./textures/wall_west.xpm
  EA ./textures/wall_east.xpm
  F 220,100,0
  C 225,30,0
  1111111111
  1000000001
  1011111101
  1010000101
  1011111101
  1000000001
  1111111111
  ```

---

## Project Requirements

- Must be implemented in **C**.
- Must follow the **42 Norm**.
- Can only use allowed standard library functions.
- The map must be surrounded by walls and should contain:
  - At least one spawn point (`N`, `S`, `E`, or `W`).
  - Only valid characters (`0`, `1`, `N`, `S`, `E`, `W`, etc.).
- Raycasting algorithm for rendering.

---

## Authors

- [Mohamed Sarda](https://github.com/Mohamedsarda)
- [El Hossein El Ghalbzouri](https://github.com/El-Hossein)

---

## Acknowledgments

- 42 Network for the project.
- Resources on raycasting and 3D rendering.
- Peers and collaborators for feedback and support.
