# Coup - Custom C++ Implementation with Qt GUI

## Overview

This project is a customized implementation of the strategic bluffing game **Coup**, developed in C++ with a graphical user interface using **Qt 5**. It features original roles and action rules, including role-specific abilities and special turn logic.

The game can be played through a user-friendly GUI that allows:
- Adding players before the game starts
- Performing actions each turn
- Viewing each player’s status and coin count
- Undo mechanics for specific roles
- Automated win detection

---

## Build Instructions

### Prerequisites

Make sure you have:
- **g++** with C++17 support
- **Qt 5** development libraries installed (including QtWidgets, QtGui, QtCore)
- **Valgrind** (for memory testing)

On Debian/Ubuntu systems, you can install Qt with:

```bash
sudo apt install qtbase5-dev
```

### Building the GUI

Use the provided `Makefile`. In the root directory, run:

```bash
make run-gui
./coup_gui
```

### Building the Console Version

To build the console-based version of the game:

```bash
make main
./main
```

---

## Running Unit Tests

Unit tests are written in `test_coup.cpp`.

To build and run the tests:

```bash
make test
./test_coup
```

To check for memory leaks:

```bash
make valgrind
```

---

## Game Logic & Structure

### Core Components

- `Game.cpp/.hpp`: Manages game state, players, turns, and actions.
- `Player.cpp/.hpp`: Base player class with default behavior.
- `MainWindow.cpp/.hpp`: Qt GUI implementation (user interface and interaction).
- Role subclasses:
  - `Governor`
  - `Spy`
  - `Baron`
  - `General`
  - `Judge`
  - `Merchant`

### Special Mechanics

- **Tax**: Available to all roles (+2 coins, +3 for Governor).
- **Undo**: Available to Governor, Spy, Judge, and General.
- **View**: Spy can view other players’ coin counts.
- **Invest**: Baron has an invest-specific mechanic.
- **Coup / Bribe / Income / Foreign Aid**: Core game actions.

---

## GUI Features

The GUI allows players to:
- Add up to 6 players before starting
- Begin the game with "Start Game"
- Perform actions using dropdowns
- Select targets for actions
- See live updates of:
  - Player names and roles
  - Coin counts
  - Alive / Dead / Temporarily Dead statuses
  - Current turn owner
- Automatically detect game end and declare winner

---

## Clean Build

To clean up all builds and binaries:

```bash
make clean
```

---


## File Layout

```
.
├── Game.cpp / Game.hpp            # Game logic
├── Player.cpp / Player.hpp        # Base player class
├── MainWindow.cpp / MainWindow.hpp# GUI frontend (Qt5)
├── main.cpp                       # Console-based version
├── test_coup.cpp                  # Unit tests
├── Governor.cpp, Spy.cpp, ...     # Role implementations
├── Makefile                       # Build system
└── README.md                      # This file
```

---

## Notes

- All memory management is handled carefully. Use `valgrind` to verify.
- Do not attempt to add players after the game starts — will throw.
- Target selection is filtered by "alive" status only.
- GUI built and tested with Qt5, not Qt6.

---

