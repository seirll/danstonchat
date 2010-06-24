#include <QApplication>
#include <QtGui>
#include <time.h>
#include "ui_FenPrincipale.h"
#include "FenPrincipale.h"



int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication app(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));

    FenPrincipale fenetre;
    fenetre.show();
    fenetre.beginQuote();

    return app.exec();
}


long nombreAleatoire(long nombreMax)
{
    return (rand() % (nombreMax + 1));
}
