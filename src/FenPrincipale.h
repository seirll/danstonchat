#ifndef HEADER_FENCLIENT
#define HEADER_FENCLIENT

#include <QtGui>
#include "ui_FenPrincipale.h"

long nombreAleatoire(long nombreMax);

class FenPrincipale : public QWidget, private Ui::FenPrincipale
{
    Q_OBJECT

    public:
        FenPrincipale(QWidget *parent = 0);
        void beginQuote();

    private slots:
        void on_boutonQuote_clicked();
        void on_boutonAbout_clicked();
        void on_boutonMaj_clicked();
        void on_boutonFavori_clicked();
        void on_goQuote_clicked();
        void on_ouvrirSite_clicked();
        void on_checkRunStart_toggled(bool runStartup);
        void ouvrirMajFav();

    private:
        Ui::FenPrincipale ui;
        QString htmlspecialchars(QString string);
        void goQuoteById(QString numberQuoteToGo);

};


#endif
