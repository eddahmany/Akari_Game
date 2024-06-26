#include "gamewindow.h"


GameWindow::GameWindow(QWidget *parent, int _inputSize, int _inputLevel)
    : QMainWindow(parent), m_time(QTime::currentTime())
{
    _gameModel = new GameModel(_inputSize, _inputLevel);
    _gameModel->readFile();
    _gridSize = _inputSize*(CELL_SIZE + MARGIN) + MARGIN;
    _width  = _gridSize + SETTINGS_SIZE;
    _height =MENU_BAR_HEIGHT+ _gridSize;
    _grid = new QRect*[_inputSize];
    for(int i = 0; i < _inputSize; ++i) {
        _grid[i] = new QRect[_inputSize];
    }
    this->setWindowTitle(tr("Tableau"));
    this->setFixedSize(_width, _height);

    createMenu();

    createDigitalClock();

    createLabels();

    createProgressBars();

    createButtons();

}
void GameWindow::createMenu(){

    OptionsMenu = menuBar()->addMenu(tr("&Options"));

    UndoAct = new QAction(tr("&Annuler"), this);
    OptionsMenu->addAction(UndoAct);
    UndoAct->setShortcut(tr("Ctrl+Z"));
    connect(UndoAct, &QAction::triggered, this, &GameWindow::handleUndo);


    RedoAct = new QAction(tr("&Refaire"), this);
    OptionsMenu->addAction(RedoAct);
    RedoAct->setShortcut(tr("Ctrl+Y"));
    connect(RedoAct, &QAction::triggered, this, &GameWindow::handleRedo);


    ResetAct = new QAction(tr("&Rénitialiser"), this);
    OptionsMenu->addAction(ResetAct);
    ResetAct->setShortcut(tr("Ctrl+R"));
    connect(ResetAct, &QAction::triggered, this, &GameWindow::resetGame);

    QuitAct = new QAction(tr("&Quitter"), this);
    OptionsMenu->addAction(QuitAct);
    QuitAct->setShortcut(tr("Ctrl+Q"));
    connect(QuitAct, &QAction::triggered, this, &GameWindow::close);


    HelpMenu = menuBar()->addMenu(tr("&Aide"));
    QAction* HelpAct = new QAction(tr("&À propos"), this);
    HelpMenu->addAction(HelpAct);
    HelpAct->setShortcut(tr("Ctrl+H"));
    connect(HelpAct, &QAction::triggered, this, &GameWindow::showHelpWindow);

}

void GameWindow::createDigitalClock(){

    _digitalClock = new QLCDNumber(this);
    _digitalClock->setDigitCount(8); // Affiche les heures, minutes et secondes
    _digitalClock->resize(CHRONO_WIDTH, CHRONO_HEIGHT);
    _digitalClock->move(_gridSize + MARGIN,MARGIN + MENU_BAR_HEIGHT);

    //_digitalClock->move(0,0);

    _digitalClock->setStyleSheet("border: 2px solid black;");
    _digitalClock->setStyleSheet("border:3px solid yellow; border-radius: 10px;color: yellow;font-size : 100;");

    _timer = new QTimer(this);
    _counterTime = 0;
    connect(_timer, SIGNAL(timeout()), this, SLOT(updateDigitalClock()));
    _timer->start(1000);

}

