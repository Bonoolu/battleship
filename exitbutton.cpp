#include "exitbutton.h"
#include "ui_exitbutton.h"

ExitButton::ExitButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExitButton)
{
    ui->setupUi(this);

    ui->shadow->hide();

    sound = new QSound(":/assets/click.wav", this);
    connect(this, &ExitButton::clicked, this, &ExitButton::animateButton);

}

void ExitButton::setSize(float width)
{
    this->setFixedWidth(width);
    this->setFixedHeight(width);

    ui->background->setFixedWidth(width);
    ui->background->setFixedHeight(width);
    ui->shadow->setFixedWidth(width);
    ui->shadow->setFixedHeight(width);
}

void ExitButton::animateButton(){
    QtConcurrent::run([] (QLabel *shadow, ExitButton *mb, QSound* sound) {
        shadow->show();
        sound->play();
        QThread::usleep(200000);
        shadow->hide();
        emit mb->animationFinished();
    }, ui->shadow, this, sound);
}

void ExitButton::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}

ExitButton::~ExitButton()
{
    delete ui;
}
