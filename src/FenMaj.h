#ifndef HEADER_FENMAJ
#define HEADER_FENMAJ

#include <QtGui>
#include <QNetworkReply>
#include <QtNetwork>
#include "ui_FenMaj.h"

class FenMaj : public QDialog, private Ui::FenMaj
{
    Q_OBJECT

    public:
        FenMaj(QWidget *parent = 0);
        void defaultTab(int number);

    private slots:
		void on_boutonMaj_clicked();
		void progressionDownload(qint64, qint64);
		void progressionDownloadFav(qint64, qint64);
		void messageErreur(QNetworkReply::NetworkError);
		void enregistrer();
		void enregistrerFav();
		void on_downloadFavoris_clicked();

    private:
        Ui::FenMaj ui;
        qint64 bytesTotalLus;
        QString pseudo;
        bool erreurRecue;
};

#endif
