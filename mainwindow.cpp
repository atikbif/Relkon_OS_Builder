#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QComboBox>

#include "gitmanager.h"
#include "branchbuilder.h"


void MainWindow::updateGUI(const Settings &conf)
{
    ui->lineEditInpPath->setText(conf.getInstance().getSourceDirPath());
    ui->lineEditOutPath->setText(conf.getInstance().getOutDirPath());
    ui->listWidgetBuild->clear();
    QStringList branches = conf.getInstance().getBranches();
    ui->listWidgetBuild->addItems(branches);
    ui->tableWidgetPLC->clear();
    QStringList plcNames = conf.getInstance().getControllers();
    ui->tableWidgetPLC->setColumnCount(2);
    ui->tableWidgetPLC->setHorizontalHeaderLabels(QStringList()<<"контроллер"<<"сборка");
    ui->tableWidgetPLC->setRowCount(plcNames.count());
    int i=0;
    foreach (QString plc, plcNames) {
        QString branch = conf.getInstance().getControllerBranch(plc);
        QTableWidgetItem* itemPLC = new QTableWidgetItem(plc);
        QTableWidgetItem* itemBranch = new QTableWidgetItem(branch);
        ui->tableWidgetPLC->setItem(i,0,itemPLC);
        ui->tableWidgetPLC->setItem(i,1,itemBranch);
        i++;
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidgetPLC->setSelectionBehavior(QAbstractItemView::SelectRows); //to select a row
    ui->tableWidgetPLC->setSelectionMode(QAbstractItemView::SingleSelection); //to have single selection.
    ui->tableWidgetPLC->setSortingEnabled(true);
    updateGUI(Settings::getInstance());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonInpPath_clicked()
{
    QString curDir = Settings::getInstance().getSourceDirPath();
    QString dir = QFileDialog::getExistingDirectory(this, "Выбрать каталог", curDir,
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty()) {
        ui->lineEditInpPath->setText(dir);
        Settings::getInstance().setSourceDir(ui->lineEditInpPath->text());
    }
}

void MainWindow::on_pushButtonOutPath_clicked()
{
    QString curDir = Settings::getInstance().getOutDirPath();
    QString dir = QFileDialog::getExistingDirectory(this, "Выбрать каталог", curDir,
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty()) {
        ui->lineEditOutPath->setText(dir);
        Settings::getInstance().setOutDir(ui->lineEditOutPath->text());
    }
}

void MainWindow::on_pushButtonAddBuild_clicked()
{
    QDialog *dialog = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout();
    QLineEdit *buildName = new QLineEdit();
    QLabel *label = new QLabel("название сборки");
    QDialogButtonBox *box = new QDialogButtonBox( Qt::Horizontal );
    QPushButton *buttonOK = new QPushButton("Принять");
    connect(buttonOK,SIGNAL(clicked(bool)),dialog,SLOT(accept()));
    QPushButton *buttonCancel = new QPushButton("Отменить");
    connect(buttonCancel,SIGNAL(clicked(bool)),dialog,SLOT(reject()));
    box->addButton(buttonOK, QDialogButtonBox::AcceptRole);
    box->addButton(buttonCancel, QDialogButtonBox::RejectRole);

    layout->addWidget(label);
    layout->addWidget(buildName);
    layout->addWidget(box);

    dialog->setLayout(layout);
    if(dialog->exec()==QDialog::Accepted) {
        QString branch = buildName->text();
        if(!branch.isEmpty()) {
            ui->listWidgetBuild->addItem(branch);
            Settings::getInstance().addBranch(branch);
        }
    }
}

void MainWindow::on_pushButtonDelBuild_clicked()
{
    int i = ui->listWidgetBuild->currentRow();
    if(i>=0 && i<ui->listWidgetBuild->count()) {
        QString curBranch = ui->listWidgetBuild->currentItem()->text();
        QDialog *dialog = new QDialog(this);
        QVBoxLayout *layout = new QVBoxLayout();
        QLabel *label = new QLabel("Удалить сборку " + curBranch + "?");
        QDialogButtonBox *box = new QDialogButtonBox( Qt::Horizontal );
        QPushButton *buttonDel = new QPushButton("Удалить");
        connect(buttonDel,SIGNAL(clicked(bool)),dialog,SLOT(accept()));
        QPushButton *buttonCancel = new QPushButton("Не удалять");
        connect(buttonCancel,SIGNAL(clicked(bool)),dialog,SLOT(reject()));
        box->addButton(buttonDel, QDialogButtonBox::AcceptRole);
        box->addButton(buttonCancel, QDialogButtonBox::RejectRole);

        layout->addWidget(label);
        layout->addWidget(box);

        dialog->setLayout(layout);
        if(dialog->exec()==QDialog::Accepted) {
            QListWidgetItem* item = ui->listWidgetBuild->takeItem(i);
            delete item;
            Settings::getInstance().deleteBranch(curBranch);
        }
    }else {
        QMessageBox::information(this, "", "Выберите сборку для удаления");
    }

}

void MainWindow::on_pushButtonEditBuild_clicked()
{
    int i = ui->listWidgetBuild->currentRow();
    if(i>=0 && i<ui->listWidgetBuild->count()) {
        QString curBranch = ui->listWidgetBuild->currentItem()->text();
        QDialog *dialog = new QDialog(this);
        QVBoxLayout *layout = new QVBoxLayout();
        QLabel *label = new QLabel("Сборка: ");
        QLineEdit *buildName = new QLineEdit(curBranch);
        QDialogButtonBox *box = new QDialogButtonBox( Qt::Horizontal );
        QPushButton *buttonEdit = new QPushButton("Изменить");
        connect(buttonEdit,SIGNAL(clicked(bool)),dialog,SLOT(accept()));
        QPushButton *buttonCancel = new QPushButton("Отмена");
        connect(buttonCancel,SIGNAL(clicked(bool)),dialog,SLOT(reject()));
        box->addButton(buttonEdit, QDialogButtonBox::AcceptRole);
        box->addButton(buttonCancel, QDialogButtonBox::RejectRole);

        layout->addWidget(label);
        layout->addWidget(buildName);
        layout->addWidget(box);

        dialog->setLayout(layout);
        if(dialog->exec()==QDialog::Accepted) {
            ui->listWidgetBuild->currentItem()->setText(buildName->text());
            Settings::getInstance().renameBranch(curBranch, buildName->text());
        }
    }else {
        QMessageBox::information(this, "", "Выберите сборку для редактирования");
    }
}

void MainWindow::on_pushButtonAddPLC_clicked()
{
    if(ui->listWidgetBuild->count()==0) {
        QMessageBox::information(this, "", "Отсутствуют доступные сборки");
        return;
    }
    QDialog *dialog = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout();
    QLabel *labelPLC = new QLabel("Контроллер: ");
    QLineEdit *plcName = new QLineEdit();
    QLabel *labelBranches = new QLabel("Сборки");
    QStringList branches = Settings::getInstance().getBranches();
    QComboBox *comboBox = new QComboBox();
    comboBox->addItems(branches);
    comboBox->setCurrentIndex(0);

    QDialogButtonBox *box = new QDialogButtonBox( Qt::Horizontal );
    QPushButton *buttonAdd = new QPushButton("Добавить");
    connect(buttonAdd,SIGNAL(clicked(bool)),dialog,SLOT(accept()));
    QPushButton *buttonCancel = new QPushButton("Отмена");
    connect(buttonCancel,SIGNAL(clicked(bool)),dialog,SLOT(reject()));
    box->addButton(buttonAdd, QDialogButtonBox::AcceptRole);
    box->addButton(buttonCancel, QDialogButtonBox::RejectRole);

    layout->addWidget(labelPLC);
    layout->addWidget(plcName);
    layout->addWidget(labelBranches);
    layout->addWidget(comboBox);
    layout->addWidget(box);

    dialog->setLayout(layout);
    if(dialog->exec()==QDialog::Accepted) {
        int rowCount = ui->tableWidgetPLC->rowCount();
        ui->tableWidgetPLC->insertRow(rowCount++);
        QTableWidgetItem *itemPLC = new QTableWidgetItem(plcName->text());
        QTableWidgetItem *itemBranch = new QTableWidgetItem(comboBox->currentText());
        ui->tableWidgetPLC->setItem(rowCount-1,0,itemPLC);
        ui->tableWidgetPLC->setItem(rowCount-1,1,itemBranch);
        Settings::getInstance().addController(plcName->text(),comboBox->currentText());
    }
}

void MainWindow::on_pushButtonDelPLC_clicked()
{
    int curRow = ui->tableWidgetPLC->currentRow();
    if((curRow>=0)&&(curRow<ui->tableWidgetPLC->rowCount())) {
        QString plcName = ui->tableWidgetPLC->item(curRow,0)->text();

        QDialog *dialog = new QDialog(this);
        QVBoxLayout *layout = new QVBoxLayout();
        QLabel *label = new QLabel("Удалить контроллер " + plcName + "?");
        QDialogButtonBox *box = new QDialogButtonBox( Qt::Horizontal );
        QPushButton *buttonDel = new QPushButton("Удалить");
        connect(buttonDel,SIGNAL(clicked(bool)),dialog,SLOT(accept()));
        QPushButton *buttonCancel = new QPushButton("Не удалять");
        connect(buttonCancel,SIGNAL(clicked(bool)),dialog,SLOT(reject()));
        box->addButton(buttonDel, QDialogButtonBox::AcceptRole);
        box->addButton(buttonCancel, QDialogButtonBox::RejectRole);

        layout->addWidget(label);
        layout->addWidget(box);

        dialog->setLayout(layout);
        if(dialog->exec()==QDialog::Accepted) {
            ui->tableWidgetPLC->removeRow(curRow);
            Settings::getInstance().deleteController(plcName);
        }
    }else {
        QMessageBox::information(this, "", "Выберите контроллер для удаления");
    }
}

void MainWindow::on_pushButtonEditPLC_clicked()
{
    int curRow = ui->tableWidgetPLC->currentRow();
    if((curRow>=0)&&(curRow<ui->tableWidgetPLC->rowCount())) {
        QString plc_name = ui->tableWidgetPLC->item(curRow,0)->text();
        QString branchName = ui->tableWidgetPLC->item(curRow,1)->text();

        QDialog *dialog = new QDialog(this);
        QVBoxLayout *layout = new QVBoxLayout();
        QLabel *labelPLC = new QLabel("Контроллер: ");
        QLineEdit *plcName = new QLineEdit(plc_name);
        QLabel *labelBranches = new QLabel("Сборки");
        QStringList branches = Settings::getInstance().getBranches();
        QComboBox *comboBox = new QComboBox();
        comboBox->addItems(branches);
        comboBox->setCurrentText(branchName);

        QDialogButtonBox *box = new QDialogButtonBox( Qt::Horizontal );
        QPushButton *buttonAdd = new QPushButton("Изменить");
        connect(buttonAdd,SIGNAL(clicked(bool)),dialog,SLOT(accept()));
        QPushButton *buttonCancel = new QPushButton("Отмена");
        connect(buttonCancel,SIGNAL(clicked(bool)),dialog,SLOT(reject()));
        box->addButton(buttonAdd, QDialogButtonBox::AcceptRole);
        box->addButton(buttonCancel, QDialogButtonBox::RejectRole);

        layout->addWidget(labelPLC);
        layout->addWidget(plcName);
        layout->addWidget(labelBranches);
        layout->addWidget(comboBox);
        layout->addWidget(box);

        dialog->setLayout(layout);
        if(dialog->exec()==QDialog::Accepted) {
            ui->tableWidgetPLC->item(curRow,0)->setText(plcName->text());
            ui->tableWidgetPLC->item(curRow,1)->setText(comboBox->currentText());
            Settings::getInstance().editController(plc_name,plcName->text(),comboBox->currentText());
        }
    }else {
        QMessageBox::information(this, "", "Выберите контроллер для изменения");
    }
}

void MainWindow::on_pushButtonBuildAll_clicked()
{
    ui->textBrowserLog->clear();
    ui->textBrowserLog->setTextColor(Qt::darkGreen);
    GitManager git(ui->lineEditInpPath->text());
    git.goToRepo();
    if(git.isRepoClean()) {
        ui->textBrowserLog->insertPlainText("The repository is clean\n\n");
        QStringList branches = git.getBranches();
        ui->textBrowserLog->insertPlainText("branches:\n");
        foreach (QString branch, branches) {
            ui->textBrowserLog->insertPlainText("   " + branch + "\n");
        }
        QString startRepo = git.getCurrentBranch();
        ui->textBrowserLog->insertPlainText("Current branch: " + startRepo + "\n\n");
        int brCount = ui->listWidgetBuild->count();
        for(int i=0; i<brCount; i++) {
            ui->textBrowserLog->insertPlainText("---\n");
            QString br = ui->listWidgetBuild->item(i)->text();
            if(git.goToBranch(br)) {
                ui->textBrowserLog->insertPlainText("Switched to branch " + br + "\n");

                BranchBuilder builder(ui->lineEditInpPath->text(),
                                      ui->lineEditOutPath->text(), br);
                QString res;
                if(!builder.createBuild(res)) {
                    ui->textBrowserLog->setTextColor(Qt::red);
                    ui->textBrowserLog->insertPlainText(res + "\n");
                    ui->textBrowserLog->setTextColor(Qt::darkGreen);
                }else {
                    ui->textBrowserLog->insertPlainText("Compile is complete\n");
                }
            }else {
                ui->textBrowserLog->setTextColor(Qt::red);
                ui->textBrowserLog->insertPlainText("Error! Cannot switch to branch " + br + "\n");
                ui->textBrowserLog->setTextColor(Qt::darkGreen);
            }
            ui->textBrowserLog->insertPlainText("---\n");
        }
        ui->textBrowserLog->insertPlainText("\n");
        if(git.goToBranch(startRepo)) {
            ui->textBrowserLog->insertPlainText("Switched to start branch " + startRepo + "\n");
        }else {
            ui->textBrowserLog->setTextColor(Qt::red);
            ui->textBrowserLog->insertPlainText("Error! Cannot switch to start branch " + startRepo + "\n");
            ui->textBrowserLog->setTextColor(Qt::darkGreen);
        }

    }else {
        ui->textBrowserLog->setTextColor(Qt::red);
        ui->textBrowserLog->insertPlainText("You should clean the repository (git stash)");
        ui->textBrowserLog->setTextColor(Qt::darkGreen);
    }
}
