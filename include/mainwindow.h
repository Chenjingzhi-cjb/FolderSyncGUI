#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QFileDialog>
#include <QTextCodec>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
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

private:
    void slotsConnect();

    void slashLeftToRight(QString &str);

private slots:
    void on_src_pushButton_clicked();

    void on_dst_pushButton_clicked();

    void on_check_pushButton_clicked();

    void on_update_pushButton_clicked();

    void slotTextBrowserPrint(QString info);

private:
    Ui::MainWindow *ui;

    FoldersSelect *fs;

    QString m_src_path{};

    QStringList m_dst_paths{};

    FolderSync m_folder_sync;
    std::atomic_bool m_check_flag;  // 保证在执行完“查询”操作后才能执行“更新”操作
};


#endif // MAINWINDOW_H
