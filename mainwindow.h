#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gamewindow.h"
#include "gamemodel.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int _inputSize = 7;
    int _inputLevel = 0;

private slots:
    void on_StartGameButton_clicked();

    void on_GridSizeComboBox_currentIndexChanged(int index);

    void on_LevelComboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    GameWindow* _gameWindow;
};
#endif // MAINWINDOW_H
