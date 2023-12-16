import os.path
import shutil
import MakeCSFile

migrationFolderPath = 'DBMigration/'
migratorProjectFilePath = 'Migrator.csproj'
csProjectFolderPath = ''
csProjectName = 'Migrator.csproj'
withMigratorLocation = '../Migrator/'

def MakeAppendFileList(sqlFileList, csFileList):
    for fileName in os.listdir(migrationFolderPath):
        if os.path.isfile(os.path.join(migrationFolderPath, fileName)) == True:
            if fileName.endswith('.sql') == True:
                sqlFileList.append(fileName)

    for sqlFile in sqlFileList:
        csFileName = sqlFile.replace('.sql', '.cs')
        csFilePath = os.path.join(migrationFolderPath, csFileName)
        if not os.path.isfile(csFilePath):
            csFileList.append(csFileName)

def DoGenerate():
    if os.path.isfile(migratorProjectFilePath) == False:
        print("Migrator.csproj is not located in migratorFilePath")
        return False
    
    sqlFileList = []
    willBeMakeFileList = []
    MakeAppendFileList(sqlFileList, willBeMakeFileList)

    for csMakeFile in willBeMakeFileList:
        if MakeCSFile.CreateCSFile(migrationFolderPath, csMakeFile) == False:
            print('Failed to create cs file')
            return False
        if MakeCSFile.UpdateCSProjectFile(csProjectFolderPath, csProjectName, migrationFolderPath, csMakeFile, withMigratorLocation) == False:
            print('Failed to update csproj file')
            return False
        
    return True


if __name__ == "__main__":
    if DoGenerate() == False:
        print("Generate failed")
    else:
        print("Generate successed")
        
    input("Press enter key...")