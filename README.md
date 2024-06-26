# Akari_Game
# Akari Game 
![akari](https://github.com/eddahmany/Akari_Game/assets/138607985/24232fad-e7a7-4fbe-ae56-c6a01c3060ea)


The project involves developing a graphical interface to play the Akari game. Akari is a mathematical logic game played on a grid containing black and white cells. The goal of the game is to illuminate all the white cells by correctly placing bulbs on the grid while adhering to the following two rules:
✓ A bulb cannot illuminate another bulb (the light beam emitted by a bulb cannot reach a cell occupied by another bulb).
✓ Some black cells contain a number. This number is always between 0 and 4. It indicates the number of adjacent cells (horizontally or vertically, but not diagonally) containing a bulb.

In our project, we separated the game model and the view into two different classes:
- `GameModel`: Defines the Akari game model.
- `GameWindow`: Defines the Akari game view.

## 1. Model

The `GameModel` class contains various methods and attributes to manage the game's logic and associated data. The class attributes include the game matrix, grid size, difficulty level, number of illuminated cells, and number of correctly illuminated black cells.

The class has methods to read the grid file, set the difficulty level, grid size, and update the illuminated cells and correctly illuminated black cells. It also provides methods to check if a cell is illuminated, empty, or black, and to get the number of illuminated neighbors of a cell.

The class also provides an `updateState()` method to update the game state and an `isFinished()` method, called at each state change to check if the game is finished.

Finally, the class uses an undo stack structure to allow players to undo and redo their moves. This mechanism is defined in the `MatrixCommand` class, which inherits from the `QUndoCommand` class. The `MatrixCommand` class has two main methods: `undo()` and `redo()`. The `undo()` method restores the previous value of a matrix cell and updates the illuminated cells and lit bulbs. The `redo()` method restores the modified value of a matrix cell and updates the illuminated cells and lit bulbs.

## 2. View

The `GameWindow` class inherits from the `QMainWindow` class and represents the main application window. This class manages the user interface and displays game changes applied in the model, such as the grid, the number of illuminated cells, the number of well-surrounded black cells, and the number of undos.

The methods in this class include creating the menu, buttons, progress bars, and digital timer. It also implements the `paintEvent(QPaintEvent e)` method to draw and update the game grid, and the `mousePressEvent(QMouseEvent *event)` method to handle mouse click events. The `GameWindow` class also contains functions to update the progress bars and game labels according to the current game state.

Finally, this class contains slot functions to handle user-triggered events, such as undo, redo, reset game, and display the help window.
