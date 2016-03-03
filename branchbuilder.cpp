#include "branchbuilder.h"
#include <QFileInfo>
#include <QFile>
#include "rcompiler.h"
#include <QXmlStreamWriter>
#include <QStringList>

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
    RCompiler compiler(coreDir.absolutePath() + "/" + tmpDirName, objDir.absolutePath());
    connect(&compiler,SIGNAL(compileMessage(QString)),this,SIGNAL(sendMessage(QString)));
    return compiler.compile();
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

bool BranchBuilder::createPLCFile(const QHash<QString, QString> &plc, const QString &pathDir)
{
    QXmlStreamWriter xmlWriter;

    xmlWriter.setAutoFormatting(true);
    QFile file(pathDir + "/core/controllers.xml");

    QStringList controller;
    controller = plc.keys();
    controller.sort();

    if (!file.open(QIODevice::WriteOnly)) {return false;}
    else
    {
        xmlWriter.setDevice(&file);

        /* Writes a document start with the XML version number. */
        xmlWriter.writeStartDocument();
        xmlWriter.writeStartElement("controllers");

        foreach (QString c, controller) {
           xmlWriter.writeStartElement("plc");
           xmlWriter.writeAttribute("name",c);
           xmlWriter.writeAttribute("build",plc.value(c));
           xmlWriter.writeEndElement();
        }

        xmlWriter.writeEndElement();
        xmlWriter.writeEndDocument();
    }
    return true;
}

BranchBuilder::BranchBuilder(const QString &inpDir, const QString &outDir, const QString &brName, QObject *parent):
    QObject(parent), inpPath(inpDir), outPath(outDir)
{
    coreDir.setPath(outDir + "/core");
    if(!coreDir.exists()) coreDir.mkdir(".");

    branchDir.setPath(coreDir.absolutePath() + "/" + brName);
    if(!branchDir.exists()) branchDir.mkdir(".");
    objDir.setPath(branchDir.absolutePath() + "/obj");
    objDir.removeRecursively();
    if(!objDir.exists()) objDir.mkdir(".");
    sourceDir.setPath(branchDir.absolutePath() + "/src");
    sourceDir.removeRecursively();
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

