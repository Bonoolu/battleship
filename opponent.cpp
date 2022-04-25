#include "opponent.h"

#define tileAt(x, y) m_playerArea->at(((y) * m_gridSize) + (x))

Opponent::Opponent(QWidget *parent, QList<Tile*> *playerArea, size_t gridSize) :
    QObject(parent),
    m_playerArea(playerArea),
    m_gridSize(gridSize)
{

}

void Opponent::bombATile() {
    for(Position pos : *m_bombedPlayerShipTiles) {
        if (!tileAt(pos.x, pos.y)->isShipSunk()){
            if (tryToSinkShip(pos.x, pos.y))
                return;
        }
    }
    bombRandomPlayerTile();
}

void Opponent::bombRandomPlayerTile() {
    size_t x = QRandomGenerator::global()->bounded(static_cast<int>(m_gridSize));
    size_t y = QRandomGenerator::global()->bounded(static_cast<int>(m_gridSize));
    Tile *tile = tileAt(x ,y);
    if (tile->isBombed()) {
        bombRandomPlayerTile();
    }
    else {
        tile->bombThisTile();
        if (tile->isOccupied()) {
            m_bombedPlayerShipTiles->append(Position({x, y}));
        }
    }
}

bool Opponent::tryToSinkShip(size_t x, size_t y) {
    size_t x_toBomb, y_toBomb;
    if (x > 0 && !tileAt(x - 1, y)->isBombed()) {
        x_toBomb = x - 1;
        y_toBomb = y;
    } else if ( x < m_gridSize - 1 && !tileAt(x + 1, y)->isBombed()) {
        x_toBomb = x + 1;
        y_toBomb = y;
    } else if ( y > 0 && !tileAt(x, y - 1)->isBombed()) {
        x_toBomb = x;
        y_toBomb = y - 1;
    } else if ( y < m_gridSize - 1 && !tileAt(x, y + 1)->isBombed()) {
        x_toBomb = x;
        y_toBomb = y + 1;
    } else {
        return false;
    }
    tileAt(x_toBomb, y_toBomb)->bombThisTile();
    if (tileAt(x_toBomb, y_toBomb)->isOccupied()) {
        m_bombedPlayerShipTiles->append(Position({x_toBomb, y_toBomb}));
    }
    return true;
}

Opponent::~Opponent() {
    delete(m_bombedPlayerShipTiles);
}
