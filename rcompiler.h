#ifndef RCOMPILER_H
#define RCOMPILER_H

#include <QString>
#include <QVector>
#include <QtXml>
#include <QMap>
#include <QObject>

typedef struct {
    QString inpName;
    QString outName;
    QString patternName;
}inpFile;

typedef struct {
    int strNum;
    QString message;
}outMessage;

class RCompiler: public QObject
{
    Q_OBJECT

    QVector<inpFile> files;
    QVector<inpFile> linkFiles;
    QVector<outMessage> errors;
    QString linkPattern;
    QString linkScript;
    QMap<QString,QString> patterns; // pattern's name, pattern's body

    QString dirInpName;
    QString dirOutName;
    QString startPath;

    void init(void);    // read settings from xml file
    void traverseNode(const QDomNode &node);    // read node from xml file (settings)
    QByteArray compFile(const QString &fName);
    QString applPath;
    QDir gccDir;
    QDir objDir;
    QDir inpDir;
public:
    explicit RCompiler(const QString &inpDir, const QString &outDir, QObject *parent = 0);
    QVector<outMessage>& getOutErrors(void) {return errors;}
    bool compile(void);
signals:
    void compileMessage(const QString &message);
};

#endif // RCOMPILER_H
