#include "tile.h"
#include "ui_tile.h"

#include "configuration.h"

Tile::Tile(QWidget *parent, QString tileColor, bool playerTile, size_t width) :
    QWidget(parent),
    ui(new Ui::Tile),
    m_tileColor(tileColor),
    m_playerTile(playerTile),
    m_width(width)
{
    ui->setupUi(this);
    setSize();
    setBlank();
    soundExplosion = new QSound(":/assets/explosion.wav", this);
}

void Tile::setSize()
{
    this->setFixedWidth(m_width);
    this->setFixedHeight(m_width);
    ui->background->setFixedWidth(m_width);
    ui->background->setFixedHeight(m_width);
}

void Tile::overlayGreyCross() {
    QPixmap pixmap = ui->background->pixmap(Qt::ReturnByValue);
    QPainter painter(&pixmap);
    painter.drawPixmap(0, 0, QPixmap(":/assets/grey_cross.png").scaled(m_width, m_width));
    painter.end();
    ui->background->setPixmap(pixmap);
}

void Tile::overlayRedCross() {
    QPixmap pixmap = ui->background->pixmap(Qt::ReturnByValue);
    QPainter painter(&pixmap);
    painter.drawPixmap(0, 0, QPixmap(":/assets/red_cross.png").scaled(m_width, m_width));
    painter.end();
    ui->background->setPixmap(pixmap);
}

void Tile::setBlank() {
    QPixmap blank(QSize(m_width, m_width));
    blank.fill(m_tileColor);
    ui->background->setPixmap(blank);
}

void Tile::hideShipTile() {
    if (isOccupied() && isShipVisible) {
        hiddenShip = ui->background->pixmap(Qt::ReturnByValue);
        isShipVisible = false;
        setBlank();
    }
}

void Tile::revealShipTile() {
    if (isOccupied() && !isShipVisible) {
        ui->background->setPixmap(hiddenShip);
        overlayRedCross();
        isShipVisible = true;
    }
}

void Tile::setMiddle(Orientation orientation, Tile* shipHeadTile, Tile* nextShipTile, bool hidden) {
    QPixmap pixmap = ui->background->pixmap(Qt::ReturnByValue);
    QPainter painter(&pixmap);
    painter.translate(m_width/2.0, m_width/2.0);
    if (orientation == horizontal) painter.rotate(90.0);
    painter.translate(m_width/-2.0, m_width/-2.0);
    painter.drawPixmap(0, 0, QPixmap(":/assets/middle.png").scaled(m_width, m_width));
    painter.end();
    ui->background->setPixmap(pixmap);
    setOccupied();
    m_shipHeadTile = shipHeadTile;
    m_nextShipTile = nextShipTile;
    if ((!Configuration::cheats) && hidden) hideShipTile();
}

void Tile::setHeadTail(Orientation orientation, HeadTail headTail, Tile* shipHeadTile, Tile* nextShipTile, bool hidden) {
    QPixmap pixmap = ui->background->pixmap(Qt::ReturnByValue);
    QPainter painter(&pixmap);
    painter.translate(m_width/2.0, m_width/2.0);
    if (orientation == Tile::horizontal && headTail == Tile::head) {
        painter.rotate(270.0);
    }
    else if (orientation == Tile::horizontal && headTail == Tile::tail){
        painter.rotate(90.0);
    }
    else if (orientation == Tile::vertical && headTail == Tile::tail){
        painter.rotate(180.0);
    }
    painter.translate(m_width/-2.0, m_width/-2.0);
    painter.drawPixmap(0, 0, QPixmap(":/assets/headTail.png").scaled(m_width, m_width));
    painter.end();
    ui->background->setPixmap(pixmap);
    setOccupied();
    m_shipHeadTile = shipHeadTile;
    m_nextShipTile = nextShipTile;
    if ((!Configuration::cheats) && hidden) hideShipTile();
}

bool Tile::isShipSunk() {
    if (m_shipHeadTile) {
        Tile *tile = m_shipHeadTile;
        while(tile){
            if (!tile->isBombed()) return false;
            tile = tile->getNextShipTile();
        }
        return true;
    }
    qWarning() << "This is not a ship tile! Why are you checking if there is a sunken ship?";
    return false;
}

void Tile::bombThisTile() {
    setBombed();
    if(isOccupied()){
        overlayRedCross();
        if (isShipSunk()) {
            Tile *tile = m_shipHeadTile;
            while(tile){
                tile->revealShipTile();
                tile = tile->getNextShipTile();
            }
            QtConcurrent::run([] (QSound* sound) {
                sound->play();
            }, soundExplosion);
            emit shipWasSunk(m_playerTile);
        }
    } else {
        overlayGreyCross();
    }
    emit playerTurnEnds(m_playerTile);
    disconnect(this, &Tile::clicked, this, &Tile::bombThisTile);
}

Tile* Tile::getNextShipTile() {
    return m_nextShipTile;
}

bool Tile::isOccupied() {
    return m_occupied;
}

bool Tile::isBombed() {
    return m_bombed;
}

void Tile::setOccupied(bool occupied) {
    m_occupied = occupied;
}

void Tile::setBombed(bool bombed) {
    m_bombed = bombed;
}

bool Tile::isPlayerTile() {
    return m_playerTile;
}

void Tile::mousePressEvent(QMouseEvent* event) {

    emit clicked();
}
