#ifndef NEWGAMEBUTTON_H
#define NEWGAMEBUTTON_H

#include <QWidget>
#include <QSound>
#include <QFont>
#include <QFontDatabase>
#include <QtConcurrent>

namespace Ui {
class NewGameButton;
}

/**
 * @brief The NewGameButton class. Menu Button for the main menu
 */
class NewGameButton : public QWidget
{
    Q_OBJECT

public:

    /**
     * @brief New Game Button. Menu Button for the main menu
     * @param parent. parent object
     */
    explicit NewGameButton(QWidget *parent = nullptr);

    /**
     * @brief sets the size of the button
     * @param width. widht of the button
     * @param height. height of the button
     */
    void setSize(float width, float height);

    virtual ~NewGameButton();

public slots:

    /**
     * @brief event handler which animates the button on click
     */
    void animateButton();

signals:

    /**
     * @brief clicked signal.
     */
    void clicked();

    /**
     * @brief animation finished signal
     */
    void animationFinished();

private:

    /**
     * @brief event handler which turns this widget into a button
     * @param event
     */
    void mousePressEvent(QMouseEvent* event);

    Ui::NewGameButton *ui;
    QSound *sound;
};

#endif // NEWGAMEBUTTON_H
