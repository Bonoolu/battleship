#ifndef OPPONENT_H
#define OPPONENT_H

#include <QObject>
#include <QRandomGenerator>

#include "tile.h"

/**
 * @brief The Opponent class. Contains the logic for the computer opponent
 */
class Opponent : public QObject
{
public:

    /**
     * @brief Opponent class. Contains the logic for the computer opponent
     * @param parent. parent object
     * @param playerArea. list which contains the player Area tiles
     * @param gridSize. size of the area
     */
    Opponent(QWidget *parent = nullptr,QList<Tile*> *playerArea = nullptr, size_t gridSize = 0);

    virtual ~Opponent();

public slots:

    /**
     * @brief bombATile. bombs a tile on the player area. the computer doesn't cheat
     */
    void bombATile();

private:

    /**
     * @brief The Position struct. contains coordinates
     */
    struct Position {
        size_t x;
        size_t y;
    };

    /**
     * @brief bombRandomPlayerTile. bombs a random player tile
     */
    void bombRandomPlayerTile();

    /**
     * @brief tryToSinkShip. trys to find ship tiles next to ship tiles which were already bombed. this method is not aware of the shapes of the ships
     * @param x. x coordinate of the ship tile which was already bombed
     * @param y. y coordinate of the ship tile which was already bombed
     * @return returns false if all 4 neighboring tiles were already bombed
     */
    bool tryToSinkShip(size_t x, size_t y);

    QList<Tile*> *m_playerArea;
    QList<Position> *m_bombedPlayerShipTiles = new QList<Position>;
    size_t m_gridSize;
};

#endif // OPPONENT_H
