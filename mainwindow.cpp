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
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent)
{
    //申请中心控件
    centerWidget = new CenterWidget(this);
        //设置中心控件
    setCentralWidget(centerWidget);

    //保存中心控件中的编辑器控件地址
    codeEditor = centerWidget->codeEditor;

    //申请函数列表区域
    functionListArea = new FunctionListArea(this, codeEditor);

    //申请状态栏
    statusBar = new StatusBar(this);
        //状态栏用的编辑器
    statusBar->setCodeEditor(codeEditor);

    recomposeDialog = new RecomposeDialog(this, codeEditor); 
    autoComple = new AutoComple(codeEditor);
    runCompile = new RunCompile(this, codeEditor);
    compileConfig = new CompileConfig(this);

    cpadPalette = new CpadPalette;

    createActions();//建立所有菜单项
    createMenus();//建立菜单
    createToolBars();//建立工具栏
    createStatusBar();//建立状态栏

    //申请查找替换区域 传递编辑器 菜单项
    findReplaceArea = new FindReplaceArea(
            this, codeEditor,
            findAction, replaceAction);
    
    generalSetting();//主窗体一般设置
    readSettings();//读取程序保存的设置并对程序进行设置
}

void MainWindow::openRecentFile()
{
    if (mayBeSave()) {
        QAction *action = qobject_cast<QAction *>(sender());
        if (action)
            loadFileContent(action->data().toString());
    }
}

void MainWindow::openColorTheme()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        emit colorThemeChanged(action->data().toInt());
}

bool MainWindow::mayBeSave()
{
    if (codeEditor->document()->isModified()) {
        //弹出对话框询问是否需要保存
        int ret = QMessageBox::warning(this, tr("警告"),
                     tr("当前文件已经被修改\n"
                        "是否需要保存您的修改？"),
                     tr("保存"),
                     tr("丢弃"),
                     tr("取消"),
                     0, 2);
        if (ret == 0)//保存文件
            return saveFile();
        else if (ret == 2)//取消操作
            return false;
    }
    return true;
}

void MainWindow::newFile()
{
    if (mayBeSave()) {
        codeEditor->clear();//清空编辑器中内容
        setCurrentFile("");//新文件 空名字
    }
}
         
void MainWindow::openFile()
{
    if (mayBeSave()) {
        //扩展名
        QString fileSuffix = "C Source File (*.c);;"
                     "C Header File (*.h);;"
                     "Text File (*.txt);;"
                     "All Files (*.*)";
        QString fileName = QFileDialog::getOpenFileName(
                    this, 
                    tr("打开文件"),
                    currentDir,
                    fileSuffix,
                    &fileSuffix//扩展名
                    );//弹出对话框 选择文件
        if (! fileName.isEmpty())//非空文件名 载入文件内容
            loadFileContent(fileName);
    }
}

bool MainWindow::saveFile()
{
    if (currentFile.isEmpty())//当前文件没有名字(新建的文件) 需要保存
        return saveAsFile();
    else
        return saveFileContent(currentFile);
}

bool MainWindow::saveAsFile()
{
    //扩展名
    QString fileSuffix = "C Source File (*.c);;"
                         "C Header File (*.h);;"
                         "Text File (*.txt);;"
                         "All Files (*.*)";
    QString fileName = QFileDialog::getSaveFileName(
                    this,
                    tr("保存文件"),
                    currentDir + currentFile,
                    fileSuffix,
                    &fileSuffix
                    );//弹出对话框保存文件
    if (fileName.isEmpty())
        return false;
    
    return saveFileContent(fileName);//写入文件内容
}

void MainWindow::aboutDialog()
{   //弹出关于对话框
    QMessageBox::about(this, tr("关于 cPad"),
            tr("cPad -C语言编辑器 Version 1.0\n"
                   "Five-UP 项目开发组\n"
                   "cPad 基于 GPL 协议发布"));
}

void MainWindow::documentWasModified()
{
    //如果当前文件有修改 标题栏有 * 号标记
    setWindowModified(codeEditor->document()->isModified());
}

