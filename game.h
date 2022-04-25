#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QList>
#include <QRandomGenerator>
#include <QDebug>

#include "configuration.h"
#include "tile.h"

/**
 * @brief The Game class. contains the logic for the battleship game
 */
class Game : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief Game. contains the logic for the battleship game
     * @param parent. parent object
     * @param playerArea. list of player tiles
     * @param enemyArea. list of enemy tiles
     * @param gridSize. size of the play area
     */
    Game(QWidget *parent = nullptr, QList<Tile*> *playerArea = nullptr, QList<Tile*> *enemyArea = nullptr, size_t gridSize = 0);

    /**
     * @brief placeShip. trys to places a ship on the area
     * @param area. area to place ship on
     * @param x. x coordinate of the tile where the ship is placed
     * @param y. y coordinate of the tile where the ship is placed
     * @param length. length of the ship
     * @param hidden. true if the ship is supposed to be hidden (enemy area)
     * @return returns true if the ship was placed. returns false if the ship would be out of bounds or if it would overlap another ship
     */
    bool placeShip(QList<Tile*> *area, int x, int y, size_t length, Tile::Orientation, bool hidden);

    /**
     * @brief automaticallyPlaceShips. automatically places ships on to an area
     * @param area. the area to place ships on
     * @param hidden. true if the ships are supposed to be hidden (enemy area)
     * @return returns true if there was enough space to place all ships
     */
    bool automaticallyPlaceShips(QList<Tile*> *area, bool hidden);

    /**
     * @brief syncScore. synchronises the player and enemy scores with the MainWindow class
     */
    void syncScore();

    virtual ~Game() = default;

public slots:

    /**
     * @brief scorePoints. event handler for if a ship was sunk, increases the score
     * @param wasPlayerShip, true if a player ship was sunk, false if enemy
     */
    void scorePoints(bool wasPlayerShip);

signals:

    /**
     * @brief updateLabels. signal to be emitted if MainWindow should update the score labels
     * @param wasPlayerShip. true if a player ship was sunk, false if enemy
     * @param amount. score to set in the label
     */
    void updateLabels(bool wasPlayerShip, size_t amount);

    /**
     * @brief endOfGame. end of game signal
     * @param playerLost. true if the player lost, false if the player won
     */
    void endOfGame(bool playerLost);

private:
    QList<Tile*> *m_playerArea;
    QList<Tile*> *m_enemyArea;
    size_t m_gridSize;
    size_t amountShips;
    size_t amountPlayerShips = 9;
    size_t amountEnemyShips = 9;
    bool isGameRunning = true;
};

#endif // GAME_H
