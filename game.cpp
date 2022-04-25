#include "game.h"

#define tileAt(area, x, y) area->at((y * m_gridSize) + x)

Game::Game(QWidget *parent, QList<Tile*> *playerArea, QList<Tile*> *enemyArea, size_t gridSize)
    : QObject(parent),
      m_playerArea(playerArea),
      m_enemyArea(enemyArea),
      m_gridSize(gridSize)
{
    automaticallyPlaceShips(m_enemyArea, true);
    automaticallyPlaceShips(m_playerArea, false);

    amountPlayerShips = 0;
    amountEnemyShips = 0;
    for(size_t length = Configuration::minShipLength; length <= Configuration::maxShipLength; length++) {
        amountPlayerShips += Configuration::amountShips[length - 1];
        amountEnemyShips += Configuration::amountShips[length - 1];
    }
}

bool Game::automaticallyPlaceShips(QList<Tile*> *area, bool hidden) {
    int tries = 0; // recursion terminates if tries is above 1000

    for(size_t length = Configuration::minShipLength; length <= Configuration::maxShipLength; length++) {
        for(size_t amount = 0; amount < Configuration::amountShips[length - 1]; amount++) {
            size_t x = QRandomGenerator::global()->bounded(static_cast<int>(m_gridSize));
            size_t y = QRandomGenerator::global()->bounded(static_cast<int>(m_gridSize));
            Tile::Orientation orientation = (QRandomGenerator::global()->generate() % 2) ? Tile::horizontal : Tile::vertical;
            if (!placeShip(area, x, y, length, orientation, hidden)) amount--;

            tries++;
            if (tries > 1000) {
                qWarning() << "Ship placment failed and still incomplete! Tried over a 1000 times to place more ships. Please change the configuration!";
                return false;
            }
        }
    }
    return true;
}

void Game::syncScore() {
    emit updateLabels(true, amountPlayerShips);
    emit updateLabels(false, amountEnemyShips);
}

void Game::scorePoints(bool wasPlayerShip) {
    if (!isGameRunning) return;
    if (wasPlayerShip) {
        amountPlayerShips--;
        emit updateLabels(wasPlayerShip, amountPlayerShips);
    } else {
        amountEnemyShips--;
        emit updateLabels(wasPlayerShip, amountEnemyShips);
    }
    if (amountPlayerShips == 0) {
        emit endOfGame(true);
        isGameRunning = false;
    }else if (amountEnemyShips == 0) {
        emit endOfGame(false);
        isGameRunning = false;
    }
}

bool Game::placeShip(QList<Tile*> *area, int x, int y, size_t length, Tile::Orientation orientation, bool hidden) {
    int *coord;
    if (orientation == Tile::horizontal) {
        coord = &x;
    }else {
        coord = &y;
    }
    if (*coord + length >= m_gridSize) {
        return false;
    }
    QList<Tile*> shipTiles;
    for (size_t i = 0; i < length; i++) {
        if (tileAt(area, x, y)->isOccupied()) {
            return false;
        }
        shipTiles.append(tileAt(area, x, y));
        (*coord)++;
    }
    int i = 0;
    Tile* shipHeadTile = shipTiles.at(0);
    shipHeadTile->setHeadTail(orientation, Tile::head, shipHeadTile, shipTiles.at(1), hidden);
    i++;
    for (; i < shipTiles.length() - 1; i++) {
        shipTiles.at(i)->setMiddle(orientation, shipHeadTile, shipTiles.at(i + 1), hidden);
    }
    shipTiles.at(i)->setHeadTail(orientation, Tile::tail, shipHeadTile, nullptr, hidden);
    return true;
}