void MainWindow::transTabToSpace(QString &codeString)
{
    int codeLength , codePosition , codePositionLine;
    codeLength = codeString . length ( );
    for ( codePosition = codePositionLine = 0 ; codePosition < codeLength ; codePosition ++ )
    {
        if ( codeString [ codePosition ] == 9 )
        {
            codeString . remove ( codePosition , 1 );
            if ( codePositionLine % 4 == 0 )
                codeString . insert ( codePosition , "    " );
            else if ( codePositionLine % 4 == 1 )
                codeString . insert ( codePosition , "   " );
            else if ( codePositionLine % 4 == 2 )
                codeString . insert ( codePosition , "  " );
            else if ( codePositionLine % 4 == 3 )
                codeString . insert ( codePosition , " " );
            codeLength = codeString . length ( );
            codePositionLine ++;
        }
        else if ( codeString [ codePosition ] == '\n' )
            codePositionLine = 0;
        else
            codePositionLine ++;
    }

}

void MainWindow::loadFileContent(const QString &fileName)
{
    QFile file(fileName);
    //读取文本文件失败
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("cPad"),
                             tr("不能读取文件 %1:\n%2")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    //in.setAutoDetectUnicode(true);
    //in.setCodec("UTF-8");
    //---------------------------------
    //qDebug() << in.codec()->name() << endl;
    //qDebug() << in.codec()->codecForLocale() << endl;
    //qDebug() << in.locale() << endl;
    //---------------------------------
#ifndef QT_NO_CURSOR//光标变化
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    QString str(in.readAll());
    transTabToSpace(str);
    codeEditor->setPlainText(str);
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    setCurrentDir(fileName);
    compileConfig->autoSetWorkingDir(currentDir, fileName);//载入文件的同时，如果编译的临时文件夹为空，设置当前目录为临时文件夹
    runCompile->setFileName(fileName);
    //statusBar()->showMessage(tr("文件已载入"), 2000);//2000ms = 2s 显示时间
    statusBar->showMessage(tr("文件已载入"), 2000);//2000ms = 2s 显示时间
}

