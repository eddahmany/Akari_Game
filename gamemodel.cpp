#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QVector>
#include <QString>
#include "gamemodel.h"
#include <QDebug>



using namespace std;

GameModel::GameModel(int _inputSize, int _inputLevel)
{
    _size = _inputSize;
    _level = (Level)_inputLevel;
    _undoStack = new QUndoStack();

    _matrix = new char*[_size];
    for(int i = 0; i < _size; ++i) {
        _matrix[i] = new char[_size];
    }
    _state = new int*[_size];
    for(int i = 0; i < _size; ++i) {
        _state[i] = new int[_size];

    }
    for(int i = 0; i < _size; ++i) {
        for (int j = 0; j < _size; j++)
        _state[i][j] = 0;
    }
    _requiredNumberOfLightedCells = new int(0);
    _lightedCells = new int(0);
    _correctlyLightedBlackCells = new int(0);
    _requiredLightedBlackCells = new int(0);
    _numberMoves = new int(0);
    _numberBacktracks = new int(0);


}
GameModel::~GameModel(){
    for (int i = 0; i < _size; ++i) {
        delete[] _matrix[i];
    }
    delete[] _matrix;

    for (int i = 0; i < _size; ++i) {
        delete[] _state[i];
    }
    delete[] _state;
    delete _undoStack;
}



void GameModel::setMatrix(int i, int j, char c){
    char oldValue = _matrix[i][j];
    _matrix[i][j] = c;
    MatrixCommand* command = new MatrixCommand(oldValue, c, i, j, _size, _matrix,_state, _numberBacktracks);
    _undoStack->push(command);

}

bool GameModel::isLightCell(int i, int j){
    return (_matrix[i][j] == '!');
}
bool GameModel::isEmptyCell(int i, int j){
    return (_matrix[i][j] == '.');
}
bool GameModel::isWall(int i, int j){
    return (!isLightCell(i,j) && !isEmptyCell(i,j));
}


QString GameModel::getFilePath(){

    QString resourcesPath = "../Akiri/ressources/Akiri_Grids/";
    QString filename;
    switch (_level) {
        case easy:
            filename = QString::number(_size) + "_easy.txt";
            break;
        case normal:
        filename = QString::number(_size) + "_normal.txt";
        break;
        default:
            filename = QString::number(_size) + "_hard.txt";
    }

    QString filepath = resourcesPath + filename;
    return filepath;
}

void GameModel::readFile(){

    int numRows;
    QString filepath = getFilePath();
    QFile inputFile(filepath);
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        numRows = 0;
        while (!in.atEnd())
        {
            in.readLine();
            numRows++;
        }


        int randomLine = qrand() % (numRows- 1) +1 ;

        if (inputFile.reset() && inputFile.seek(0))
        {
            QTextStream in(&inputFile);
            int i = 0;
            while (!in.atEnd())
            {
                QString line = in.readLine();
                if (i == randomLine)
                {
                    QTextStream lineStream(&line);
                    int j = 0;
                    char value;
                    while (!lineStream.atEnd()  && j < _size*_size)
                    {
                        lineStream >> value;
                        _matrix[j/_size][j%_size] = value;
                        if (! isWall(j/_size, j%_size)) (*_requiredNumberOfLightedCells)++;
                        if (isWall(j/_size, j%_size) && value != 'X') (*_requiredLightedBlackCells)++;
                        j++;
                    }

                }
                i++;
            }
            }


            inputFile.close();
        }

}


//retourner les indices des cases qui doivent etre allumées a partir de la lampe de position (i, j)

int GameModel::getNeighboursLights(int row, int col){
    int numbersOfLights = 0;
    for(int i = - 1; i <= 1; i++){
        if (i != 0 && row+i > -1 && row+i < _size && isLightCell(row+i, col)){
            numbersOfLights++;
        }
    }
    for(int j = - 1; j <= 1; j++){
         if (j != 0 && col+j > -1 && col+j < _size && isLightCell(row, col+j)){
            numbersOfLights++;
         }
    }
    return numbersOfLights;
}


QVector<QPair<int, int>> GameModel::getLightedCells(int i, int j)
{


    QVector<QPair<int, int>> indices;

    // Ajouter les indices de la même colonne
    for (int row = i-1; row >= 0 && _matrix[row][j] == '.'; row--){
        indices.append(QPair<int, int>(row, j));
    }
    for (int row = i+1; row < _size && _matrix[row][j] == '.'; row++){
        indices.append(QPair<int, int>(row, j));
    }

    // Ajouter les indices de la même ligne
    for (int col = j-1; col >= 0 && _matrix[i][col] == '.'; col--){
        indices.append(QPair<int, int>(i, col));
    }

    for (int col = j+1; col < _size && _matrix[i][col] == '.'; col++){
        indices.append(QPair<int, int>(i, col));
    }
    return indices;
}

