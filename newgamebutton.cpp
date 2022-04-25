#include "newgamebutton.h"
#include "ui_newgamebutton.h"

NewGameButton::NewGameButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewGameButton)
{
    ui->setupUi(this);

    ui->shadow->hide();

    sound = new QSound(":/assets/click.wav", this);
    connect(this, &NewGameButton::clicked, this, &NewGameButton::animateButton);

}

void NewGameButton::setSize(float width, float height)
{
    this->setFixedWidth(width);
    this->setFixedHeight(height);

    ui->background->setFixedWidth(width);
    ui->background->setFixedHeight(height);
    ui->shadow->setFixedWidth(width);
    ui->shadow->setFixedHeight(height);

}

void NewGameButton::animateButton(){
    QtConcurrent::run([] (QLabel *shadow, NewGameButton *mb, QSound* sound) {
        shadow->show();
        sound->play();
        QThread::usleep(200000);
        shadow->hide();
        emit mb->animationFinished();
    }, ui->shadow, this, sound);
}

void NewGameButton::mousePressEvent(QMouseEvent* event) {

    emit clicked();
}

NewGameButton::~NewGameButton()
{
    delete ui;
}
