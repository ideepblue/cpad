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
#ifndef COMPILECONFIG_H
#define COMPILECONFIG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QGridLayout;
class QGroupBox;
class QPushButton;
class QLabel;
class QLineEdit;
class QDir;
QT_END_NAMESPACE

class CompileConfig : public QDialog
{
    Q_OBJECT
public:
    CompileConfig(QWidget *parent);
    ~CompileConfig();
    void createWidget();
    void generalSetting();
    QString getCompilerDir();
    QStringList getArgs();
    QString getWorkingDir();
    void autoSetWorkingDir(QString wkDir, QString fn);

signals:
    void configChanged(QString getCompiler, QString getWorkingDir, QStringList getArgs);

public slots:
    void showDialog();
    void closeDialog();
    void setCompileConfig();
    void openCompileConfig(QString fPath);
    void openExeFile();
    void openDir();

private:
    void readSettings();
    void writeSettings();
    QGridLayout *inLayout, *mainLayout;
    QGroupBox * groupBox;
    QPushButton *yesButton, *noButton, *browserFileButton, *browserDirButton;
    QLabel *compilePositionLabel, *tempPositionLabel, *compileArgsLabel;
    QLineEdit *compilePositionEdit, *tempPositionEdit, *compileArgsEdit;
    QString compilerFile;
    QStringList args;
    QString workingDir;
};

#endif // COMPILECONFIG_H