void GameWindow::createLabels(){
    _lightedCells = new QLabel(this);
    _lightedCells->setGeometry(_gridSize + MARGIN,MENU_BAR_HEIGHT+ CHRONO_HEIGHT + 3*MARGIN , CHRONO_WIDTH, WIDGET_HEIGHT);
    _lightedCells->setText(tr("Lampes illuminées "));
    _lightedCells->setStyleSheet("border:3px solid lightGray;color: black;font-size : 100;background-color : lightGray");
    _lightedCells->setAlignment(Qt::AlignCenter);
    _lightedCells->setFont(QFont("Arial", 14));

    _correctlyLightedBlackCells = new QLabel(this);
    _correctlyLightedBlackCells->setGeometry(_gridSize + MARGIN,MENU_BAR_HEIGHT+ CHRONO_HEIGHT + BAR_HEIGHT+ WIDGET_HEIGHT + 5*MARGIN , CHRONO_WIDTH, WIDGET_HEIGHT);
    _correctlyLightedBlackCells->setText(tr("Cases noires correctement illuminées "));
    _correctlyLightedBlackCells->setStyleSheet("border:3px solid lightGray;color: black;font-size : 100 ;background-color : lightGray");
    _correctlyLightedBlackCells->setAlignment(Qt::AlignCenter);
    _correctlyLightedBlackCells->setFont(QFont("Arial", 10.5));

    _numberBacktracks = new QLabel(this);
    _numberBacktracks->setGeometry(_gridSize + MARGIN,MENU_BAR_HEIGHT+ CHRONO_HEIGHT + 2*(BAR_HEIGHT+ WIDGET_HEIGHT)+ BUTTON_HEIGHT + 10*MARGIN , CHRONO_WIDTH, WIDGET_HEIGHT);
    _numberBacktracks->setStyleSheet("border:3px solid lightGray;color: black;font-size : 100 ;background-color : lightGray");
    _numberBacktracks->setAlignment(Qt::AlignCenter);
    _numberBacktracks->setFont(QFont("Arial", 10));
    updateBacktracksLabel();


}
void GameWindow::createProgressBars(){
    _lightedCellsProgressBar  = new QProgressBar(this);
    _lightedCellsProgressBar->setRange(0, *(_gameModel->_requiredNumberOfLightedCells));
    updateLightedCellsBar();
    _lightedCellsProgressBar->resize(BAR_WIDTH, BAR_HEIGHT);
    _lightedCellsProgressBar->move(_gridSize + MARGIN,MENU_BAR_HEIGHT+CHRONO_HEIGHT +WIDGET_HEIGHT + 3*MARGIN);
    QString style = "QProgressBar::chunk { background-color: yellow; }\
                    QProgressBar { background-color: #808080;  \
                    border: 3px solid black; \
                    color: #000000; \
                    font-size: 20px; \
                    text-align: center; } \
                     ";
    _lightedCellsProgressBar->setStyleSheet(style);

    _correctlyLightedBlackCellsProgressBar  = new QProgressBar(this);
    _correctlyLightedBlackCellsProgressBar->setRange(0, *(_gameModel->_requiredNumberOfLightedCells));
    updateCorrectlyLightedBlackCellsProgressBar();
    _correctlyLightedBlackCellsProgressBar->resize(BAR_WIDTH, BAR_HEIGHT);
    _correctlyLightedBlackCellsProgressBar->move(_gridSize + MARGIN,MENU_BAR_HEIGHT+CHRONO_HEIGHT + BAR_HEIGHT+ 2*WIDGET_HEIGHT + 5*MARGIN); // Positionner la barre de progression à l'emplacement (50, 50) dans la fenêtre
    _correctlyLightedBlackCellsProgressBar->setStyleSheet(style);
}

void GameWindow::createButtons(){
    _undoButton = new QPushButton(tr("Annuler"), this);
    _undoButton->setGeometry(_gridSize + MARGIN,MENU_BAR_HEIGHT+ CHRONO_HEIGHT + 2*BAR_HEIGHT+ 2*WIDGET_HEIGHT + 7*MARGIN, BUTTON_WIDTH, BUTTON_HEIGHT);
    connect(_undoButton, &QPushButton::clicked, this, &GameWindow::handleUndo);


    _redoButton = new QPushButton(tr("Refaire"), this);
    _redoButton->setGeometry(_gridSize + 2.5*MARGIN +BUTTON_WIDTH,MENU_BAR_HEIGHT+ CHRONO_HEIGHT + 2*BAR_HEIGHT+ 2*WIDGET_HEIGHT + 7*MARGIN , BUTTON_WIDTH, BUTTON_HEIGHT);
    connect(_redoButton, &QPushButton::clicked, this, &GameWindow::handleRedo);


    _resetButton = new QPushButton(tr("Réinitialiser"), this);
    _resetButton->setGeometry(_gridSize + 4*MARGIN + 2*BUTTON_WIDTH,MENU_BAR_HEIGHT+ CHRONO_HEIGHT + 2*BAR_HEIGHT+ 2*WIDGET_HEIGHT + 7*MARGIN , BUTTON_WIDTH, BUTTON_HEIGHT);
    QFont font("Arial", 10);
    _resetButton->setFont(font);
    connect(_resetButton, &QPushButton::clicked, this, &GameWindow::resetGame);

}


