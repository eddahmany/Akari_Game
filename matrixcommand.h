#ifndef MATRIXCOMMAND_H
#define MATRIXCOMMAND_H

#include <QUndoCommand>
#include <QtGui>
#include <QPair>



class MatrixCommand : public QUndoCommand
{
public:
    MatrixCommand(char oldValue, char newValue, int row, int col,int size, char** matrix, int ** state, int * numberBacktracks);

    void undo();
    void redo();
    QVector<QPair<int, int>> getLateralCells(int i, int j);
    void updateState( char value,QVector<QPair<int, int>> indices, int i , int j);


private:
    char _oldValue;
    char _newValue;
    int _row;
    int _col;
    int _size;
    char** _matrix;
    int** _state;
    int * _numberBacktracks;
    /*char* _ptrChangedPoint;
    int * _ptrLightedCells;
    //int _oldNumberLightedCells;
    //int _newNumberLightedCells;
    int _newValueLightedCells;
    int _oldValueLightedCells;*/

};

#endif // MATRIXCOMMAND_H
