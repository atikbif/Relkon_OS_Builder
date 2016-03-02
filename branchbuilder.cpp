#include "branchbuilder.h"

const QString BranchBuilder::tmpDirName = "tmpBuild";

bool BranchBuilder::createTmpDir()
{
    QDir tmpDir(coreDir.absolutePath() + "/" + tmpDirName);
    if(!tmpDir.exists()) return tmpDir.mkdir(".");
    return true;
}

bool BranchBuilder::copySourceFileToTemplateDir()
{
    return false;
}

bool BranchBuilder::build()
{
    return false;
}

bool BranchBuilder::copyHeaderFiles()
{
    return false;
}

bool BranchBuilder::copyLinkerFiles()
{
    return false;
}

bool BranchBuilder::copyPaternFiles()
{
    return false;
}

bool BranchBuilder::delTemplateDir()
{
    QDir tmpDir(coreDir.absolutePath() + "/" + tmpDirName);
    if(tmpDir.exists()) return tmpDir.removeRecursively();
    return true;
}

BranchBuilder::BranchBuilder(const QString &inpDir, const QString &outDir, const QString &brName):
    inpPath(inpDir), outPath(outDir)
{
    coreDir.setPath(outDir + "/core");
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
    if(!copySourceFileToTemplateDir()) {result = "Cannot copy files to the template directory"; return false;}
    if(build()) {
        if(!copyHeaderFiles()) {result = "Cannot copy the header files"; return false;}
        if(!copyLinkerFiles()) {result = "Cannot copy the linker files";return false;}
        if(!copyPaternFiles()) {result = "Cannot copy the xml pattern files"; return false;}
    }else result = "Compile error";
    if(!delTemplateDir()) {result = "Cannot delete the template directory";return false;}
    return true;
}
