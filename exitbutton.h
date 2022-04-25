#ifndef EXITBUTTON_H
#define EXITBUTTON_H

#include <QWidget>
#include <QSound>
#include <QFont>
#include <QFontDatabase>
#include <QtConcurrent>

namespace Ui {
class ExitButton;
}

/**
 * @brief The ExitButton class. Exit Button shown in the top right corner
 */
class ExitButton : public QWidget
{
    Q_OBJECT

public:

    /**
     * @brief Exit Button. Button for returning to main Menu or for quitting the Game
     * @param parent. parent object
     */
    explicit ExitButton(QWidget *parent = nullptr);

    /**
     * @brief sets the size of the button
     * @param width. width and height of the button
     */
    void setSize(float width);

    virtual ~ExitButton();

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

    Ui::ExitButton *ui;
    QSound *sound;
};

#endif // EXITBUTTON_H
