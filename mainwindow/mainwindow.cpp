#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::MainWindow),
          m_check_flag(false) {
    ui->setupUi(this);

    setWindowIcon(QIcon(":/image/resource/icon2.ico"));

    slotsConnect();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::slotsConnect() {
    connect(&m_folder_sync, &FolderSync::signalTextBrowserPrint, this, &MainWindow::slotTextBrowserPrint);

    connect(&m_folder_sync, &FolderSync::signalFindDiffCompleted, this, [=]() {
        m_check_flag.store(true);
    });

    connect(&m_folder_sync, &FolderSync::signalUpdateCompleted, this, [=]() {
        m_check_flag.store(false);
    });
}

void MainWindow::slashLeftToRight(QString &str) {
    QString temp = "";

    for (auto &i : str) {
        if (i == '/') {
            temp += '\\';
        } else {
            temp += i;
        }
    }

    str = std::move(temp);
}

void MainWindow::on_src_pushButton_clicked() {
    m_src_path = QFileDialog::getExistingDirectory(this, "Select source directory", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!m_src_path.isEmpty()) {
        m_check_flag.store(false);

        slashLeftToRight(m_src_path);
        QString src_path_display = "\"" + m_src_path + "\"";
        ui->src_lineEdit->setText(src_path_display);
    }
}

void MainWindow::on_dst_pushButton_clicked() {
    m_dst_path = QFileDialog::getExistingDirectory(this, "Select destination directory", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!m_dst_path.isEmpty()) {
        m_check_flag.store(false);

        slashLeftToRight(m_dst_path);
        QString dst_path_display = "\"" + m_dst_path + "\"";
        ui->dst_lineEdit->setText(dst_path_display);
    }
}

void MainWindow::on_check_pushButton_clicked() {
    if ((m_src_path != "") && (m_dst_path != "")) {
        std::string src_path = m_src_path.toStdString();
        std::vector<std::string> dst_paths{m_dst_path.toStdString()};
        m_folder_sync.setPath(src_path, dst_paths);

        std::thread find_diff_Thread(&FolderSync::findDiff, &m_folder_sync);
        find_diff_Thread.detach();
    }
}

void MainWindow::on_update_pushButton_clicked() {
    if (m_check_flag.load()) {
        std::thread update_Thread(&FolderSync::update, &m_folder_sync);
        update_Thread.detach();
    } else {
        slotTextBrowserPrint("The \"check\" operation must be performed before the \"update\" operation!\n");
    }
}

void MainWindow::slotTextBrowserPrint(QString info) {
    ui->output_textBrowser->append(info);
}
