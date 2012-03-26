/*
    This file is part of cPad Project.

    cPad is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    cPad is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with cPad.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "compileconfig.h"
#include <QtGui>

CompileConfig::CompileConfig(QWidget *parent):
    QDialog(parent)
{

#ifdef Q_WS_WIN32
    compilerFile = QDir("tcc").absoluteFilePath("tcc.exe");
#else
    compilerFile = QDir("/usr/bin").absoluteFilePath("gcc");
#endif
    args << "-Wall";
    createWidget();
    generalSetting();
    readSettings();
}

CompileConfig::~CompileConfig()
{
    writeSettings();
}

void CompileConfig::generalSetting()
{
    setWindowTitle(trUtf8("编译器设置"));
    setFixedSize(400, 150);

    connect( noButton, SIGNAL(clicked()),
             this, SLOT(closeDialog()));
    connect( yesButton, SIGNAL(clicked()),
            this, SLOT(setCompileConfig()));
    connect( browserFileButton, SIGNAL(clicked()),
             this, SLOT(openExeFile()));
    connect( browserDirButton, SIGNAL(clicked()),
             this, SLOT(openDir()));
}

void CompileConfig::createWidget()
{
    groupBox = new QGroupBox(trUtf8("编译器"), this);

    compilePositionLabel = new QLabel(trUtf8("编译器(.exe)"), this);
    tempPositionLabel = new QLabel(trUtf8("临时文件位置"), this);
    compileArgsLabel = new QLabel(trUtf8("编译参数"), this);
    compilePositionEdit = new QLineEdit(this);
    tempPositionEdit = new QLineEdit(this);
    compileArgsEdit = new QLineEdit(this);
    browserFileButton = new QPushButton(trUtf8("浏览"), this);
    browserFileButton->setFixedWidth(40);
    browserDirButton = new QPushButton(trUtf8("浏览"), this);
    browserDirButton->setFixedWidth(40);

    inLayout = new QGridLayout(groupBox);
    
    inLayout->addWidget(compilePositionLabel, 0, 0);
    inLayout->addWidget(compilePositionEdit, 0, 1);
    inLayout->addWidget(browserFileButton, 0, 2);

    inLayout->addWidget(tempPositionLabel, 1, 0);
    inLayout->addWidget(tempPositionEdit, 1, 1);
    inLayout->addWidget(browserDirButton, 1, 2);

    inLayout->addWidget(compileArgsLabel, 2, 0);
    inLayout->addWidget(compileArgsEdit, 2, 1);

    groupBox->setLayout(inLayout);

    yesButton = new QPushButton(trUtf8("确定"), this);
    noButton = new QPushButton(trUtf8("取消"), this);

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(groupBox, 0, 0, 4, 4);
    mainLayout->addWidget(yesButton, 4, 2);
    mainLayout->addWidget(noButton, 4, 3);
}

void CompileConfig::showDialog()
{
    compilePositionEdit->setText(compilerFile);
    compileArgsEdit->setText(args.join(" "));
    tempPositionEdit->setText(workingDir);
    show();
}

void CompileConfig::closeDialog()
{
    close();
}

/* 击编译器设置按钮时候，调用这个函数*/
void CompileConfig::openCompileConfig(QString fPath)
{
    if ( workingDir.isEmpty() )
        workingDir = fPath;
    if ( compilerFile.isEmpty() )
    {
#ifdef Q_WS_WIN32
        compilerFile = QDir("tcc").absoluteFilePath("tcc.exe");
#else
        compilerFile = QDir("/usr/bin").absoluteFilePath("gcc");
#endif
    }
    showDialog();
}

QString CompileConfig::getCompilerDir()
{
    return compilerFile;
}
QStringList CompileConfig::getArgs()
{
    return args;
}
QString CompileConfig::getWorkingDir()
{
    return workingDir;

}

/*
 * 用户点击确定之后，设置当前的参数，并
 * 把更改的东西发送给 runcompile*/
void CompileConfig::setCompileConfig()
{
    compilerFile = compilePositionEdit->text();
    ////////////////
    QString tmp;
    tmp = compileArgsEdit->text();
    args = tmp.split(" ", QString::SkipEmptyParts);
    ////////////////
    workingDir = tempPositionEdit->text();
    closeDialog();
    emit configChanged(compilerFile, workingDir, args);
}

/* 在用户打开.c文件的时候，如果工作目录为空，
 * 将.c文件目录改成默认目录  */
void CompileConfig::autoSetWorkingDir(QString wkDir, QString fn)
{
    if ( workingDir.isEmpty() )
        workingDir = wkDir;
}

void CompileConfig::openExeFile()
{
    QString fileSuffix = "EXE Source File (*.exe);;"
                 "All Files (*.*)";
    QString currentDir = workingDir;
    QString fileName = QFileDialog::getOpenFileName(
                this, 
                trUtf8("打开文件"),
                currentDir,
                fileSuffix,
                &fileSuffix//扩展名
                );//弹出对话框 选择文件
    if (! fileName.isEmpty())
    {//非空文件名
        compilerFile = fileName;
        compilePositionEdit->setText(fileName);
    }

}

void CompileConfig::openDir()
{
    QString currentDir = workingDir;
    QString DirName = QFileDialog::getExistingDirectory(
                this,
                trUtf8("打开文件"),
                currentDir,
                QFileDialog::ShowDirsOnly
              | QFileDialog::DontResolveSymlinks);//弹出对话框
    if (! DirName.isEmpty())
    {//非空文件名
        workingDir = DirName;
        tempPositionEdit->setText(DirName);
    }
}

void CompileConfig::readSettings()
{
    QSettings settings("5-up", "cPad");//调用设置
#ifdef Q_WS_WIN32
    compilerFile = settings.value("CompilerPosition", QDir("tcc").absoluteFilePath("tcc.exe")).toString();
#else
    compilerFile = settings.value("CompilerPosition", QDir("/usr/bin").absoluteFilePath("gcc")).toString();
#endif
    args = settings.value("CompilerArgs", "-Wall").toStringList();
    workingDir = settings.value("WorkingPosition", "").toString();
}

void CompileConfig::writeSettings()
{
    QSettings settings("5-up", "cPad");//调用设置
    settings.setValue("CompilerPosition", compilerFile);
    settings.setValue("CompilerArgs", args);
    settings.setValue("WorkingPosition", workingDir);
}
