#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene)
{
    ui->setupUi(this);

    scene->addText("Hello, world!");
}

MainWindow::~MainWindow()
{
    delete scene;
    delete ui;
}