void GameModel::printState(){
    qDebug() << "Print State : ";
    for (int i = 0; i < _size; i++) {
        QString row = "";
        for (int j = 0; j < _size; j++) {
            row += QString::number(_state[j][i]) + " ";
        }
        qDebug() << row;
    }

}
void GameModel::resetGame(){
    _undoStack->clear();
    *(_numberBacktracks) = 0;
}


void GameModel::updateState(int row , int col){
    *_numberBacktracks = 0;
    QVector<QPair<int, int>> indices = getLightedCells( row, col);
    switch (_matrix[row][col]){
        case '.':{
            setMatrix(row, col, '!');


        break;}

    case '!' :{
        setMatrix(row, col, '.');
        for (const auto &index : indices) {
            int i = index.first;
            int j = index.second;

        }

        break;}

    default:
        break;
    }
    (*_numberMoves)++;
    printState();
}



QVector<QPair<int, int>> GameModel::getLightsOnSameColumn(int i, int j){
    QVector<QPair<int, int>> indices;
    // Ajouter les indices de la même colonne
    for (int row = i-1; row >= 0 && !isWall(row,j); row--){
        if (isLightCell(row, j)){
            indices.append(QPair<int, int>(row, j));
        }
    }
    for (int row = i+1; row < _size && !isWall(row,j); row++){
        if (isLightCell(row, j)){
            indices.append(QPair<int, int>(row, j));
        }
    }    return indices;

}


QVector<QPair<int, int>> GameModel::getLightsOnSameRow(int i, int j){
    QVector<QPair<int, int>> indices;

    // Ajouter les indices de la même ligne

    for (int col = j-1; col >= 0 && !isWall(i,col); col--){
        if (isLightCell(i, col)) indices.append(QPair<int, int>(i,col));
    }
    for (int col = j+1; col < _size && !isWall(i,col); col++){
        if (isLightCell(i, col)) indices.append(QPair<int, int>(i,col));
    }
    return indices;
}

void GameModel::printMatrix(){
    qDebug() << "Print Matrix : ";
    for (int i = 0; i < _size; i++) {
        QString row = "";
        for (int j = 0; j < _size; j++) {
            row += QString(_matrix[j][i]) + " ";
        }
        qDebug() << row;
    }


}
void GameModel::updateNumberOfCorrectlyLightedBlackCells(){
    int CorrectlyLightedBlackCells = 0;

    for (int i = 0; i < _size; ++i) {
        for (int j = 0 ; j < _size ; j++){
            if (isWall(i,j) && _matrix[i][j] != 'X'){
                string s(1, _matrix[i][j]);

                if ( getNeighboursLights(i, j) == stoi(s)){
                    CorrectlyLightedBlackCells++;
                }
            }
        }
    }
    *_correctlyLightedBlackCells = CorrectlyLightedBlackCells;
}


void GameModel::updateNumberOfLightedCells(){
    int lightedCells = 0;
    for (int i = 0; i < _size; ++i) {
        for (int j = 0 ; j < _size ; j++){

            switch(_matrix[i][j]){
                case '.':
                if (_state[i][j] > 0) lightedCells++;
                break;
                case '!': lightedCells++;
                break;
                default:
                break;

            }
        }
    }

    *_lightedCells = lightedCells;

}
void GameModel::printNumber(){
    qDebug() << "<<<<<<<<<<<<<<<<<<<<<<_lightedCells : " << *(_lightedCells);

}


bool GameModel::isFinished(){
    qDebug() << "testing Game finishing : ";
    for (int i = 0; i < _size; ++i) {
        for (int j = 0 ; j < _size ; j++){

            switch(_matrix[i][j]){
                case '.':

                if (_state[i][j] == 0) return false;
                break;
                case '!':
                    if ( !getLightsOnSameRow(i,j).isEmpty() || !getLightsOnSameColumn(i,j).isEmpty()){
                        return false;
                    }
                break;
            case 'X':
                break;
                default:
                    string s(1, _matrix[i][j]);

                    int requiredLights = _matrix[i][j] - '0';

                    if (getNeighboursLights(i, j) != requiredLights){ return false;
                    }
                break;

            }
        }
    }

    return true;
}
