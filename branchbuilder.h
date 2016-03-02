#ifndef BRANCHBUILDER_H
#define BRANCHBUILDER_H

#include <QString>
#include <QDir>

class BranchBuilder
{
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
public:
    BranchBuilder(const QString &inpDir, const QString &outDir, const QString &brName);
    bool createBuild(QString &result);
};

#endif // BRANCHBUILDER_H
