#include "neural_network_card.h"
#include "ui_neural_network_card.h"

Neural_network_card::Neural_network_card(int id_nn, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Neural_network_card)
{
    this->id_nn = id_nn;
    ui->setupUi(this);
    this->setParent(0);
    this->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint); //Set window to fixed size
    this->setWindowFlags(Qt::CustomizeWindowHint); //Set window with no title bar
    this->setWindowFlags(Qt::FramelessWindowHint); //Set a frameless window
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setMouseTracking(true);
    connect(ui->learning_rate, SIGNAL(textEdited(QString)), this, SLOT(learning_rate()));
    connect(ui->batch_size, SIGNAL(valueChanged(int)), this, SLOT(batch_size()));
    vector<string> request = Database::read("SELECT clustering_rate, positive_classifed_well, positive_misclassified, negative_classified_well, negative_misclassified FROM neural_networks "
                                            "WHERE id = "+ to_string(id_nn) + " AND id_combo = (SELECT id FROM combos)");
    ui->title->setText("TOTO");//name_row_nn.c_str()
    ui->clustering_rate->setText((QString)request[0].c_str() + "%");

    int total_p = stoi(request[1]) + stoi(request[2]);
    int total_n = stoi(request[3]) + stoi(request[4]);
    int total_c_w = stoi(request[1]) + stoi(request[3]);
    int total_total = total_p + total_n;

    ui->positive_cr->setText(QString::fromStdString(request[1] + "/" + to_string(total_p)));

    ui->negative_cr->setText(QString::fromStdString(request[3] + "/" + to_string(total_n)));

    ui->cr->setText(QString::fromStdString(to_string(total_c_w) + "/" + to_string(total_total)));

    request = Database::read("SELECT COUNT(*) FROM perceptrons "
                                "WHERE id_nn = "+ to_string(id_nn) + " AND id_layer = 0");

    ui->number_of_neurons_in_hidden_layer->setText(QString::fromStdString(request[0]));

   request = Database::read("SELECT learning_rate FROM perceptrons "
                                    "WHERE id_nn = "+ to_string(id_nn) + " AND id_layer = 0 LIMIT 1");
    ui->learning_rate->setText(request[0].c_str());

    request = Database::read("SELECT batch_size FROM neural_networks "
                                    "WHERE id = "+ to_string(id_nn) + " LIMIT 1");
    ui->batch_size->setValue(atoi(request[0].c_str()));

    ui->warning->setVisible(false);
}

Neural_network_card::~Neural_network_card()
{
    ;
}

bool Neural_network_card::event(QEvent *event)
{
    if(event->type() == QEvent::WindowDeactivate)
    {
        this->~Neural_network_card();
    }
    else if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *e = static_cast<QKeyEvent *>(event);
        if(e->key() == Qt::Key_Escape)
            this->~Neural_network_card();
    }
    else if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        if( e->buttons() == Qt::LeftButton ) //dragging
            move( e->globalPos() - QPoint(width()/2, height()/2) );
    }
    else if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        clickPos = e->pos();
    }
    return QWidget::event(event);
}

void Neural_network_card::learning_rate()
{
    Database::write("UPDATE perceptrons SET learning_rate = " + ui->learning_rate->text().toStdString() + " "
                    "WHERE id_nn = " + to_string(id_nn));
    ui->warning->setVisible(true);
}

void Neural_network_card::batch_size()
{
    Database::write("UPDATE neural_networks SET batch_size = " + to_string(ui->batch_size->value()) + " "
                    "WHERE id = "+ to_string(id_nn));
    ui->warning->setVisible(true);
}

