#include "settings.h"
#include <QMutexLocker>
#include <QSettings>

const QString Settings::fName = "settings.conf";

Settings::Settings()
{
    readAll();
}

void Settings::writeAll()
{
    QSettings settings( fName, QSettings::IniFormat );
    settings.clear();
    settings.beginGroup( "Path" );
    settings.setValue("sourcePath",sourceDirPath);
    settings.setValue("outPath",outDirPath);
    settings.endGroup();

    settings.beginWriteArray("branches");
    for (int i = 0; i < branches.count(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("brName", branches.at(i));
    }
    settings.endArray();

    settings.beginWriteArray("controllers");
    QStringList plcNames = plcBranch.keys();
    for (int i = 0; i < plcNames.count(); ++i) {
        settings.setArrayIndex(i);
        QString plcName = plcNames.at(i);
        QString brName = plcBranch.value(plcName);
        settings.setValue("plcName", plcName);
        settings.setValue("brName",brName);
    }
    settings.endArray();
}

void Settings::readAll()
{
    QSettings settings( fName, QSettings::IniFormat );
    settings.beginGroup( "Path" );
    sourceDirPath = settings.value("sourcePath").toString();
    outDirPath = settings.value("outPath").toString();
    settings.endGroup();

    int size = settings.beginReadArray("branches");
    branches.clear();
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        QString brName = settings.value("brName").toString();
        branches.append(brName);
    }
    settings.endArray();

    size = settings.beginReadArray("controllers");
    plcBranch.clear();
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        QString plcName = settings.value("plcName").toString();
        QString brName = settings.value("brName").toString();
        if((!plcName.isEmpty())&&(!brName.isEmpty())) {
            plcBranch.insert(plcName, brName);
        }
    }
    settings.endArray();
}

QStringList Settings::getControllers() const
{
    QMutexLocker locker(&mutex);
    return plcBranch.keys();
}

QString Settings::getControllerBranch(const QString &plcName) const
{
    QMutexLocker locker(&mutex);
    return plcBranch.value(plcName);
}

void Settings::setSourceDir(const QString &dirName)
{
    QMutexLocker locker(&mutex);
    sourceDirPath = dirName;
    writeAll();
}

void Settings::setOutDir(const QString &dirName)
{
    QMutexLocker locker(&mutex);
    outDirPath = dirName;
    writeAll();
}

void Settings::addBranch(const QString &brName)
{
    QMutexLocker locker(&mutex);
    branches.append(brName);
    branches.removeDuplicates();
    writeAll();
}

void Settings::deleteBranch(const QString &brName)
{
    QMutexLocker locker(&mutex);
    branches.removeAll(brName);
    writeAll();
}

void Settings::renameBranch(const QString &oldName, const QString &newName)
{
    QMutexLocker locker(&mutex);
    branches.removeAll(oldName);
    branches.append(newName);
    branches.removeDuplicates();
    writeAll();
}

void Settings::addController(const QString &name, const QString &branch)
{
    QMutexLocker locker(&mutex);
    plcBranch.insert(name,branch);
    writeAll();
}

void Settings::editController(const QString &oldPLCName, const QString &newPLCName, const QString &newBranchName)
{
    QMutexLocker locker(&mutex);
    plcBranch.remove(oldPLCName);
    plcBranch.insert(newPLCName,newBranchName);
    writeAll();
}

void Settings::deleteController(const QString &contrName)
{
    QMutexLocker locker(&mutex);
    plcBranch.remove(contrName);
    writeAll();
}
