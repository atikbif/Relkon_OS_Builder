#include "gitmanager.h"
#include <QCoreApplication>
#include <QDir>
#include <QProcess>
#include <QRegExp>
#include <QDebug>
#include <QFile>
#include <QTextStream>

const QString GitManager::cleanRepoString = "nothing";

GitManager::GitManager(const QString &repoPath):repo(repoPath)
{
    startPath = QDir::currentPath();
}

void GitManager::goToRepo()
{
    QDir::setCurrent(repo);
}

void GitManager::goToStartPath()
{
    QDir::setCurrent(startPath);
}

bool GitManager::isRepoClean()
{
    QProcess p;
    p.setProcessChannelMode(QProcess::MergedChannels);
    QString cmd = "git";
    QStringList arg;
    arg << "status";
    p.start(cmd,arg);
    p.waitForFinished();
    QString output(p.readAll());
    p.close();
    if(output.contains(cleanRepoString)) return true;
    return false;
}

QStringList GitManager::getBranches()
{
    QProcess p;
    p.setProcessChannelMode(QProcess::MergedChannels);
    QString cmd = "git branch";
    p.start(cmd);
    p.waitForFinished();
    QString output(p.readAll());
    QStringList branches = output.split("\n");
    p.close();
    for(int i=0;i<branches.count();i++) {
        QString br = branches.at(i);
        br.remove(QRegExp("[\\s\\*]"));
        branches[i] = br;
    }
    return branches;
}

QString GitManager::getCurrentBranch()
{
    QProcess p;
    p.setProcessChannelMode(QProcess::MergedChannels);
    QString res;
    QString cmd = "git branch";
    p.start(cmd);
    p.waitForFinished();
    QString output(p.readAll());
    QStringList branches = output.split("\n");
    p.close();
    for(int i=0;i<branches.count();i++) {
        QString br = branches.at(i);
        if(br.contains("*")) res = br.remove(QRegExp("[\\s\\*]"));
    }
    return res;
}

bool GitManager::goToBranch(const QString &brName)
{
    QProcess p;
    p.setProcessChannelMode(QProcess::MergedChannels);
    QString cmd = "git checkout " + brName;
    p.start(cmd);
    p.waitForFinished();
    QString output(p.readAll());
    p.close();
    if(output.contains("error")) return false;
    return true;
}

void GitManager::createInfoFile(const QString &repoPath, const QString &fileName)
{
    QString startPath = QDir::currentPath();
    QDir::setCurrent(repoPath);
    QProcess p;
    p.setProcessChannelMode(QProcess::MergedChannels);
    QString cmd = "git log --pretty=\"%h - %cd\" -1";// + fileName;
    p.start(cmd);
    p.waitForFinished();
    QString output(p.readAll());
    qDebug() << output;
    p.close();
    QDir::setCurrent(startPath);
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << output;
        file.close();
    }
}
