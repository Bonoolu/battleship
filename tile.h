#ifndef TILE_H
#define TILE_H

#include <QWidget>
#include <QThread>
#include <QPainter>
#include <QSound>
#include <QtConcurrent>

namespace Ui {
class Tile;
}

/**
 * @brief The Tile class. clickable Tile which can show ships and bombs
 */
class Tile : public QWidget
{
    Q_OBJECT

public:

    /**
     * @brief Tile. clickable Tile which can show ships and bombs
     * @param parent. parent object
     * @param tileColor. background color of the tile
     * @param playerTile. true if this is a player tile, false if it is an enemy tile
     * @param width. width and height of the tile
     */
    explicit Tile(QWidget *parent = nullptr, QString tileColor = "#185cb1", bool playerTile = true, size_t width = 0);

    /**
     * @brief The Orientation enum. options are "horizontal" and "vertical"
     */
    enum Orientation {
        horizontal,
        vertical
    };

    /**
     * @brief The HeadTail enum. options are "head" and "tail"
     */
    enum HeadTail {
        head,
        tail
    };

    /**
     * @brief setSize. set size of this widget
     */
    void setSize();

    /**
     * @brief setBlank. fill the widget with the background color
     */
    void setBlank();

    /**
     * @brief setMiddle. set a middle ship tile
     * @param orientation. orientation of the tile
     * @param shipHeadTile. pointer to the head of the ship
     * @param nextShipTile. linked list to the next ship tile in the diretion of the tail
     * @param hidden. true if the ship is hidden (enemy tile)
     */
    void setMiddle(Orientation orientation, Tile* shipHeadTile, Tile* nextShipTile, bool hidden);

    /**
     * @brief setHeadTail. sets a head ot tail ship tile
     * @param orientation. orientation of the tile
     * @param headTail. head if head tile, tail if tail tile
     * @param shipHeadTile. pointer to the head of the ship
     * @param nextShipTile. linked list to the next ship tile in the diretion of the tail
     * @param hidden. true if the ship is hidden (enemy tile)
     */
    void setHeadTail(Orientation orientation, HeadTail headTail, Tile* shipHeadTile, Tile* nextShipTile, bool hidden);

    /**
     * @brief overlayGreyCross. overlays a grey cross onto the tile
     */
    void overlayGreyCross();

    /**
     * @brief overlayRedCross. overlays a red cross onto the tile
     */
    void overlayRedCross();

    /**
     * @brief isOccupied. returns true if here is a ship at this tile
     * @return returns true if here is a ship at this tile
     */
    bool isOccupied();

    /**
     * @brief isBombed. returns true if this tile was bombed
     * @return returns true if this tile was bombed
     */
    bool isBombed();

    /**
     * @brief isShipSunk. returns true if this tile contains a sunken ship tile
     * @return returns true if this tile contains a sunken ship tile
     */
    bool isShipSunk();

    /**
     * @brief getNextShipTile. returns next ship tile. this is a linked list
     * @return returns next ship tile. this is a linked list
     */
    Tile* getNextShipTile();

    /**
     * @brief bombThisTile. bomb this tile
     */
    void bombThisTile();

    /**
     * @brief revealShipTile. reveals ship tile which was hidden with hideShipTile()
     */
    void revealShipTile();

    /**
     * @brief isPlayerTile. returns true if this is a player tile, false if enemy
     * @return returns true if this is a player tile, false if enemy
     */
    bool isPlayerTile();

    virtual ~Tile() = default;

signals:

    /**
     * @brief clicked signal.
     */
    void clicked();

    /**
     * @brief shipWasSunk. signal to be emitted if a ship was sunk
     * @param playerTile. true if a player ship was sunk, false if enemy.
     */
    void shipWasSunk(bool playerTile);

    /**
     * @brief playerTurnEnds. signal to be emitted if the player turn ends
     * @param playerTile. false if this was emmited by an enemyTile. this signal is actually ignored if emmitted by an playerTile
     */
    void playerTurnEnds(bool playerTile);

private:

    /**
     * @brief event handler which turns this widget into a button
     * @param event
     */
    void mousePressEvent(QMouseEvent* event);

    /**
     * @brief setOccupied. sets occupied status of this tile
     * @param occupied, true if there is a ship, false if not
     */
    void setOccupied(bool occupied = true);

    /**
     * @brief setBombed. sets the bombed status of this tile
     * @param bombed. true if this tile is bombed, false if not
     */
    void setBombed(bool bombed = true);

    /**
     * @brief hideShipTile. hides the ship tile and fills this tile with the background color. ship can be revealed again with revealShipTile()
     */
    void hideShipTile();

    Ui::Tile *ui;
    QString m_tileColor;
    bool m_occupied = false;
    bool m_bombed = false;
    Tile* m_nextShipTile = nullptr;
    Tile* m_shipHeadTile = nullptr;
    QPixmap hiddenShip;
    bool isShipVisible = true;
    const bool m_playerTile;
    size_t m_width;
    //QSound *soundWater;
    QSound *soundExplosion;
};

#endif // TILE_H
