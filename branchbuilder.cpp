#include "branchbuilder.h"
#include <QFileInfo>
#include <QFile>

const QString BranchBuilder::tmpDirName = "tmpBuild";

bool BranchBuilder::createTmpDir()
{
    QDir tmpDir(coreDir.absolutePath() + "/" + tmpDirName);
    if(!tmpDir.exists()) return tmpDir.mkdir(".");
    return true;
}

bool BranchBuilder::copySourceFileToTemplateDir()
{
    QDir tmpDir(coreDir.absolutePath() + "/" + tmpDirName);
    if(!tmpDir.exists()) return false;
    return copyFileRecursively(inpPath, tmpDir.absolutePath());
}

bool BranchBuilder::build()
{
    return true;
}

bool BranchBuilder::copyHeaderFiles()
{
    QDir tmpDir(coreDir.absolutePath() + "/" + tmpDirName);
    if(!tmpDir.exists()) return false;
    QStringList fileNames = tmpDir.entryList(QDir::Files | QDir::NoSymLinks);
    foreach (const QString &fileName, fileNames) {
        if(fileName.contains(".h")) {
            if(!QFile::copy(tmpDir.absolutePath() + "/" + fileName, sourceDir.absolutePath() + "/" + fileName)) return false;
        }
    }
    return true;
}

bool BranchBuilder::copyLinkerFiles()
{
    QDir tmpDir(coreDir.absolutePath() + "/" + tmpDirName);
    if(!tmpDir.exists()) return false;
    QStringList fileNames = tmpDir.entryList(QDir::Files | QDir::NoSymLinks);
    foreach (const QString &fileName, fileNames) {
        if(fileName.contains(".ld")) {
            if(!QFile::copy(tmpDir.absolutePath() + "/" + fileName, sourceDir.absolutePath() + "/" + fileName)) return false;
        }
    }
    return true;
}

bool BranchBuilder::copyPaternFiles()
{
    QDir tmpDir(coreDir.absolutePath() + "/" + tmpDirName);
    if(!tmpDir.exists()) return false;
    QStringList fileNames = tmpDir.entryList(QDir::Files | QDir::NoSymLinks);
    foreach (const QString &fileName, fileNames) {
        if(fileName.contains(".xml")) {
            if(!QFile::copy(tmpDir.absolutePath() + "/" + fileName, sourceDir.absolutePath() + "/" + fileName)) return false;
        }
    }
    return true;
}

bool BranchBuilder::delTemplateDir()
{
    QDir tmpDir(coreDir.absolutePath() + "/" + tmpDirName);
    if(tmpDir.exists()) return tmpDir.removeRecursively();
    return true;
}

bool BranchBuilder::copyFileRecursively(const QString &fPath, const QString &destDir)
{
    QFileInfo srcFileInfo(fPath);
    if(srcFileInfo.isDir()) {
        QDir sourceDir(fPath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        foreach (const QString &fileName, fileNames) {
            const QString newSrcFilePath
                    = fPath + QLatin1Char('/') + fileName;
            if (!copyFileRecursively(newSrcFilePath, destDir))
                return false;
        }
    }else {
        QString fName = srcFileInfo.fileName();
        if(fName.contains(".c") | fName.contains(".h") |
                fName.contains(".xml") | fName.contains(".s") |
                fName.contains(".ld")) {
            if(!QFile::copy(fPath, destDir + "/" + fName)) return false;
        }
    }
    return true;
}

BranchBuilder::BranchBuilder(const QString &inpDir, const QString &outDir, const QString &brName):
    inpPath(inpDir), outPath(outDir)
{
    coreDir.setPath(outDir + "/core");
    coreDir.removeRecursively();
    if(!coreDir.exists()) coreDir.mkdir(".");

    branchDir.setPath(coreDir.absolutePath() + "/" + brName);
    if(!branchDir.exists()) branchDir.mkdir(".");
    objDir.setPath(branchDir.absolutePath() + "/obj");
    if(!objDir.exists()) objDir.mkdir(".");
    sourceDir.setPath(branchDir.absolutePath() + "/src");
    if(!sourceDir.exists()) sourceDir.mkdir(".");
}

bool BranchBuilder::createBuild(QString &result)
{
    if(!createTmpDir()) {result = "Cannot create the template directory"; return false;}
    if(!copySourceFileToTemplateDir()) {result = "Cannot copy files to the template directory"; delTemplateDir(); return false;}
    if(build()) {
        if(!copyHeaderFiles()) {result = "Cannot copy the header files"; delTemplateDir(); return false;}
        if(!copyLinkerFiles()) {result = "Cannot copy the linker files";delTemplateDir(); return false;}
        if(!copyPaternFiles()) {result = "Cannot copy the xml pattern files"; delTemplateDir(); return false;}
    }else {result = "Compile error"; delTemplateDir(); return false;}
    if(!delTemplateDir()) {result = "Cannot delete the template directory";return false;}
    return true;
}
