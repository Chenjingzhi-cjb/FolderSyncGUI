#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QFileDialog>
#include <QTextCodec>
#include <string>
#include <vector>
#include "folders_select.h"
#include "folder_sync_qt.hpp"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_src_pushButton_clicked();

    void on_dst_pushButton_clicked();

    void on_check_pushButton_clicked();

    void on_update_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    FoldersSelect *fs;

    QString m_src_path{};

    QStringList m_dst_paths{};
};


#endif // MAINWINDOW_H