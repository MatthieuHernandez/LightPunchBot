#ifndef NEURAL_NETWORK_CARD_H
#define NEURAL_NETWORK_CARD_H

#include <QFrame>
#include <QMouseEvent>
#include <QKeyEvent>
#include <qdebug.h>

#include <environnement/database.h>

namespace Ui {
class Neural_network_card;
}

class Neural_network_card : public QFrame
{
    Q_OBJECT

    public:
        explicit Neural_network_card(int id_nn, QWidget *parent = 0);
        ~Neural_network_card();

    private:
        Ui::Neural_network_card *ui;
        QPoint clickPos;
        int id_nn;

    protected:
        virtual bool event(QEvent *event);

    private slots:
        void learning_rate();
        void batch_size();

};

#endif // NEURAL_NETWORK_CARD_H
