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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "centerwidget.h"//MainWindow 中心控件头文件
#include "functionlistarea.h"//函数折叠控件头文件
#include "findreplacearea.h"//替换控件头文件
#include "statusbar.h"//状态栏头文件
#include "recomposedialog.h"//重排版对话框
#include "cpadpalette.h" //调色类
#include "autocomple.h"//自动补全
#include "runcompile.h"//编译
#include "compileconfig.h" //编译器信息

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
        Q_OBJECT
    public:
        MainWindow(QWidget *parent = 0);
        void openArgvFile(char *file);//打开作为参数传进的文件

    protected:
        void closeEvent(QCloseEvent *event);//关闭事件函数
        void dragEnterEvent(QDragEnterEvent *event);//拖进事件
        void dropEvent(QDropEvent *event);//放下事件

    signals:
        void currentFileChanged(QString currentFile);//当前文件改变信号
        void currentDirChanged(QString currentDir);//当前目录改变信号
        void colorThemeChanged(int num);//发送当前主题配色

    private slots:
        void newFile();//新建文件函数
        void openFile();//打开文件函数
        bool saveFile();//保存文件函数
        bool saveAsFile();//另存为文件函数
        void aboutDialog();//关于对话框函数
        void documentWasModified();//判断编辑器是否有被修改 在标题栏有 * 号显示
        void openRecentFile();//打开最近文件
        void cleanRecentFile();//清除最近打开文件
        void statusBarVisible(bool visible);//设置显示状态栏
        void menuBarVisible(bool visible);//设置显示菜单栏
        void fileToolVisible(bool visible);//设置文件工具栏可视
        void editToolVisible(bool visible);//设置编辑工具栏可视
        void toolToolVisible(bool visible);//设置功能工具栏可视
        void openColorTheme();//打开主题配色
        void preCompile();//编译前的准备工作
        bool preCompileSaveFile();
        void openCompileconfig();
        void compileButtonChecked();
        void switchAutoIndent(bool ok);//自动缩进的开关
        void openHelp();

    private:
        void createActions();//建立所有菜单项函数
        void createMenus();//建立所有菜单函数
        void createToolBars();//建立工具栏函数
        void createStatusBar();//建立状态栏函数
        void generalSetting();//主窗体的一般设置
        void readSettings();//程序所有设定的读取
        void writeSettings();//程序所有设定的写入
        bool mayBeSave();//判断编辑器内容是否需要保存
        void loadFileContent(const QString &fileName);//读取文件内容
        bool saveFileContent(const QString &fileName);//写入文件内容
        void setCurrentFile(const QString &fileName);//设置当前文件名
        void setCurrentDir(const QString &fileName);//设置当前路径
        void updateRecentFile();//更新最近文件菜单项
        QString strippedName(const QString &fullFileName);//取文件名
        void transTabToSpace(QString &codeString);//把所有 tab 变空格

        QString currentFile;//当前文件名
        QString currentDir;//当前路径

        QMenu *fileMenu;//文件菜单
        QAction *newAction;//新建菜单项
        QAction *openAction;//打开菜单项
        QAction *saveAction;//保存菜单项
        QAction *saveAsAction;//另存为菜单项
        QMenu   *recentFileMenu;//最近文件子菜单
        enum { MaxRecentFiles = 5 };//最近文件最大数目
        QAction *recentFileActions[MaxRecentFiles];//最近文件菜单项
        QAction *cleanRecentFileAction;//清楚最近打开文件
        QAction *exitAction;//退出菜单项

        QMenu *editMenu;//编辑菜单
        QAction *undoAction;//撤销菜单项
        QAction *redoAction;//重做菜单项
        QAction *cutAction;//剪切菜单项
        QAction *copyAction;//复制菜单项
        QAction *pasteAction;//粘贴菜单项
        QAction *selectAllAction;//全选菜单项
        QAction *deleteAction;//删除菜单项
        QAction *findAction;//查找菜单项
        QAction *replaceAction;//替换菜单项

        QMenu *visualMenu;//视图菜单
        QAction *statusBarAction;//状态栏开关项
        QAction *fileToolAction;//文件工具栏
        QAction *editToolAction;//编辑工具栏
        QAction *toolToolAction;//功能工具栏
        QMenu *colorThemeMenu;//主题配色
        enum { MaxColorThemes = 6 };//主题配色数量
        QAction *colorThemeActions[MaxColorThemes];//配色子菜单
        QActionGroup *colorThemeGroup;//主题配色组
        
        QMenu *formatMenu;//格式菜单
        QAction *wrapAction;//自动换行菜单项
        QAction *fontSettingAction;//字体菜单项

        QMenu *toolMenu;//工具
        QAction *compileAction;//编译菜单项
        QAction *compileAndRunAction;//编译并运行
        QAction *compileConfigAction;//编译设置
        QAction *syntaxHighlightAction;//高亮菜单项
        QAction *highlightLineAction;//行号菜单项
        QAction *recomposeAction;//重排版菜单项
        QAction *functionListAction;//函数列表菜单项
        QAction *functionCommentAction;//函数注释菜单项
        QAction *autoIndentAction;//自动缩进菜单项开关
        QAction *autoCompleAction;//自动补全菜单项
        QAction *quickCommentAction;//快速注释反注释菜单项


        QMenu *helpMenu;//帮助菜单
        QAction *helpAction;//帮助菜单项
        QAction *aboutAction;//关于菜单项

        QToolBar *fileToolBar;//文件工具栏
        QToolBar *editToolBar;//编辑工具栏
        QToolBar *toolToolBar;//工具工具栏

        CenterWidget *centerWidget;//中心控件
        FunctionListArea *functionListArea;//函数折叠区域控件
        FindReplaceArea *findReplaceArea;//查找替换区域控件
        CodeEditor *codeEditor;//编辑器控件
        StatusBar *statusBar;//状态栏
        RecomposeDialog *recomposeDialog;//重排版对话框
        CpadPalette * cpadPalette;
        AutoComple *autoComple;//自动补全
        RunCompile *runCompile;//编译
        CompileConfig *compileConfig;
};

#endif // MAINWINDOW_H
