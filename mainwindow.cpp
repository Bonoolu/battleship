#include "mainwindow.h"
#include "ui_mainwindow.h"

#define calculateTileColor(x, y) ((x + ((y % 2) * ((Configuration::gridSize + 1) % 2 ))) % 2) ? "#185cb1" : "#7a9fd3"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSize size = QGuiApplication::screenAt(QCursor::pos())->size();
    float height = size.height();
    float width = size.width();

    this->setFixedHeight(height);
    this->setFixedWidth(width);

    showFullScreen();

    QPixmap oceanBackground;

    float aspectRatio = width/height;
    if(fabs(aspectRatio - (16.0/9.0)) < 0.01){
        oceanBackground = QPixmap(":/assets/ocean_16_9.jpg");
    }
    else if(fabs(aspectRatio - (16.0/10.0)) < 0.01){
        oceanBackground = QPixmap(":/assets/ocean_16_10.jpg");
    }
    else if(fabs(aspectRatio - (4.0/3.0)) < 0.01){
        oceanBackground = QPixmap(":/assets/ocean_4_3.jpg");
    }
    else {
        oceanBackground = QPixmap(":/assets/ocean_16_9.jpg");
    }

    oceanBackground = oceanBackground.scaled(width, height, Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, oceanBackground);
    this->setPalette(palette);

    ui->centralwidget->setFixedWidth(width);
    ui->centralwidget->setFixedHeight(height);

    playerArea = new QList<Tile*>();
    enemyArea = new QList<Tile*>();

    areaWidth = 1500.0 * width / 3840.0;

    ui->widgetContainerEnemy->setFixedHeight(areaWidth);
    ui->widgetContainerEnemy->setFixedWidth(areaWidth);

    ui->widgetContainerPlayer->setFixedHeight(areaWidth);
    ui->widgetContainerPlayer->setFixedWidth(areaWidth);

    ui->verticalSpacerScore->changeSize(20, 400.0 * width / 3840.0);

    ui->horizontalSpacerScore1->changeSize(40.0 * width / 3840.0, 20);
    ui->horizontalSpacerScore2->changeSize(40.0 * width / 3840.0, 20);
    ui->horizontalSpacerScore3->changeSize(40.0 * width / 3840.0, 20);

    ui->verticalSpacerCenterBottom->changeSize(40.0, height / 2.0);

    ui->logo->setFixedHeight(476.0 * width / 3840.0);
    ui->logo->setFixedWidth(1756.0 * width / 3840.0);

    QFontDatabase::addApplicationFont(":/assets/myfont.ttf");
    QFont myfont1("MyFont");
    myfont1.setPointSize(width / 3840.0 * 40.0);
    QFont myfont2("MyFont");
    myfont2.setPointSize(width / 3840.0 * 80.0);
    QFont myfont3("MyFont");
    myfont3.setPointSize(width / 3840.0 * 200.0);
    ui->labelAmountEnemyShips->setFont(myfont2);
    ui->labelAmountPlayerShips->setFont(myfont2);
    ui->labelEnemyShips->setFont(myfont1);
    ui->labelYourShips->setFont(myfont1);
    ui->labelEndGame->setFont(myfont3);

    ui->labelEndGame->hide();

    exitButtonSpacer = new QSpacerItem(width, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    exitButton = new ExitButton(this);
    ui->horizontalLayoutTopRow->addItem(exitButtonSpacer);
    ui->horizontalLayoutTopRow->addWidget(exitButton);
    exitButton->setSize(128.0 * width / 3840.0);

    for (size_t y = 0; y < Configuration::gridSize; y++) {
        for (size_t x = 0; x < Configuration::gridSize; x++) {
            Tile *playerTile = new Tile(this, calculateTileColor(playerArea->length(), y), true, areaWidth/Configuration::gridSize);
            ui->gridLayoutPlayerArea->addWidget(playerTile, y, x);
            playerArea->append(playerTile);

            Tile *enemyTile = new Tile(this, calculateTileColor(playerArea->length(), y), false, areaWidth/Configuration::gridSize);
            ui->gridLayoutEnemyArea->addWidget(enemyTile, y, x);
            enemyArea->append(enemyTile);
            connect(enemyTile, &Tile::clicked, enemyTile, &Tile::bombThisTile);
        }
    }

    game = new Game(this, playerArea, enemyArea, Configuration::gridSize);

    opponent = new Opponent(this, playerArea, Configuration::gridSize);

    for (Tile *tile : *playerArea) {
        connect(tile, &Tile::shipWasSunk, game, &Game::scorePoints);
    }
    for (Tile *tile : *enemyArea) {
        connect(tile, &Tile::shipWasSunk, game, &Game::scorePoints);
        connect(tile, &Tile::playerTurnEnds, opponent, &Opponent::bombATile);
    }

    connect(game, &Game::updateLabels, this, &MainWindow::updateLabels);



    buttonNewGame = new NewGameButton(this);
    ui->gridLayoutCenter->addWidget(buttonNewGame, 1, 1);
    buttonNewGame->setSize(300 * height / 1080.0, 150 * height / 1080.0);

    connect(buttonNewGame, &NewGameButton::animationFinished, this, &MainWindow::startGame);

    game->syncScore();
    connect(game, &Game::endOfGame, this, &MainWindow::endOfGame);
    connect(exitButton, &ExitButton::animationFinished, this, &MainWindow::close);

    hideEverything();
}

