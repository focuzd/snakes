# snakes

A terminal-based Snake game written in C using the ncurses library.

## About

Classic snake gameplay in your terminal. Eat food to grow, avoid running into yourself. The game features a bordered playground window, a live score display, and ASCII art title/game-over screens.

### Controls

| Key | Action |
|---|---|
| `W` / `↑` | Move up |
| `A` / `←` | Move left |
| `S` / `↓` | Move down |
| `D` / `→` | Move right |
| `Q` | Quit |

### Game Rules

- The snake wraps around the edges of the playground.
- Eating food (`*`) increases your score and length.
- Colliding with your own body ends the game.

## Building

### Prerequisites

- A C compiler (gcc)
- ncurses development library

On Debian/Ubuntu:
```
sudo apt install gcc libncurses5-dev
```

On Fedora:
```
sudo dnf install gcc ncurses-devel
```

On Arch:
```
sudo pacman -S gcc ncurses
```

### Compile

```
gcc snake.c -o snake -lncurses
```

### Run

```
./snake
```

## Project Structure

```
.
├── snake.c            # Main game source
├── skeleton_engine.c  # Early prototype / skeleton code
├── start_screen.txt   # ASCII art start screen
├── game_over.txt      # ASCII art game over screen
└── README.md
```

## License

Do whatever you want with it.
