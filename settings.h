#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QStringList>
#include <QHash>
#include <QMutex>

class Settings
{
private:
    static const QString fName;
    QString sourceDirPath;
    QString outDirPath;
    QStringList branches;
    QHash<QString, QString> plcBranch;
    mutable QMutex mutex;

    Settings();
    Settings(const Settings&);
    Settings& operator=(Settings&);

    void writeAll();
    void readAll();
public:
    static Settings& getInstance()  {
        static Settings instance;
        return instance;
    }
    QString getSourceDirPath() const {return sourceDirPath;}
    QString getOutDirPath() const {return outDirPath;}
    QStringList getBranches() const {return branches;}
    QStringList getControllers() const;
    QString getControllerBranch(const QString &plcName) const;
    void setSourceDir(const QString &dirName);
    void setOutDir(const QString &dirName);
    void addBranch(const QString &brName);
    void deleteBranch(const QString &brName);
    void renameBranch(const QString &oldName, const QString &newName);
    void addController(const QString &name, const QString &branch);
    void editController(const QString &oldPLCName, const QString &newPLCName, const QString &newBranchName);
    void deleteController(const QString &contrName);
};

#endif // SETTINGS_H
