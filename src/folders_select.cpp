#include "folders_select.h"


FoldersSelect::FoldersSelect(QWidget *parent)
    : QFileDialog(parent)
{

}

void FoldersSelect::go()
{
    QDialog::accept();
}