bool MainWindow::saveFileContent(const QString &fileName)
{
    QFile file(fileName);
    //写入文本文件失败
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("cPad"),
                             tr("不能写入文件 %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    //out.setAutoDetectUnicode(true);
    out.setCodec(QTextCodec::codecForLocale());
#ifndef QT_NO_CURSOR//光标变化
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << codeEditor->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    setCurrentDir(fileName);
    statusBar->showMessage(tr("文件已保存"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    currentFile = fileName;
    codeEditor->document()->setModified(false);//新文件设置未修改
    setWindowModified(false);

    QString shownName = currentFile;
    if (currentFile.isEmpty()) {//对空文件名进行 untitled.c 显示
        shownName = "untitled.c";
        emit currentFileChanged(strippedName(shownName));//新建文件显示 shownName
    }
    else {
        emit currentFileChanged(strippedName(currentFile));//现有文件显示 currentFile

        QSettings settings("5-up", "cPad");
        QStringList files = settings.value("recentFileList").toStringList();
        files.removeAll(fileName);
        files.prepend(fileName);
        while (files.size() > MaxRecentFiles)
            files.removeLast();

        settings.setValue("recentFileList", files);
    }
    updateRecentFile();
    setWindowFilePath(shownName);//显示名称
}

void MainWindow::setCurrentDir(const QString &fileName)
{
    currentDir = QFileInfo(fileName).dir().path();
    emit currentDirChanged(currentDir);
}

void MainWindow::updateRecentFile()
{
    QSettings settings("5-up", "cPad");
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    recentFileMenu->setEnabled(numRecentFiles > 0);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActions[i]->setText(text);
        recentFileActions[i]->setData(files[i]);
        recentFileActions[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActions[j]->setVisible(false);
}

void MainWindow::cleanRecentFile()
{
    QSettings settings("5-up", "cPad");
    QStringList files = settings.value("recentFileList").toStringList();
    files.clear();
    settings.setValue("recentFileList", files);
    updateRecentFile();
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::generalSetting()
{
    //设置 Modified 修改信号的连接
    connect(codeEditor->document(), SIGNAL(contentsChanged()),
            this, SLOT(documentWasModified()));
    setCurrentFile("");//直接打开程序时设置为新建文件
    setWindowIcon(QIcon(":/resources/donkey32.png"));//设置左上角图标
    setContextMenuPolicy(Qt::NoContextMenu);//去除右键菜单
    setAcceptDrops(true);

    connect(this, SIGNAL(colorThemeChanged(int)),
            cpadPalette, SLOT(myFavorate(int)));
    connect(cpadPalette, SIGNAL(groupChanged(QList<QColor>)),
            codeEditor, SLOT(changeColorGroup(QList<QColor>)));
    connect(cpadPalette, SIGNAL(groupChanged(QList<QColor>)),
            centerWidget->syntaxHighlighter, SLOT(changeColorGroup(QList<QColor>)));
    connect(codeEditor, SIGNAL(needAutoComple(int)),
            autoComple, SLOT(doAutoComple(int)));
    connect(codeEditor, SIGNAL(fontChanged(QFont)),
            autoComple, SLOT(fontSettings(QFont)));
    connect(runCompile->compileToRun, SIGNAL(clicked()),
            this, SLOT(compileButtonChecked()));
}

void MainWindow::createActions()
{
    newAction = new QAction(QIcon(":/resources/new.png"), tr("新建(&N)"), this);
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("新建文件"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(QIcon(":/resources/open.png"), tr("打开(&O)..."), this);
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("打开文件"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

    saveAction = new QAction(QIcon(":/resources/save.png"), tr("保存(&S)"), this);
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("保存文件"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsAction = new QAction(QIcon(":/resources/saveas.png"), tr("另存为(&A)..."), this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("保存为另一个文件"));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAsFile()));

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }

    cleanRecentFileAction = new QAction(tr("清除所有"), this);
    cleanRecentFileAction->setStatusTip(tr("清楚所有最近打开文件"));
    connect(cleanRecentFileAction, SIGNAL(triggered()),
            this, SLOT(cleanRecentFile()));

    exitAction = new QAction(tr("退出(&X)"), this);
    exitAction->setShortcut(QKeySequence::Quit);
    exitAction->setStatusTip(tr("退出程序"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    
    undoAction = new QAction(QIcon(":/resources/undo.png"), tr("撤销(&U)"), this);
    undoAction->setShortcut(QKeySequence::Undo);
    undoAction->setStatusTip(tr("撤销上次的操作"));
    //默认设置不可用
    undoAction->setEnabled(false);
    connect(undoAction, SIGNAL(triggered()), codeEditor, SLOT(undo()));
    //编辑器可撤销信号与菜单项可用设置链接
    connect(codeEditor, SIGNAL(undoAvailable(bool)),
            undoAction, SLOT(setEnabled(bool)));

    redoAction = new QAction(QIcon(":/resources/redo.png"), tr("重做(&R)"), this);
    redoAction->setShortcut(QKeySequence::Redo);
    redoAction->setStatusTip(tr("重做上次的操作"));
    //默认设置不可用
    redoAction->setEnabled(false);
    connect(redoAction, SIGNAL(triggered()), codeEditor, SLOT(redo()));
    //编辑器可重做信号与菜单项可用设置链接
    connect(codeEditor, SIGNAL(redoAvailable(bool)),
            redoAction, SLOT(setEnabled(bool)));

    cutAction = new QAction(QIcon(":/resources/cut.png"), tr("剪切(&T)"), this);
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip(tr("剪切框选的内容至剪贴板"));
    //默认设置不可用
    cutAction->setEnabled(false);
    connect(cutAction, SIGNAL(triggered()), codeEditor, SLOT(cut()));
    //编辑器可复制信号与菜单项可用设置链接
    connect(codeEditor, SIGNAL(copyAvailable(bool)),
            cutAction, SLOT(setEnabled(bool)));

    copyAction = new QAction(QIcon(":/resources/copy.png"), tr("复制(&C)"), this);
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(tr("复制框选的内容至剪贴板"));
    //默认设置不可用
    copyAction->setEnabled(false);
    connect(copyAction, SIGNAL(triggered()), codeEditor, SLOT(copy()));
    //编辑器可复制信号与菜单项可用设置链接
    connect(codeEditor, SIGNAL(copyAvailable(bool)),
            copyAction, SLOT(setEnabled(bool)));

    pasteAction = new QAction(QIcon(":/resources/paste.png"), tr("粘贴(&P)"), this);
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip(tr("粘贴剪贴板中的内容至当前位置"));
    connect(pasteAction, SIGNAL(triggered()), codeEditor, SLOT(paste()));

    selectAllAction = new QAction(QIcon(":/resources/selectall.png"), tr("全选(&A)"), this);
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    selectAllAction->setStatusTip(tr("将文本框中全部内容框选"));
    connect(selectAllAction, SIGNAL(triggered()), codeEditor, SLOT(selectAll()));

    deleteAction = new QAction(QIcon(":/resources/delete.png"), tr("删除(&L)"), this);
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setStatusTip(tr("删除框选的内容"));
    //默认设置不可用
    deleteAction->setEnabled(false);
    connect(deleteAction, SIGNAL(triggered()),
            codeEditor, SLOT(deleteSelectedText()));
    //编辑器可复制信号与菜单项可用设置链接
    connect(codeEditor, SIGNAL(copyAvailable(bool)),
            deleteAction, SLOT(setEnabled(bool)));

    //findAction = findArea->toggleViewAction();//使用 FindArea 提供的菜单项
    findAction = new QAction(this);
    findAction->setText(tr("查找(&F)"));
    findAction->setIcon(QIcon(":/resources/find.png"));
    findAction->setShortcut(QKeySequence::Find);
    findAction->setStatusTip(tr("查找字符串"));
    findAction->setCheckable(true);

    //replaceAction = replaceArea->toggleViewAction();//使用 ReplaceArea 提供的菜单项 
    replaceAction = new QAction(this);
    replaceAction->setText(tr("替换(&R)"));
    replaceAction->setIcon(QIcon(":/resources/replace.png"));
    replaceAction->setShortcut(QKeySequence::Replace);
    replaceAction->setStatusTip(tr("替换字符串"));
    replaceAction->setCheckable(true);
    
    statusBarAction = new QAction(tr("状态栏"), this);
    statusBarAction->setShortcut(Qt::Key_F9);
    statusBarAction->setStatusTip(tr("状态栏显示与否"));
    statusBarAction->setCheckable(true);
    connect(statusBarAction, SIGNAL(toggled(bool)), 
            this, SLOT(statusBarVisible(bool)));

    fileToolAction = new QAction(tr("文件工具栏(&F)"), this);
    fileToolAction->setStatusTip(tr("文件工具栏显示与否"));
    fileToolAction->setCheckable(true);
    fileToolAction->setChecked(true);
    connect(fileToolAction, SIGNAL(toggled(bool)), 
            this, SLOT(fileToolVisible(bool)));

    editToolAction = new QAction(tr("编辑工具栏(&E)"), this);
    editToolAction->setStatusTip(tr("编辑工具栏显示与否"));
    editToolAction->setCheckable(true);
    editToolAction->setChecked(true);
    connect(editToolAction, SIGNAL(toggled(bool)), 
            this, SLOT(editToolVisible(bool)));

    toolToolAction = new QAction(tr("功能工具栏(&U)"), this);
    toolToolAction->setStatusTip(tr("功能工具栏显示与否"));
    toolToolAction->setCheckable(true);
    toolToolAction->setChecked(true);
    connect(toolToolAction, SIGNAL(toggled(bool)), 
            this, SLOT(toolToolVisible(bool)));

    syntaxHighlightAction = new QAction(tr("语法高亮"), this);
    syntaxHighlightAction->setStatusTip(tr("对编辑器中的语法是否进行高亮"));
    syntaxHighlightAction->setCheckable(true);
    connect(syntaxHighlightAction, SIGNAL(toggled(bool)),
            centerWidget->syntaxHighlighter, SLOT(turnOnSyntaxHighlight(bool)));

    highlightLineAction = new QAction(tr("高亮光标行"), this);
    highlightLineAction->setStatusTip(tr("对编辑器中的光标所在行是够进行高亮"));
    highlightLineAction->setCheckable(true);
    connect(highlightLineAction, SIGNAL(toggled(bool)),
            codeEditor, SLOT(turnOnHighlightCurrentLine(bool)));
    
    colorThemeActions[0] = new QAction(tr("Desert"), this);
    colorThemeActions[1] = new QAction(tr("Koehler"), this);
    colorThemeActions[2] = new QAction(tr("Morning"), this);
    colorThemeActions[3] = new QAction(tr("Ron"), this);
    colorThemeActions[4] = new QAction(tr("Sample"), this);
    colorThemeActions[5] = new QAction(tr("Dark"), this);

    colorThemeGroup = new QActionGroup(this);
    for ( int i = 0; i < MaxColorThemes; i++ ) {
        colorThemeActions[i]->setData(i);
        colorThemeActions[i]->setCheckable(true);
        colorThemeGroup->addAction(colorThemeActions[i]);
        connect(colorThemeActions[i], SIGNAL(triggered()),
                this, SLOT(openColorTheme()));
    }

    wrapAction = new QAction(tr("自动换行(&W)"), this);
    wrapAction->setStatusTip(tr("自动换行"));
    wrapAction->setCheckable(true);//设置此菜单项为可选的(类似开关)
    //开关状态链接编辑器的设置开关的 SLOT 函数
    connect(wrapAction, SIGNAL(toggled(bool)), codeEditor, 
            SLOT(setWrapMode(bool)));

    fontSettingAction = new QAction(QIcon(":/resources/font.png"), tr("选择字体(&F)..."), this);
    fontSettingAction->setStatusTip(tr("设置字体"));
    //设置字体函数
    connect(fontSettingAction, SIGNAL(triggered()), codeEditor, 
            SLOT(fontSettings()));

    compileAction = new QAction(QIcon(":resources/build.png"), tr("编译"), this);
    compileAction->setShortcut(Qt::Key_F5);
    compileAction->setStatusTip(tr("编译当前程序"));
    compileAction->setData(0);
    connect(compileAction, SIGNAL(triggered()), 
            this, SLOT(preCompile()));

    compileAndRunAction = new QAction(QIcon(":resources/run.png"), tr("编译并运行"), this);
    compileAndRunAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_F5));
    compileAndRunAction->setStatusTip(tr("编译并运行当前程序"));
    compileAndRunAction->setData(1);
    connect(compileAndRunAction, SIGNAL(triggered()), 
            this, SLOT(preCompile()));

    recomposeAction = new QAction(QIcon(":/resources/recompose.png"), tr("重排版(&R)"), this);
    recomposeAction->setShortcut(Qt::Key_F6);
    recomposeAction->setStatusTip(tr("对文本框中的代码进行重排版"));
    connect(recomposeAction, SIGNAL(triggered()),
            recomposeDialog, SLOT(open()));

    compileConfigAction = new QAction(tr("编译器设置"), this);
    //compileConfigAction->setShortcut(Qt::Key_F10);
    compileConfigAction->setStatusTip(tr("进行编译器的设置"));
    connect(compileConfigAction, SIGNAL(triggered()),
            this, SLOT(openCompileconfig()));
    //当点击确定时，告诉"编译并执行"对话框，参数可能已经更改了
    connect( compileConfig, SIGNAL(configChanged(QString,QString,QStringList)),
             runCompile, SLOT(configChanged(QString,QString,QStringList)));

    functionListAction = new QAction(QIcon(":/resources/functionlist.png"), tr("函数列表(&F)"), this);
    functionListAction->setShortcut(Qt::Key_F7);
    functionListAction->setStatusTip(tr("显示代码中存在的函数"));
    functionListAction->setCheckable(true);
    connect(functionListAction, SIGNAL(toggled(bool)),
            functionListArea, SLOT(enableArea(bool)));

    functionCommentAction = new QAction(QIcon(":/resources/functioncomment.png"), tr("生成函数注释(&C)"), this);
    functionCommentAction->setShortcut(Qt::Key_F8);
    functionCommentAction->setStatusTip(tr("若光标所在行是函数实体的开始，则生成函数标准注释"));
    connect(functionCommentAction, SIGNAL(triggered()),
            codeEditor, SLOT(functionComment()));

    autoIndentAction = new QAction(tr("自动缩进"), this);
    autoIndentAction->setStatusTip(tr("是否打开自动缩进功能"));
    autoIndentAction->setCheckable(true);
    connect(autoIndentAction, SIGNAL(toggled(bool)),
            this, SLOT(switchAutoIndent(bool)));

    autoCompleAction = new QAction(QIcon(":/resources/autocomple.png"), tr("自动补全"), this);
    autoCompleAction->setShortcut(QKeySequence(Qt::ALT+Qt::Key_N));
    autoCompleAction->setStatusTip(tr("对光标前字符进行自动补全查找"));
    connect(autoCompleAction, SIGNAL(triggered()),
            autoComple, SLOT(doAutoComple()));

    quickCommentAction = new QAction(QIcon(":/resources/quickcomment.png"), tr("快速注释反注释"), this);
    quickCommentAction->setShortcut(QKeySequence(Qt::ALT+Qt::Key_M));
    quickCommentAction->setStatusTip(tr("快速注释或者反注释当前光标所在行的内容"));
    connect(quickCommentAction, SIGNAL(triggered()),
            codeEditor, SLOT(quickComment()));

    helpAction = new QAction(QIcon(":/resources/help.png"), tr("查看帮助(&H)"), this);
    helpAction->setShortcut(QKeySequence(Qt::Key_F1));
    helpAction->setStatusTip(tr("查看关于此程序的帮助文件"));
    connect(helpAction, SIGNAL(triggered()),
            this, SLOT(openHelp()));

    aboutAction = new QAction(tr("关于(&A)"), this);
    aboutAction->setStatusTip(tr("显示关于此程序的信息"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutDialog()));

}

void MainWindow::readSettings()
{
    QSettings settings("5-up", "cPad");//调用设置

    //读取 WrapMode 的枚举
    QPlainTextEdit::LineWrapMode wrapMode =
        static_cast<QPlainTextEdit::LineWrapMode>(
                settings.value("WrapMode", QPlainTextEdit::WidgetWidth).toUInt());
        //读取后设置 WrapMode
    codeEditor->setLineWrapMode(wrapMode);
    if (wrapMode == QPlainTextEdit::WidgetWidth)
        wrapAction->setChecked(true);
    else 
        wrapAction->setChecked(false);

    //读取窗口大小位置
    QPoint pos = settings.value("MainWindowPos", QPoint(200, 50)).toPoint();
    QSize size = settings.value("MainWindowSize", QSize(800, 600)).toSize();
    bool maxOpen = settings.value("MainWindowMaxed", "false").toBool();
    if (maxOpen)
        this->showMaximized();
    else {
        resize(size);
        move(pos);
    }


    //读取工具栏和状态栏开启状态
    bool visible;
    visible = settings.value("StatusBarVisible", "true").toBool();
    statusBarAction->setChecked(visible);
    statusBarVisible(visible);
    visible = settings.value("FileToolVisible", "true").toBool();
    fileToolAction->setChecked(visible);
    visible = settings.value("EditToolVisible", "true").toBool();
    editToolAction->setChecked(visible);
    visible = settings.value("ToolToolVisible", "true").toBool();
    toolToolAction->setChecked(visible);

    //读取语法高亮、高亮当前行、自动缩进开关
    bool open;
    open = settings.value("syntaxHighlighterVisible", "true").toBool();
    syntaxHighlightAction->setChecked(open);
    open = settings.value("highlightLineVisible", "true").toBool();
    highlightLineAction->setChecked(open);
    open = settings.value("AutoIndentSwitch", "true").toBool();
    autoIndentAction->setChecked(open);

    //读取当前目录
    currentDir = settings.value("CurrentDir", QDir().homePath()).toString();
    currentDir += "/.";//为了被吃掉末尾
    setCurrentDir(currentDir);

    //读取主题配色
    int colorNum;
    colorNum = settings.value("ColorThemeNumber", 2).toInt();
    colorThemeActions[colorNum]->setChecked(true);
    emit colorThemeChanged(colorNum);
}

void MainWindow::writeSettings()
{
    QSettings settings("5-up", "cPad");//调用设置
    
    //写入 WrapMode 的枚举
    QPlainTextEdit::LineWrapMode wrapMode = codeEditor->lineWrapMode();
    settings.setValue("WrapMode", (uint)wrapMode);

    //记录窗口大小位置
    if (!isMaximized()) {
        settings.setValue("MainWindowPos", pos());
        settings.setValue("MainWindowSize", size());
    }
    settings.setValue("MainWindowMaxed", this->isMaximized());

    //记录工具栏和状态栏开启状态
    settings.setValue("StatusBarVisible", statusBarAction->isChecked());
    settings.setValue("FileToolVisible", fileToolAction->isChecked());
    settings.setValue("EditToolVisible", editToolAction->isChecked());
    settings.setValue("ToolToolVisible", toolToolAction->isChecked());

    //记录高亮行、语法高亮、自动缩进开启状态
    settings.setValue("syntaxHighlighterVisible", syntaxHighlightAction->isChecked());
    settings.setValue("highlightLineVisible", highlightLineAction->isChecked());
    settings.setValue("AutoIndentSwitch", autoIndentAction->isChecked());

    //设置当前目录
    settings.setValue("CurrentDir", currentDir);

    //设置配色号
    for (int i = 0; i < MaxColorThemes; i++) {
        if (colorThemeActions[i]->isChecked())
            settings.setValue("ColorThemeNumber", i);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (mayBeSave()) {
        writeSettings();//关闭之前写入设置
        event->accept();
    } 
    else//保存失败取消关闭
        event->ignore();
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("文件(&F)"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    recentFileMenu = fileMenu->addMenu(QIcon(":/resources/recentfile.png"), tr("最近打开文件(&T)"));
    for (int i = 0; i < MaxRecentFiles; ++i)
        recentFileMenu->addAction(recentFileActions[i]);
    recentFileMenu->addSeparator();
    recentFileMenu->addAction(cleanRecentFileAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("编辑(&E)"));
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addSeparator();
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(selectAllAction);
    editMenu->addAction(deleteAction);
    editMenu->addSeparator();
    editMenu->addAction(findAction);
    editMenu->addAction(replaceAction);

    //menuBar()->addSeparator();
    visualMenu = menuBar()->addMenu(tr("查看(&V)"));
    //visualMenu->addAction(menuBarAction);
    visualMenu->addAction(statusBarAction);
    visualMenu->addAction(fileToolAction);
    visualMenu->addAction(editToolAction);
    visualMenu->addAction(toolToolAction);
    visualMenu->addSeparator();
    visualMenu->addAction(syntaxHighlightAction);
    visualMenu->addAction(highlightLineAction);
    visualMenu->addSeparator();
    colorThemeMenu = visualMenu->addMenu(QIcon(":/resources/color.png"), tr("主题配色"));
    for (int i = 0; i < MaxColorThemes; i++ ) 
        colorThemeMenu->addAction(colorThemeActions[i]);

    formatMenu = menuBar()->addMenu(tr("格式(&O)"));
    formatMenu->addAction(wrapAction);
    formatMenu->addAction(autoIndentAction);
    formatMenu->addSeparator();
    formatMenu->addAction(fontSettingAction);

    toolMenu = menuBar()->addMenu(tr("工具(&T)"));
    toolMenu->addAction(compileAction);
    toolMenu->addAction(compileAndRunAction);
    toolMenu->addAction(compileConfigAction);
    toolMenu->addSeparator();
    toolMenu->addAction(recomposeAction);
    toolMenu->addAction(functionListAction);
    toolMenu->addAction(functionCommentAction);
    toolMenu->addAction(quickCommentAction);
    toolMenu->addAction(autoCompleAction);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("帮助"));
    helpMenu->addAction(helpAction);
    helpMenu->addAction(aboutAction);
}


void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("文件工具栏"));
    fileToolBar->setIconSize(QSize(26,26));
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);

    editToolBar = addToolBar(tr("编辑工具栏"));
    editToolBar->setIconSize(QSize(26,26));
    editToolBar->addAction(undoAction);
    editToolBar->addAction(redoAction);
    editToolBar->addSeparator();
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
    editToolBar->addSeparator();
    editToolBar->addAction(findAction);
    editToolBar->addAction(replaceAction);

    toolToolBar = addToolBar(tr("功能工具栏"));
    toolToolBar->setIconSize(QSize(26,26));
    toolToolBar->addAction(compileAction);
    toolToolBar->addAction(compileAndRunAction);
    toolToolBar->addSeparator();
    toolToolBar->addAction(recomposeAction);
    toolToolBar->addAction(functionListAction);
    toolToolBar->addAction(functionCommentAction);
    toolToolBar->addAction(quickCommentAction);
    toolToolBar->addAction(autoCompleAction);
}

void MainWindow::createStatusBar()
{
    setStatusBar(statusBar);

    connect(codeEditor, SIGNAL(cursorPositionChanged()),
            statusBar, SLOT(updateTextCursorLabel()));

    connect(codeEditor, SIGNAL(blockCountChanged(int)),
            statusBar, SLOT(updateTextCursorLabel()));

    connect(this, SIGNAL(currentDirChanged(QString)),
            statusBar, SLOT(updateCurrentDirLabel(QString)));

    connect(this, SIGNAL(currentFileChanged(QString)),
            statusBar, SLOT(updateCurrentFileLabel(QString)));
}

void MainWindow::menuBarVisible(bool visible)
{
    menuBar()->setVisible(visible);
}

void MainWindow::statusBarVisible(bool visible)
{
    statusBar->setVisible(visible);
}

void MainWindow::fileToolVisible(bool visible)
{
    fileToolBar->setVisible(visible);
}

void MainWindow::editToolVisible(bool visible)
{
    editToolBar->setVisible(visible);
}

void MainWindow::toolToolVisible(bool visible)
{
    toolToolBar->setVisible(visible);
}

void MainWindow::openArgvFile(char *argv)
{
    if (mayBeSave()) {
        //设置转码来源
        QTextCodec *codec = QTextCodec::codecForLocale();
        //转至 Unicode
        QString str = codec->toUnicode(argv);
        loadFileContent(str);
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
    {
        return;
    }
    QString fileName = urls.first().toLocalFile();
    if (fileName.isEmpty())
    {
        return;
    }
    //QString filePath = QFileInfo(fileName).canonicalFilePath();

    if (mayBeSave()) {
        loadFileContent(fileName);
    }
}

void MainWindow::preCompile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    int code = 0;
    if (action)
        code = action->data().toInt();
    //弹出框 提示自动保存
    if ( ! preCompileSaveFile() )
        return;
    if (code)
        runCompile->showCompileAndRun(compileConfig->getCompilerDir(),
                                      compileConfig->getWorkingDir(),
                                      compileConfig->getArgs(),
                                      currentFile);
    else
        runCompile->showCompileOnly(compileConfig->getCompilerDir(),
                                    compileConfig->getWorkingDir(),
                                    compileConfig->getArgs(),
                                    currentFile);
}
bool MainWindow::preCompileSaveFile()
{
    //弹出框 提示自动保存
    if (codeEditor->document()->isModified() || currentFile.isEmpty()) {
        //弹出对话框询问是否需要保存
        int ret = QMessageBox::warning(this, tr("警告"),
                     tr("当前文件已经被修改\n"
                        "是否需要保存您的修改？"),
                     tr("保存并编译"),
                     tr("取消"),
                     0, 0);
        if (ret == 0) {//保存文件
            if (!saveFile())
                return false;
        }
        else if (ret == 1)//取消操作
            return false;
    }
    return true;
}

void MainWindow::compileButtonChecked()
{
    if ( preCompileSaveFile() )
        runCompile->showCompileAndRun(compileConfig->getCompilerDir(),
                                      compileConfig->getWorkingDir(),
                                      compileConfig->getArgs(),
                                      currentFile);
    return;
}

void MainWindow::openCompileconfig()
{
    compileConfig->openCompileConfig(currentDir);
}

void MainWindow::switchAutoIndent(bool ok)
{
    QSettings settings("5-up", "cPad");

    settings.setValue("AutoIndentSwitch", ok);
}

void MainWindow::openHelp()
{
    QString helpFile = QDir("doc/").absoluteFilePath("help.html");
    QDesktopServices::openUrl(QUrl(helpFile, QUrl::TolerantMode));
}
