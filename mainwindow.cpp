#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Akari"));



}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_StartGameButton_clicked()
{
    _gameWindow = new GameWindow(this, _inputSize, _inputLevel);

    _gameWindow->show();

}

void MainWindow::on_GridSizeComboBox_currentIndexChanged(int index)
{
    switch( index){
        case 0:
        _inputSize = 7;
        break;

        case 1 :
        _inputSize = 10;

            break;
        case 2:
        _inputSize = 14;
            break;

       default :
        _inputSize = 3;


    }
}

void MainWindow::on_LevelComboBox_currentIndexChanged(int index)
{
    _inputLevel = index;
}
