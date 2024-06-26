#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H
#define CELL_SIZE 60
#define MARGIN 10
#define CHRONO_WIDTH 300
#define CHRONO_HEIGHT 100
#define WIDGET_HEIGHT 30
#define BAR_HEIGHT 40
#define BAR_WIDTH 300
#define SETTINGS_SIZE 320
#define BUTTON_HEIGHT 50
#define BUTTON_WIDTH 90
#define MENU_BAR_HEIGHT 30


#include <QMainWindow>
#include "gamemodel.h"
#include <QShortcut>
#include <QGridLayout>
#include<QMouseEvent>
#include<QMessageBox>
#include <QtGui>
#include <QLabel>
#include <QFont>
#include <QPixmap>
#include <QLCDNumber>
#include <QPaintEvent>
#include <QMenuBar>
#include <QAction>
#include <QScrollArea>



class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent, int _inputSize , int _inputLevel);

    ~GameWindow();

    void createMenu();
    void createDigitalClock();
    void createButtons();
    void createLabels();
    void createProgressBars();

   void paintEvent(QPaintEvent* e);
   void TurnOnCells(QVector<QPair<int, int>>);
   void mousePressEvent(QMouseEvent *event);

   void updateLightedCellsBar();
   void updateCorrectlyLightedBlackCellsProgressBar();
   void updateBacktracksLabel();



    QRect** _grid;
    int _width;
    int _height;
    int _gridSize;
    GameModel* _gameModel;
    QTime m_time;
    QTimer *_timer;
    QLCDNumber * _digitalClock;
    int _counterTime;
    QPushButton *_undoButton;
    QPushButton *_redoButton;
    QPushButton *_resetButton;

    QLabel * RequiredLightedCells;
    QLabel * _lightedCells;
    QLabel * _numberBacktracks;
    QLabel * _correctlyLightedBlackCells;

    QProgressBar *_lightedCellsProgressBar;
    QProgressBar * _correctlyLightedBlackCellsProgressBar;

    QMenu *OptionsMenu;
    QMenu *HelpMenu;

    QAction *UndoAct;
    QAction *RedoAct;
    QAction *ResetAct;
    QAction *QuitAct;
    QAction *AboutAct;





public slots :
    void showHelpWindow();

    void handleUndo();
    void handleRedo();
    void resetGame();

    void updateDigitalClock();

};

#endif // GAMEWINDOW_H
