# 🐍 Terminal Snake Game

A classic Snake game implementation in C that runs directly in your terminal!

## 📋 Description

This is a terminal-based Snake game written in pure C. Control the snake using arrow keys or WASD, eat food to grow longer, and try to achieve the highest score without hitting the walls or yourself!

## ✨ Features

- **Smooth Controls**: Responsive keyboard input using arrow keys or WASD
- **Score Tracking**: Earn 10 points for each food item eaten
- **Dynamic Growth**: Snake grows longer with each food consumed
- **Collision Detection**: Game ends when hitting walls or the snake's own body
- **Clean ASCII Graphics**: Simple and clear visual representation
- **Adjustable Speed**: Easy to modify game speed in the code

## 🎮 Game Elements

- `O` - Snake head
- `o` - Snake body segments
- `*` - Food
- `#` - Walls/borders
- Score and length displayed below the game board

## 🚀 Getting Started

### Prerequisites

- GCC compiler (or any C compiler)
- Linux/Unix terminal (or WSL on Windows)
- Terminal that supports ANSI escape codes

### Compilation

```bash
gcc -o snake_game snake_game.c
```

For additional warnings and debugging:
```bash
gcc -o snake_game snake_game.c -Wall -Wextra
```

### Running the Game

```bash
./snake_game
```

## 🎯 How to Play

1. **Start the Game**: Run the compiled executable
2. **Controls**:
   - **Arrow Keys** or **W/A/S/D**: Move the snake (Up/Left/Down/Right)
   - **Q**: Quit the game at any time
3. **Objective**: Eat the food (`*`) to grow your snake and increase your score
4. **Avoid**:
   - Hitting the walls (`#`)
   - Running into your own body
5. **Win Condition**: There's no winning - just try to beat your high score!

## 🎲 Game Rules

- The snake starts with a length of 3 segments
- Each food item eaten adds 1 segment to the snake's length
- Each food item eaten adds 10 points to your score
- The snake cannot reverse direction (e.g., if moving right, can't immediately go left)
- Game ends on collision with walls or self

## ⚙️ Customization

You can easily modify the game by changing these constants in the code:

```c
#define WIDTH 40          // Board width (default: 40)
#define HEIGHT 20         // Board height (default: 20)

// In main() function:
usleep(100000);          // Game speed in microseconds (default: 100ms)
                         // Lower value = faster game
                         // Higher value = slower game
```

### Speed Settings Examples:
- **Easy**: `usleep(150000)` - 150ms delay
- **Medium**: `usleep(100000)` - 100ms delay (default)
- **Hard**: `usleep(50000)` - 50ms delay
- **Expert**: `usleep(30000)` - 30ms delay

### Score Settings:
Change the score increment in the code:
```c
score += 10;  // Change 10 to any value you prefer
```

## 🛠️ Technical Details

### Terminal Control
The game uses:
- `termios.h` for raw terminal mode (no buffering)
- ANSI escape codes for screen clearing
- `select()` for non-blocking keyboard input

### Game Loop
The game runs at approximately 10 FPS (configurable) and follows this cycle:
1. Handle user input
2. Move snake
3. Check for food collision
4. Check for wall/self collision
5. Redraw board
6. Sleep for configured delay

### Memory Usage
- Maximum snake length: 800 segments (40 × 20 = entire board)
- Minimal memory footprint
- No dynamic memory allocation

## 🐛 Troubleshooting

**Problem**: Arrow keys don't work
- **Solution**: Make sure your terminal supports ANSI escape sequences. Try using WASD keys instead.

**Problem**: Game is too fast/slow
- **Solution**: Adjust the `usleep()` value in the main game loop.

**Problem**: Screen flickering
- **Solution**: This is normal due to terminal clearing. You can reduce it by optimizing the drawing code.

**Problem**: Terminal doesn't reset after crash
- **Solution**: Type `reset` in your terminal to restore normal settings.

## 📝 File Structure

```
snake_game.c          # Main game source code
snake_game            # Compiled executable (after compilation)
README.md             # This file
```

## 🎓 Learning Outcomes

This project demonstrates:
- Terminal I/O manipulation in C
- Game loop implementation
- Collision detection algorithms
- Array and structure usage
- Real-time input handling
- ANSI terminal control

## 📜 License

This is free and open-source software. Feel free to modify and distribute.

## 🤝 Contributing

Feel free to fork this project and add your own features! Some ideas:
- Add difficulty levels
- Implement obstacles
- Add power-ups
- Create a high score system
- Add sound effects (terminal beep)
- Implement different game modes

## 📧 Support

If you encounter any issues or have questions, feel free to open an issue or reach out!

## 🎉 Credits

Classic Snake game concept - A timeless classic enjoyed by millions!

---

**Enjoy the game and happy coding! 🐍**
