#include <QApplication>
#include <QString>
#include <QtGui>
#include "FenPrincipale.h"
#include "ui_FenPrincipale.h"
#include "FenMaj.h"

FenPrincipale::FenPrincipale(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    ouvrirSite->setCursor(Qt::PointingHandCursor);
    ouvrirSite->setStyleSheet("background-image: url(':/Images/dtc.png')");

    QDir dir;
    dir.mkpath(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/danstonchat/");

    #ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QString path=QCoreApplication::applicationFilePath();
    path.replace('/', '\\');

    if (settings.value("DansTonChat") == path)
        checkRunStart->setChecked(true);

    #else
    checkRunStart->hide();
    #endif
}

void FenPrincipale::beginQuote()
{
    on_boutonQuote_clicked();
}

void FenPrincipale::on_ouvrirSite_clicked()
{
    QDesktopServices::openUrl(QUrl("http://danstonchat.com"));
}

void FenPrincipale::on_boutonQuote_clicked()
{
    int count_nb_quotes = 0;
    QFile file(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/danstonchat/fortunes.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) //si tu ne peux lire le fichier
    {
        QMessageBox::critical(this, "Erreur", "Erreur à l'ouverture du fichier.<br />Faites une Mise à Jour pour télécharger les citations.");
        FenMaj fenetreMajfav(this);
        fenetreMajfav.exec();
    }
    else
    {

        //On compte le nombre de quotes
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            if (line == "%")
                count_nb_quotes++;
        }
        labelNbQuotes->setText(QString::number(count_nb_quotes));
        file.reset();
        //Permet de remettre les flags du fichier à zéro, la lecture recommence au début.

        int ligneQuoteChoisie = nombreAleatoire(count_nb_quotes-1);
        QString mot;
        int count = 0;

        //On positionne le curseur au bon endroit
        QString osef;
        while (count < ligneQuoteChoisie)
        {
            osef = in.readLine();
            if (osef == "%")
                count++;
        }
        QRegExp dtcExp ("^-- http://danstonchat.com/");
        QString lignelue;
        while (lignelue != "%")
        {
            lignelue = in.readLine();
            if ((lignelue != "%")&&(!lignelue.contains(dtcExp)))
                textEditQuote->append(htmlspecialchars(lignelue));
            if (lignelue.contains(dtcExp))
            {
                uint taille = lignelue.length(); //Taille de l'ID (on enleve les 26 premiers caractères et les 5 ".html")
                QString idQuote = lignelue.mid(26, taille -31); //On vire les 26 permiers caractères et les 5 de la fin
                QString urlQuote = lignelue.mid(3, taille); //On vire les 3 premiers caractères
                labelIDQuote->setText("<a href='" + urlQuote + "'>#" + idQuote + " (Voir les commentaires)</a>"); //On marque l'ID de la quote
            }
        }
        file.close();
    }
}


void FenPrincipale::on_goQuote_clicked()
{
    QString numberQuoteToGo = lineEditNumeroQuote->text();
    goQuoteById(numberQuoteToGo);
}


void FenPrincipale::on_boutonAbout_clicked()
{
    QMessageBox::information(this, "À propos", "Ce logiciel a été conçu par Serialtueur avec l'accord de Remouk, programmeur du site DansTonChat (http://danstonchat.com).<br /><br />Enjoy ! \\o/");
}

void FenPrincipale::on_boutonMaj_clicked()
{
    FenMaj fenetreMaj(this);
    fenetreMaj.exec();
}

void FenPrincipale::on_checkRunStart_toggled(bool runStartup)
{
    if (runStartup == true)
    {
        QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
        QString path=QCoreApplication::applicationFilePath();
        path.replace('/', '\\');
        settings.setValue("DansTonChat", path);
    }
    else
    {
        QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
        settings.remove("DansTonChat");
    }
}

void FenPrincipale::goQuoteById(QString numberQuoteToGo)
{
    QFile file(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/danstonchat/fortunes.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) //si tu ne peux lire le fichier
    {
        QMessageBox::critical(this, "Erreur", "Erreur à l'ouverture du fichier.<br />Faites une Mise à Jour pour télécharger les citations.");
        FenMaj fenetreMajfav(this);
        fenetreMajfav.exec();
    }
    else
    {

        //On compte le nombre de quotes
        QTextStream in(&file);
        QString string = in.readAll();

        int end = string.indexOf(QString("\n-- http://danstonchat.com/%1.html").arg(numberQuoteToGo));
        int begin = string.lastIndexOf("\n%\n", end) + 3;
        QString text = string.mid(begin, end - begin);

        if (!text.isEmpty())
        {
            textEditQuote->clear();
            textEditQuote->append(text);
            labelIDQuote->setText("<a href='http://danstonchat.com/" + numberQuoteToGo + ".html'>#" + numberQuoteToGo + " (Voir les commentaires)</a>"); //On marque l'ID de la quote
        }
        else
        {
            QMessageBox::critical(this, "Erreur", "Cette citation n'existe pas. Vérifiez que vous avez correctement entré le n° ou faites une mise à jour.");
        }
        file.close();
    }

}


void FenPrincipale::on_boutonFavori_clicked()
{
     int count_nb_lignes = 0;
     QFile file(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/danstonchat/favoris.txt");
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
     {
        QMessageBox::critical(this, "Erreur", "Aucun favori n'a été trouvé. Veuillez faire une Mise à Jour.");
        ouvrirMajFav();
        return;
     }

     //On compte le nombre de lignes
     QTextStream in(&file);
     while (!in.atEnd())
     {
        count_nb_lignes++;
        in.readLine();
     }
     file.reset();

     int ligneMotChoisi = nombreAleatoire(count_nb_lignes-1);
     QString idQuote;

     for(int count = 0 ; count <= ligneMotChoisi ; count++)
        idQuote = in.readLine();

    file.close();
    goQuoteById(idQuote);

}


QString FenPrincipale::htmlspecialchars(QString string)
{
    QString str = string;
    str.replace("<", "&lt;");

    return str;
}

void FenPrincipale::ouvrirMajFav()
{
    FenMaj fenetreMajfav(this);
    fenetreMajfav.defaultTab(1);
    fenetreMajfav.exec();
}
