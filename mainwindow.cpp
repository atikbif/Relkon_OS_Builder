#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::updateGUI(const Settings &conf)
{
    Q_UNUSED(conf)
}

void MainWindow::updateSettings(Settings &conf)
{
    Q_UNUSED(conf)
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateGUI(Settings::getInstance());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonInpPath_clicked()
{
    updateSettings(Settings::getInstance());
}

void MainWindow::on_pushButtonOutPath_clicked()
{
    updateSettings(Settings::getInstance());
}

void MainWindow::on_pushButtonAddBuild_clicked()
{
    updateSettings(Settings::getInstance());
}

void MainWindow::on_pushButtonDelBuild_clicked()
{
    updateSettings(Settings::getInstance());
}

void MainWindow::on_pushButtonEditBuild_clicked()
{
    updateSettings(Settings::getInstance());
}

void MainWindow::on_pushButtonAddPLC_clicked()
{
    updateSettings(Settings::getInstance());
}

void MainWindow::on_pushButtonDelPLC_clicked()
{
    updateSettings(Settings::getInstance());
}

void MainWindow::on_pushButtonEditPLC_clicked()
{
    updateSettings(Settings::getInstance());
}

void MainWindow::on_pushButtonBuildAll_clicked()
{

}
