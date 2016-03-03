#ifndef BRANCHBUILDER_H
#define BRANCHBUILDER_H

#include <QString>
#include <QDir>
#include <QObject>
#include <QHash>

class BranchBuilder: public QObject
{
    Q_OBJECT
    QString inpPath;
    QString outPath;
    static const QString tmpDirName;
    QDir coreDir;
    QDir branchDir;
    QDir objDir;
    QDir sourceDir;

    bool createTmpDir();
    bool copySourceFileToTemplateDir();
    bool build();
    bool copyHeaderFiles();
    bool copyLinkerFiles();
    bool copyPaternFiles();
    bool delTemplateDir();
    bool copyFileRecursively(const QString &fName, const QString &destDir);

public:
    BranchBuilder(const QString &inpDir, const QString &outDir, const QString &brName, QObject *parent = 0);
    bool createBuild(QString &result);
    static bool createPLCFile(const QHash<QString, QString> &plc, const QString &pathDir);
signals:
    void sendMessage(const QString &message);
};

#endif // BRANCHBUILDER_H
