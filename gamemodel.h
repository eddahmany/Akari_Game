#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include <QVector>
#include "matrixcommand.h"
#include <QPushButton>
#include <QProgressBar>


class GameModel
{

public:
    enum Level{
        easy = 0,
        normal,
        hard,
    };
    GameModel(int _inputSize, int _inputLevel);
    ~GameModel();

    QString getFilePath();
    void readFile();

    void setLevel(Level level);
    void setSize(int size);
    void setMatrix(int i, int j, char c);

    bool isLightCell(int i, int j);
    bool isEmptyCell(int i, int j);
    bool isWall(int i, int j);

    int getNeighboursLights(int row, int col);


    QVector<QPair<int, int>> getLightedCells(int i, int j);
    QVector<QPair<int, int>> getLightsOnSameColumn(int i, int j);
    QVector<QPair<int, int>> getLightsOnSameRow(int i, int j);

    void updateNumberOfLightedCells();
    void updateNumberOfCorrectlyLightedBlackCells();



    bool isFinished();
    void printMatrix();
    void printState();

    void updateState(int row ,int col);

    void resetGame();
    void printNumber();



    char ** _matrix;
    int** _state;
    int _size;
    Level _level;
    QUndoStack* _undoStack;
    int* _lightedCells;
    int *_requiredNumberOfLightedCells;
    int * _correctlyLightedBlackCells;
    int * _requiredLightedBlackCells;
    int * _numberMoves;
    int * _numberBacktracks;
};
#endif // GAMEMODEL_H
