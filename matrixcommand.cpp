#include "matrixcommand.h"

MatrixCommand::MatrixCommand(char oldValue, char newValue, int row, int col,int size, char** matrix, int ** state, int * numberBacktracks)
    : _oldValue(oldValue), _newValue(newValue), _row(row), _col(col), _size(size)
{
    _matrix = matrix;
    _state = state;
    _numberBacktracks = numberBacktracks;
}


QVector<QPair<int, int>> MatrixCommand::getLateralCells(int i, int j)
{
    QVector<QPair<int, int>> indices;
    // Ajouter les indices de la même colonne
    for (int row = i-1; row >= 0 && (_matrix[row][j] == '.'|| _matrix[row][j] == '!'); row--){
        indices.append(QPair<int, int>(row, j));
    }
    for (int row = i+1; row < _size && (_matrix[row][j] == '.' || _matrix[row][j] == '!'); row++){
        indices.append(QPair<int, int>(row, j));
    }

    // Ajouter les indices de la même ligne
    for (int col = j-1; col >= 0 && (_matrix[i][col] == '.' || _matrix[i][col] == '!'); col--){
        indices.append(QPair<int, int>(i, col));
    }

    for (int col = j+1; col < _size && (_matrix[i][col] == '.' || _matrix[i][col] == '!'); col++){
        indices.append(QPair<int, int>(i, col));
    }
    return indices;
}

void MatrixCommand::updateState( char value, QVector<QPair<int, int>> indices, int i , int j){
    switch (value){
        case '!':{
            (_state[i][j])++;
            for (const auto &index : indices) {
                int row = index.first;
                int col = index.second;
                    (_state[row][col])++;
            }

        break;}

    case '.' :{
        (_state[i][j])--;
        for (const auto &index : indices) {
            int row = index.first;
            int col = index.second;
                _state[row][col]--;

        }
    }
    default:
        break;
}
}

void MatrixCommand::undo()
{
    (*_numberBacktracks)++;

    _matrix[_row][_col] = _oldValue;

    updateState(_matrix[_row][_col],getLateralCells(_row, _col), _row, _col);


}

void MatrixCommand::redo()
{
    (*_numberBacktracks)--;
    if ((*_numberBacktracks) < 0) (*_numberBacktracks)=0;


    _matrix[_row][_col] = _newValue;
    updateState(_matrix[_row][_col],getLateralCells(_row, _col), _row, _col);




}
