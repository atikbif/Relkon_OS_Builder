#include "rcompiler.h"
#include <QFile>
#include <QProcess>
#include <QDebug>
#include <QDir>
#include <QRegExp>

void RCompiler::init()
{
    applPath = QCoreApplication::applicationDirPath();
    gccDir.setPath(applPath+"/arm-gcc/bin");
    objDir.setPath(dirOutName);
    inpDir.setPath(dirInpName);

    const QString fName = dirInpName+"/compiler.xml";
    QDomDocument domDoc;
    QFile file(fName);
    if(file.open(QIODevice::ReadOnly)) {
        if(domDoc.setContent(&file)) {
            QDomElement domElement=domDoc.documentElement();
            files.clear();
            linkFiles.clear();
            errors.clear();
            linkPattern="";
            traverseNode(domElement);
        }
        file.close();
    }else {
        outMessage fileError;
        fileError.strNum = -1;
        fileError.message = "Не удалось прочитать файл настроек " + fName;
        errors += fileError;
    }
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("IBM 866"));
}

void RCompiler::traverseNode(const QDomNode &node)
{
    QDomNode domNode = node.firstChild();
    while(!domNode.isNull()) {
        if(domNode.isElement()) {
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull()) {
                if(domElement.tagName() == "source") {
                    inpFile fileDefinition;
                    fileDefinition.inpName = domElement.text();
                    fileDefinition.outName = domElement.attribute("out");
                    fileDefinition.patternName = domElement.attribute("pattern");
                    files += fileDefinition;
                }else if(domElement.tagName() == "linkSource") {
                    inpFile fileDefinition;
                    fileDefinition.inpName = domElement.text();
                    fileDefinition.outName = domElement.attribute("out");
                    fileDefinition.patternName = domElement.attribute("pattern");
                    linkFiles += fileDefinition;
                }else if(domElement.tagName() == "link") {
                    linkPattern = domElement.text();
                    linkScript = domElement.attribute("script");
                }else if(domElement.tagName() == "pattern") {
                    QString patName = domElement.attribute("name");
                    if(!patName.isEmpty()) {
                        patterns.insert(patName,domElement.text());
                    }
                }
            }
        }
        traverseNode(domNode);
        domNode = domNode.nextSibling();
    }
}

QByteArray RCompiler::compFile(const QString &fName)
{
    QDir::setCurrent(gccDir.absolutePath());

    QString outName;
    QString patternName;
    bool hasFileBeenFound = false;
    for(int i=0;i<files.count();i++) {
        if(files[i].inpName == fName) {
            hasFileBeenFound = true;
            outName = files[i].outName;
            patternName = files[i].patternName;
            break;
        }
    }
    if(!hasFileBeenFound) for(int i=0;i<linkFiles.count();i++) {
        if(linkFiles[i].inpName == fName) {
            hasFileBeenFound = true;
            outName = linkFiles[i].outName;
            patternName = linkFiles[i].patternName;
            break;
        }
    }
    if(hasFileBeenFound) {

        if(!objDir.exists()) {
            objDir.mkdir(".");
        }
        if (QFile::exists(objDir.absolutePath()+"/" + outName)) {
            QFile::remove(objDir.absolutePath()+"/" + outName);
        }
        QProcess builder;
        builder.setProcessChannelMode(QProcess::MergedChannels);
        QString program = gccDir.absolutePath() + "/arm-none-eabi-gcc.exe";

        QString attr = " " + patterns[patternName];
        attr += " -I \"" + dirInpName + "\"";
        attr += " -o \"" + dirOutName + "/" + outName + "\"";
        attr += " \"" + dirInpName + "/" + fName + "\"";

        builder.start(program+attr);
        if (!builder.waitForFinished()) {
            QDir::setCurrent(applPath);
            return builder.errorString().toUtf8();
        }
        QDir::setCurrent(applPath);
        return builder.readAll();
    }
    QString errMessage;
    errMessage = "file " + fName + " was not found";
    QByteArray noFileMessage(errMessage.toUtf8());
    QDir::setCurrent(applPath);
    return noFileMessage;
}

RCompiler::RCompiler(const QString &inpDir, const QString &outDir, QObject *parent):
    QObject(parent), dirInpName(inpDir), dirOutName(outDir)
{
    startPath = QDir::currentPath();
    init();
}

bool RCompiler::compile()
{
    bool res = true;
    QDir::setCurrent(gccDir.absolutePath());

    QFile logFile;
    logFile.setFileName(applPath+"/compile.log");
    if(logFile.open(QIODevice::WriteOnly)){

        qDebug() << "START COMPILE";
        emit compileMessage("START COMPILE");
        qDebug() << "------------------------";
        emit compileMessage("-------------------------");

        QTextStream out(&logFile);

        for(int i=0;i<files.count();i++) {
            QByteArray result = compFile(files[i].inpName);
            if(result.count()) {out << result;res = false;}
            qDebug() << files[i].inpName;
            emit compileMessage(files[i].inpName);
        }
        qDebug() << "-------------------------";
        emit compileMessage("-------------------------");
        qDebug() << "COMPLETE";
        emit compileMessage("COMPLETE");

        logFile.close();
    }
    QDir::setCurrent(startPath);
    return res;
}
