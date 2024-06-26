#include "mainwindow.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStringList languages = QLocale().uiLanguages();
    if (languages.size()) {
        QString language = languages.front().split("-").front();
        QString qmFile = QString("../Akiri/akiri_%1.qm").arg(language);
        if (QFileInfo(qmFile).exists()) {

            qDebug()<<"File " << qmFile << " Exist ! ";
            QTranslator * translator = new QTranslator(&a);
            if (translator->load(qmFile)) {
                a.installTranslator(translator);
                }
            else {
                translator->deleteLater();
            }
            translator = new QTranslator(&a);
            if (translator->load(QString("qt_%1").arg(language), QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
                a.installTranslator(translator);
            }
            else {
                translator->deleteLater();
            }
            }
        else { qDebug()<<"Translation File does not Exist ! ";}

    }
    MainWindow w;
    w.show();
    return a.exec();
}
