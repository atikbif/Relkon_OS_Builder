#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    void updateGUI(const Settings &conf);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonInpPath_clicked();

    void on_pushButtonOutPath_clicked();

    void on_pushButtonAddBuild_clicked();

    void on_pushButtonDelBuild_clicked();

    void on_pushButtonEditBuild_clicked();

    void on_pushButtonAddPLC_clicked();

    void on_pushButtonDelPLC_clicked();

    void on_pushButtonEditPLC_clicked();

    void on_pushButtonBuildAll_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