void GameWindow::paintEvent(QPaintEvent* e)
{

    QPainter painter(this);
    QPen WhitePen = QPen(Qt::white);
    QPen BlackPen = QPen(Qt::black);
    QPen GreenPen = QPen(Qt::green);

    QPixmap lightImage = QPixmap("../Akiri/ressources/light.png");
    QPixmap redLightImage = QPixmap("../Akiri/ressources/red_light.png");

    QVector<QPair<int, int>> lights;
    QFont font = painter.font();
    font.setPointSize(28);
    painter.setFont(font);

    painter.setBrush(Qt::lightGray);
    QRect GlobalRect = QRect(0,0, _width, _height);
    painter.drawRect(GlobalRect);

    painter.setBrush(Qt::black);
    for (int i = 0 ; i< _gameModel->_size; i ++){
        for (int j = 0 ; j< _gameModel->_size; j ++){
            painter.setPen(BlackPen);
            QRect currentRect = QRect(i*(CELL_SIZE + MARGIN) + MARGIN ,j*(CELL_SIZE + MARGIN) + MARGIN  +MENU_BAR_HEIGHT, CELL_SIZE, CELL_SIZE);
            _grid[i][j] = currentRect;
            switch(_gameModel->_matrix[i][j]){
            case '.':{
                painter.setBrush(Qt::darkGray);
                painter.drawRect(currentRect);
                break;
            }
            case '!':{
                lights.append(QPair<int, int>(i, j));
                painter.setBrush(Qt::yellow);
                painter.drawRect(currentRect);
                QVector<QPair<int, int>> redLightPositions = _gameModel->getLightsOnSameColumn( i, j) + _gameModel->getLightsOnSameRow( i, j);
                if (! redLightPositions.isEmpty()){
                    painter.drawPixmap(currentRect, redLightImage);

                    for (const auto &index : redLightPositions) {
                        int row = index.first;
                        int col = index.second;
                        painter.drawPixmap((_grid[row][col]), redLightImage);
                    }
                }
                else {       painter.drawPixmap(currentRect, lightImage);}
                break;}
            case 'X':{
                painter.setBrush(Qt::black);
                painter.drawRect(currentRect);
                break;}
            default:
                std::string s(1, _gameModel->_matrix[i][j]);
                painter.setBrush(Qt::black);

                if ( _gameModel->getNeighboursLights(i, j) == std::stoi(s)){
                    painter.setPen(GreenPen);}
                else { painter.setPen(WhitePen); }
                painter.drawRect(currentRect);
                painter.drawText(currentRect, Qt::AlignCenter, QString::fromStdString(std::string(1, _gameModel->_matrix[i][j])));

            }
    }
}
    TurnOnCells(lights);


}

void GameWindow::TurnOnCells(QVector<QPair<int, int>> lights){
    QPainter painter(this);
    painter.setBrush(Qt::yellow);
    for (const auto &index : lights) {
        int i = index.first;
        int j = index.second;
        QVector<QPair<int, int>> indices = _gameModel->getLightedCells( i, j);
        for (const auto &index : indices) {
            int row = index.first;
            int col = index.second;
            painter.fillRect((_grid[row][col]), painter.brush());
            char element = _gameModel->_matrix[row][col];
        }
    }
}






void GameWindow::mousePressEvent(QMouseEvent *event){
    int x = event->x();
    int y = event->y();
    int row = x / (CELL_SIZE + MARGIN) ;
    int col = (y - MENU_BAR_HEIGHT)/ (CELL_SIZE + MARGIN) ;

    if ( row < _gameModel->_size && x > row*(CELL_SIZE + MARGIN) + MARGIN && x < (row+1)*(CELL_SIZE + MARGIN)){
        if (col < _gameModel->_size && y > col*(CELL_SIZE + MARGIN) + MARGIN + MENU_BAR_HEIGHT && y < (col+1)*(CELL_SIZE + MARGIN) + MENU_BAR_HEIGHT ){
            _gameModel->updateState(row, col);

            update();
            updateLightedCellsBar();
            updateCorrectlyLightedBlackCellsProgressBar();
            updateBacktracksLabel();


            if (_gameModel->isFinished() == true ){
                _timer->stop();
                QMessageBox msgBox;
                QString nbHours = (_counterTime / 3600 != 0) ? QString::number(_counterTime / 3600) + "h, " : "";
                QString nbMinutes = (_counterTime / 60 != 0) ? QString::number(_counterTime / 60)  + "min, " : "";
                QString nbSeconds = QString::number(_counterTime % 60) + " secondes.";

                QString message = tr("Bien Joué ✅ \n temps passé : ") + nbHours + nbMinutes + nbSeconds;
                msgBox.setText(message);
                msgBox.exec();

        }
        }
    }
}



void GameWindow::updateLightedCellsBar(){
    _gameModel->updateNumberOfLightedCells();
    QString ratioString = QString("%1/%2").arg(QString::number(*(_gameModel->_lightedCells))).arg(QString::number(*(_gameModel->_requiredNumberOfLightedCells)));
    _lightedCellsProgressBar->setFormat(ratioString);
    _lightedCellsProgressBar->setValue((*(_gameModel->_lightedCells)));

}

