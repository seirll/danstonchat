#include <QApplication>
#include <QString>
#include <QApplication>
#include <QtNetwork>
#include <QtGui>
#include "FenMaj.h"
#include "FenPrincipale.h"
#include "ui_FenMaj.h"

FenMaj::FenMaj(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    erreurRecue = false;
}

void FenMaj::defaultTab(int number)
{
    tabWidget->setCurrentIndex(number);
}

void FenMaj::on_boutonMaj_clicked()
{
    const QUrl URL= QUrl ("http://danstonchat.com/fortunes");
    const QNetworkRequest requette(URL);
    QNetworkAccessManager *m = new QNetworkAccessManager;
    QNetworkReply *r = m->get(requette);
    connect(r, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(messageErreur(QNetworkReply::NetworkError)));
    connect(r, SIGNAL(finished()), this, SLOT(enregistrer()));
    connect(r, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(progressionDownload(qint64, qint64) ));
    boutonMaj->setEnabled(false);
}

void FenMaj::progressionDownload(qint64 bytesReceived, qint64 bytesTotal)
{
	progress->setRange(0, bytesTotal);
	progress->setValue(bytesReceived);
	label_octets->setText(QString::number(bytesReceived));
}

void FenMaj::messageErreur(QNetworkReply::NetworkError code)
{
    Q_UNUSED(code);
    erreurRecue = true;
}

void FenMaj::enregistrer()
{
    QNetworkReply *r = qobject_cast<QNetworkReply*>(sender());
    if(r->error() != QNetworkReply::NoError)
    {
        QMessageBox::critical(this, "Erreur", "Erreur lors du chargement. Avez vous entré le nom correctement ?<br />Vérifiez votre connexion internet ou réessayez plus tard <br /><br /> Code de l'erreur : <br /><em>" + r->errorString() + "</em>");
    } else {
    QFile f(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "fortunes.txt");
    f.open(QIODevice::WriteOnly);
    f.write(r->readAll());
    f.close();
    r->deleteLater();
	QMessageBox::information(this, "Fin de téléchargement", "Téléchargement terminé !");
	close();
    }
}


void FenMaj::on_downloadFavoris_clicked()
{
    pseudo = nomCompte->text();
    const QUrl URL= QUrl ("http://danstonchat.com/bookmarks/" + pseudo + ".xml");
    const QNetworkRequest requette(URL);
    QNetworkAccessManager *m = new QNetworkAccessManager(this);
    QNetworkReply *r = m->get(requette);
    connect(r, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(messageErreur(QNetworkReply::NetworkError)));
    connect(r, SIGNAL(finished()), this, SLOT(enregistrerFav()));
    connect(r, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(progressionDownloadFav(qint64, qint64) ));
    boutonMaj->setEnabled(false);
}

void FenMaj::progressionDownloadFav(qint64 bytesReceived, qint64 bytesTotal)
{
	progressDownloadFav->setRange(0, bytesTotal);
	progressDownloadFav->setValue(bytesReceived);
	label_octetsFav->setText(QString::number(bytesReceived));
}

void FenMaj::enregistrerFav()
{
    if (!erreurRecue)
    {
        QNetworkReply *r = qobject_cast<QNetworkReply*>(sender());
        QFile f(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + pseudo + ".xml");
        f.open(QIODevice::WriteOnly);
        f.write(r->readAll());
        f.close();
        r->deleteLater();

        QFile xml(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + pseudo + ".xml");
        xml.open(QIODevice::ReadOnly);

        QFile fileFavoris(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "favoris.txt");
        fileFavoris.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream fluxFav(&fileFavoris);

        QRegExp titleExp ("<title>([0-9]*)</title>", Qt::CaseInsensitive);
        bool debut = true;
        while (!xml.atEnd())
        {
            int pos = titleExp.indexIn(xml.readLine());
            if (pos > -1)
            {
                if (!debut)
                    fluxFav<<"\r\n";
                fluxFav<<titleExp.cap(1);
                debut = false;
            }
        }

        xml.close();
        xml.remove();
        fileFavoris.close();
        QMessageBox::information(this, "Fin de téléchargement", "Téléchargement terminé !");
        close();
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer la liste des favoris. Vérifiez que vous avez entré le nom correctement ainsi que l'état de votre connexion internet.");
        close();
    }
}