void MainWindow::endOfGameSlot() {
    endOfGame(true);
    ui->labelEndGame->hide();
    ui->logo->show();
}

void MainWindow::endOfGame(bool lost) {
    ui->labelEndGame->show();
    if (!lost) {
        ui->labelEndGame->setText("YOU WON");
    }else {
        ui->labelEndGame->setText("YOU LOST");
    }
    buttonNewGame->show();
    hideEverything();
    connect(buttonNewGame, &NewGameButton::animationFinished, this, &MainWindow::newGame);
    disconnect(exitButton, &ExitButton::animationFinished, this, &MainWindow::endOfGameSlot);
    connect(exitButton, &ExitButton::animationFinished, this, &MainWindow::close);
}

void MainWindow::newGame() {
    disconnect(buttonNewGame, &NewGameButton::animationFinished, this, &MainWindow::newGame);
    connect(exitButton, &ExitButton::animationFinished, this, &MainWindow::endOfGameSlot);
    disconnect(exitButton, &ExitButton::animationFinished, this, &MainWindow::close);
    ui->logo->hide();
    ui->widgetContainerPlayer->show();
    ui->widgetContainerEnemy->show();
    ui->labelAmountEnemyShips->show();
    ui->labelAmountPlayerShips->show();
    ui->labelEnemyShips->show();
    ui->labelYourShips->show();
    ui->labelEndGame->hide();
    buttonNewGame->hide();
    for (Tile *tile : *playerArea) {
        disconnect(tile, &Tile::shipWasSunk, game, &Game::scorePoints);
        delete(tile);
    }
    for (Tile *tile : *enemyArea) {
        disconnect(tile, &Tile::shipWasSunk, game, &Game::scorePoints);
        disconnect(tile, &Tile::clicked, tile, &Tile::bombThisTile);
        disconnect(tile, &Tile::playerTurnEnds, opponent, &Opponent::bombATile);
        delete(tile);
    }
    enemyArea->clear();
    playerArea->clear();
    delete(opponent);
    disconnect(game, &Game::updateLabels, this, &MainWindow::updateLabels);
    disconnect(game, &Game::endOfGame, this, &MainWindow::endOfGame);
    delete(game);

    for (size_t y = 0; y < Configuration::gridSize; y++) {
        for (size_t x = 0; x < Configuration::gridSize; x++) {
            Tile *playerTile = new Tile(this, calculateTileColor(playerArea->length(), y), true, areaWidth/Configuration::gridSize);
            ui->gridLayoutPlayerArea->addWidget(playerTile, y, x);
            playerArea->append(playerTile);

            Tile *enemyTile = new Tile(this, calculateTileColor(playerArea->length(), y), false, areaWidth/Configuration::gridSize);
            ui->gridLayoutEnemyArea->addWidget(enemyTile, y, x);
            enemyArea->append(enemyTile);
            connect(enemyTile, &Tile::clicked, enemyTile, &Tile::bombThisTile);
        }
    }

    game = new Game(this, playerArea, enemyArea, Configuration::gridSize);

    opponent = new Opponent(this, playerArea, Configuration::gridSize);

    for (Tile *tile : *playerArea) {
        connect(tile, &Tile::shipWasSunk, game, &Game::scorePoints);
    }
    for (Tile *tile : *enemyArea) {
        connect(tile, &Tile::shipWasSunk, game, &Game::scorePoints);
        connect(tile, &Tile::playerTurnEnds, opponent, &Opponent::bombATile);
    }

    connect(game, &Game::updateLabels, this, &MainWindow::updateLabels);
    connect(game, &Game::endOfGame, this, &MainWindow::endOfGame);

    game->syncScore();
}

void MainWindow::hideEverything() {
    ui->widgetContainerPlayer->hide();
    ui->widgetContainerEnemy->hide();
    ui->labelAmountEnemyShips->hide();
    ui->labelAmountPlayerShips->hide();
    ui->labelEnemyShips->hide();
    ui->labelYourShips->hide();
}

void MainWindow::startGame() {
    ui->logo->hide();
    ui->widgetContainerPlayer->show();
    ui->widgetContainerEnemy->show();
    ui->labelAmountEnemyShips->show();
    ui->labelAmountPlayerShips->show();
    ui->labelEnemyShips->show();
    ui->labelYourShips->show();
    buttonNewGame->hide();
    disconnect(buttonNewGame, &NewGameButton::animationFinished, this, &MainWindow::startGame);
    disconnect(exitButton, &ExitButton::animationFinished, this, &MainWindow::close);
    connect(exitButton, &ExitButton::animationFinished, this, &MainWindow::endOfGameSlot);
}

void MainWindow::updateLabels(bool wasPlayerShip, size_t amount) {
    if (wasPlayerShip) {
        ui->labelAmountPlayerShips->setText(QString::number(amount));
    } else {
        ui->labelAmountEnemyShips->setText(QString::number(amount));
    }
}


MainWindow::~MainWindow()
{
    delete(playerArea);
    delete(enemyArea);
    delete ui;
}