void GameWindow::updateCorrectlyLightedBlackCellsProgressBar(){
    _gameModel->updateNumberOfCorrectlyLightedBlackCells();
    QString ratioString = QString("%1/%2").arg(QString::number(*(_gameModel->_correctlyLightedBlackCells))).arg(QString::number(*(_gameModel->_requiredLightedBlackCells)));
    _correctlyLightedBlackCellsProgressBar->setFormat(ratioString);
    _correctlyLightedBlackCellsProgressBar->setValue((*(_gameModel->_correctlyLightedBlackCells))*(*(_gameModel->_requiredNumberOfLightedCells))/(*(_gameModel->_requiredLightedBlackCells)) );
}



void GameWindow::updateBacktracksLabel(){
    if (*(_gameModel->_numberBacktracks)== 0 ){
        _numberBacktracks->hide();
    }
    else{
        _numberBacktracks->show();
        _numberBacktracks->setText(tr("Nombre de retours en arrière :")+QString::number(*(_gameModel->_numberBacktracks)));
    }

}

// Slots utilisées :



void GameWindow::updateDigitalClock()
{

    _counterTime++;
    QString text = QString("%1:%2:%3")
            .arg(_counterTime/3600, 2, 10, QChar('0'))
            .arg(_counterTime/60, 2, 10, QChar('0'))
            .arg(_counterTime%60,  2, 10, QChar('0'));

    _digitalClock->display(text);

}

void GameWindow::resetGame()
{
    for ( int i = *(_gameModel->_numberMoves); i > 0 ; i--){
        handleUndo();
    }
    _gameModel->resetGame();
    updateBacktracksLabel();

}


void GameWindow::showHelpWindow(){
    QDialog* helpWindow = new QDialog(this);
    helpWindow->setWindowTitle(tr("Aide"));

    QLabel* label = new QLabel(helpWindow);
    label->setTextFormat(Qt::RichText);
    label->setWordWrap(true);
    label->setText(tr("<h2>Comment jouer à l'akari</h2>"
                      "<p>Un akari est un jeu mathématique de logique sous forme d'une grille comprenant des cases noires et des cases grises. Le but du jeu est d'éclairer chaque case grise en plaçant des ampoules aux bons endroits tout en respectant les deux règles suivantes :</p>"
                      "<ol>"
                      "<li>Une ampoule ne peut pas en éclairer une autre (le rayon lumineux diffusé par une ampoule ne peut pas atteindre une case occupée par une autre ampoule) :</li>"
                      "<p align='center'><img src='../Akiri/ressources/rule_1.png'></p>"
                      "<br>"
                      "<br>"
                      "<li>Certaines cases noires contiennent un chiffre. Celui-ci est toujours compris entre 0 et 4. Il s’agit du nombre de cases adjacentes (horizontalement ou verticalement, mais pas en oblique) contenant une ampoule :</li>"
                      "<p align='center'><img src='../Akiri/ressources/rule_2.png'></p>"
                      "</ol>"
                      "<br>"
                      "<br>"
                      "<p>Pour placer une ampoule, cliquez sur une case vide. Pour supprimer une ampoule, cliquez à nouveau sur la case correspondante. Le jeu est terminé lorsque toutes les cases grises sont éclairées et que les règles sont respectées.</p>"
                      "<p align='center'>Exemple d'akari complété : <p>"
                      "<p align='center'><img src='../Akiri/ressources/example.png'></p>"
                      ));

    QScrollArea* scrollArea = new QScrollArea(helpWindow);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(label);
    helpWindow->setStyleSheet("background-color: lightGray;");
    label->setStyleSheet("background-color: lightGray;");

    helpWindow->setFixedSize(725, 520);
    helpWindow->exec();
}


void GameWindow::handleUndo()
{
    _gameModel->_undoStack->undo();
    update();
    updateLightedCellsBar();
    updateCorrectlyLightedBlackCellsProgressBar();
    updateBacktracksLabel();
}

void GameWindow::handleRedo()
{
    _gameModel->_undoStack->redo();
    update();
    updateLightedCellsBar();
    updateCorrectlyLightedBlackCellsProgressBar();
    updateBacktracksLabel();
}

GameWindow::~GameWindow()
{
    for (int i = 0; i < _gameModel->_size; ++i) {
        delete[] _grid[i];
    }
    delete[] _grid;
    delete _gameModel;
    delete _digitalClock;
    delete _undoButton;
    delete _redoButton;

}



