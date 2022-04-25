#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QScreen>
#include <QSpacerItem>
#include <QFontDatabase>

#include "tile.h"
#include "game.h"
#include "opponent.h"
#include "newgamebutton.h"
#include "exitbutton.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /**
     * @brief MainWindow. Main Window
     * @param parent. parent object
     */
    MainWindow(QWidget *parent = nullptr);

    virtual ~MainWindow();

public slots:

    /**
     * @brief updateLabels. event handler for updateing the scores in the labels
     * @param wasPlayerShip. true if a playership was sunk, false if enemy
     * @param amount. score to set in the label
     */
    void updateLabels(bool wasPlayerShip, size_t amount);

    /**
     * @brief hideEverything. hides the playing area and scores. only to be called before the game starts for the first time
     */
    void hideEverything();

    /**
     * @brief startGame. event handler for the new game button. only to be called after the game starts for the first time
     */
    void startGame();

    /**
     * @brief endOfGameSlot. event handler for if the game is cancelled. helper function, calls endOfGameSlot()
     */
    void endOfGameSlot();

    /**
     * @brief newGame. event handler for if a new game starts
     */
    void newGame();

private:

    /**
     * @brief endOfGame. event handler for if the game ends.
     * @param lost. false if player won, true if enemt won
     */
    void endOfGame(bool lost);

    Game *game;
    QList<Tile*> *playerArea;
    QList<Tile*> *enemyArea;
    Ui::MainWindow *ui;
    size_t m_gridSize;
    Opponent *opponent = nullptr;
    NewGameButton *buttonNewGame;
    size_t areaWidth;
    ExitButton *exitButton;
    QSpacerItem  *exitButtonSpacer;
};
#endif // MAINWINDOW_H
