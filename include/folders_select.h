#ifndef FOLDERS_SELECT_H
#define FOLDERS_SELECT_H

#include <QWidget>
#include <QFileDialog>
#include <QListView>
#include <QTreeView>
#include <QDialogButtonBox>


class FoldersSelect :public QFileDialog {
    Q_OBJECT

public:
    explicit FoldersSelect(QWidget *parent = 0);

public slots:
    void go();
};


#endif // FOLDERS_SELECT_H
