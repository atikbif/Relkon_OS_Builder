#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonInpPath_clicked()
{

}

void MainWindow::on_pushButtonOutPath_clicked()
{

}

void MainWindow::on_pushButtonAddBuild_clicked()
{

}

void MainWindow::on_pushButtonDelBuild_clicked()
{

}

void MainWindow::on_pushButtonEditBuild_clicked()
{

}

void MainWindow::on_pushButtonAddPLC_clicked()
{

}

void MainWindow::on_pushButtonDelPLC_clicked()
{

}

void MainWindow::on_pushButtonEditPLC_clicked()
{

}

void MainWindow::on_pushButtonBuildAll_clicked()
{

}
