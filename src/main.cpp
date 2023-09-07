#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QLocale>
#include <QTranslator>
#include <windows.h>


int main(int argc, char *argv[]) {

#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    // 适配高清屏幕
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    // 设置字符编码为UTF-8
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "FolderSync_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;
    w.show();

    return a.exec();
}
