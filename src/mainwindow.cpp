#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::MainWindow),
          fs(new FoldersSelect),
          m_check_flag(false) {
    ui->setupUi(this);

    setWindowIcon(QIcon(":/image/resource/icon2.ico"));

    fs->setOption(QFileDialog::DontUseNativeDialog,true);
    //支持多选
    QListView *listView = fs->findChild<QListView*>("listView");
    if (listView) listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QTreeView *treeView = fs->findChild<QTreeView*>();
    if (treeView) treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QDialogButtonBox *button = fs->findChild<QDialogButtonBox *>("buttonBox");
    // 更改槽函数
    disconnect(button, SIGNAL(accepted()), fs, SLOT(accept()));
    connect(button, SIGNAL(accepted()), fs, SLOT(go()));
    // 设置
    fs->setNameFilter("Folders (*._)");

    slotsConnect();
}

MainWindow::~MainWindow() {
    delete ui;
    delete fs;
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
    if(fs->exec()==QDialog::Accepted) {
        m_check_flag.store(false);

        m_dst_paths = fs->selectedFiles();

        if (!m_dst_paths.isEmpty()) {
            QString dst_paths_display = "";
            for (auto &i : m_dst_paths) {
                slashLeftToRight(i);
                dst_paths_display += "\"";
                dst_paths_display += i;
                dst_paths_display += "\"";
                dst_paths_display += ",";
            }
            dst_paths_display = dst_paths_display.mid(0, dst_paths_display.length() - 1);

            ui->dst_lineEdit->setText(dst_paths_display);
        }
    }
}

void MainWindow::on_check_pushButton_clicked() {
    if ((m_src_path != "") && (!m_dst_paths.isEmpty())) {
        std::string src_path = m_src_path.toStdString();
        std::vector<std::string> dst_paths;
        for (auto &i : m_dst_paths) {
            dst_paths.emplace_back(i.toStdString());
        }
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

