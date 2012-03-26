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
#include <QtGui>
#include "runcompile.h"

#include <QDebug>

RunCompile::RunCompile(QWidget *parent, CodeEditor *editor):
    QDockWidget(parent)
{
    codeEditor = editor;
    createWidget();
    generalSetting();
}

void RunCompile::compileClear()
{
    compileEdit->clear();
}

void RunCompile::compilePrograms()
{
    compileAndRun();
}

void RunCompile::createWidget()
{
    compileEdit = new QPlainTextEdit();
    compileEdit->setReadOnly(true);
    
    compileLable = new QLabel(tr("编译结果"));

    oneLayout = new QGridLayout();
    twoLayout = new QGridLayout();
    oneWidget = new QWidget();
    twoWidget = new QWidget();
    clearCompile = new QPushButton(tr("清除"));
    compileToRun = new QPushButton(tr("编译并运行"));
}

void RunCompile::generalSetting()
{
    //设置 Dock 可移动区域为上下
    setAllowedAreas(Qt::BottomDockWidgetArea);
    //查找栏 可关 可移 可浮出 竖向标题
    setFeatures(QDockWidget::DockWidgetClosable | 
                QDockWidget::DockWidgetFloatable |
                QDockWidget::DockWidgetMovable
                );
    setWindowTitle(tr("编译"));//附着时不用标题
    clearCompile->setFixedWidth(80);
    compileToRun->setFixedWidth(80);
    QMainWindow *main = qobject_cast<QMainWindow *>(parent());
    main->addDockWidget(Qt::BottomDockWidgetArea, this);
    hide();

    connect(clearCompile, SIGNAL(clicked()),
            this, SLOT(compileClear()));
/*    connect(compileToRun, SIGNAL(clicked()),
            this, SLOT(compilePrograms()));*/

}

bool RunCompile::checkTcc()
{
    QFile compiler(compilerAbsoluteFile);

    if ( ! compiler.exists() )
    {
        QMessageBox::critical(this, tr("错误"),
                      tr("未发现编译器"),
                tr("确定"), 0, 0);
        return false;
    }
    else
        return true;
}

/* 此函数仅打开编译执行界面  */
void RunCompile::showCompileAndRun(QString getCompiler, QString getWorkingDir, QStringList getArgs, QString fn)
{
    getConfig(getCompiler, getWorkingDir, getArgs, fn);
    compileAndRun();
    twoLayout->addWidget(compileLable, 0, 0);
    twoLayout->addWidget(compileToRun, 0, 1);
    twoLayout->addWidget(clearCompile, 0, 2);
    twoLayout->addWidget(compileEdit, 1, 0, 1, 3);
    twoWidget->setLayout(twoLayout);
    this->setWidget(twoWidget);

    setWindowTitle(tr("编译并运行"));
    setMinimumHeight(150);
    show();
}

void RunCompile::showCompileOnly(QString getCompiler, QString getWorkingDir, QStringList getArgs, QString fn)
{
    getConfig(getCompiler, getWorkingDir, getArgs, fn);
    compileOnly();  //编译程序
    //====下面是控件排版
    oneLayout->addWidget(compileLable, 0, 0);
    oneLayout->addWidget(clearCompile, 0, 1);
    oneLayout->addWidget(compileEdit, 1, 0, 1, 2);
    oneWidget->setLayout(oneLayout);
    this->setWidget(oneWidget);

    setWindowTitle(tr("编译"));
    setMinimumHeight(150);
    show();
}
void RunCompile::compileAndRun()
{
    bool ret = compileOnly(); //先编译
    if ( ret == false )
        return;

    compileEdit->appendPlainText("Starting " + exeFile + "...");

#ifdef Q_WS_WIN32
    compile.start("cmd", QStringList() << "/C" << "start"  << "cmd" << "/K" << exeFile);
#else
    compile.start("xterm",QStringList() << "-hold" <<"-e" << exeFile);
#endif

    if ( ! compile.waitForFinished(5000) )
    {    //默认给5秒，如果超过时间，或者程序没有运行则返回
        return;
    }

}


bool RunCompile::compileOnly()
{
    if ( ! checkTcc() )
    {
        return false;
    }
    compile.setWorkingDirectory(workingDir);

    QStringList arg;
    int exit;
    arg << "-o" << exeFile << fName;

    compile.start(compilerAbsoluteFile, arg);

    if ( ! compile.waitForFinished(5000) ) //important
    {
        return false;
    }
    exit = compile.exitCode();
    error = compile.readAllStandardError();
    if ( error == "" )
        error = "Compiled " + exeFile + " successfully!";
    else
    {
        //codec = QTextCodec::codecForName("gbk");
        codec = QTextCodec::codecForLocale();
        error = codec->toUnicode(error.toLatin1().data());
    }

    compileEdit->appendPlainText(error);
    if ( exit == 1 )
        return false;
    return true;
}

void RunCompile::getConfig(QString getCompiler, QString getWorkingDir, QStringList getArgs, QString fn)
{
    fName = fn;
    compilerAbsoluteFile = getCompiler;
    workingDir = getWorkingDir;
    args = getArgs;
    QFileInfo f(fName);

    exeFile = QDir(workingDir).absoluteFilePath(f.baseName()+".exe");
}

/* slots */
void RunCompile::configChanged(QString getCompiler, QString getWorkingDir, QStringList getArgs)
{
    compilerAbsoluteFile = getCompiler;
    workingDir = getWorkingDir;
    args = getArgs;
    QFileInfo f(fName);
    exeFile = QDir(workingDir).absoluteFilePath(f.baseName()+".exe");
}

void RunCompile::setFileName(QString fn)
{
    fName = fn;
}

