import os.path

def UpdateCSProjectFile(csProjectPath, csProjectFileName, migrationPath, csFileName, withMigratorLocation):
    sqlFileName = ''
    if(csFileName.endswith('.cs') == False):
        return False
    sqlFileName = csFileName[:-3] + '.sql'
    
    try:
        with open(os.path.join(csProjectPath, csProjectFileName), 'r', encoding='utf-8') as f:
            lines = f.readlines()
        with open(os.path.join(csProjectPath, csProjectFileName), 'w', encoding='utf-8') as f:
            for i in range(len(lines)):
                if '</ItemGroup>' in lines[i]:
                    lines.insert(i, '    <EmbeddedResource Include=\"' + withMigratorLocation + migrationPath + sqlFileName + '\" />' + '\n')
                    break

            f.seek(0)
            f.writelines(lines)

        f.close()
        return True
    except Exception as e:
        print(e)
        f.close()
        return False

def CreateCSFile(migrationFolderPath, csFileName):
    if csFileName.startswith('Migration_') == False or csFileName.endswith('.cs') == False:
        return False
    
    index = len('Migration_')
    migrationVersionInfo = csFileName[index:]
    migrationVersionInfo = migrationVersionInfo[:-3]
    if migrationVersionInfo.isdigit() == False:
        return False
    
    try:
        # cs 파일 생성 로직
        with open(os.path.join(migrationFolderPath, csFileName), 'w') as f:
            f.write('using FluentMigrator;' + '\n\n')
            f.write('namespace Migrator.DBMigration' + '\n')
            f.write('{' + '\n')
            f.write('\t' + '[Migration(' + migrationVersionInfo + ')]' + '\n')
            f.write('\t' + 'public class ' + 'Migration_' + migrationVersionInfo + ': Migration' + '\n')
            f.write('\t' + '{' + '\n')
            f.write('\t' + '\t' + 'public override void Up()' + '\n')
            f.write('\t' + '\t' + '{' + '\n')
            f.write('\t' + '\t\t' + 'Execute.EmbeddedScript(\"' + 'Migration_' + migrationVersionInfo + '.sql\");' + '\n')
            f.write('\t' + '\t' + '}' + '\n\n')
            f.write('\t' + '\t' + 'public override void Down()' + '\n')
            f.write('\t' + '\t' + '{' + '\n')
            f.write('\t' + '\t' + '}' + '\n')
            f.write('\t' + '}' + '\n')
            f.write('}' + '\n')
        return True
    except:
        return False