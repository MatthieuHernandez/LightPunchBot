#ifndef NEURAL_NETWORK_MANAGER_H
#define NEURAL_NETWORK_MANAGER_H

#include <QVector>
#include <QTimer>
#include <QMutex>

#include <parameters.h>
#include <gui/thread.h>
#include <environnement/database_manager.h>
#include <ai/neural_network.h>


class Neural_network_manager
{
    private:

        Neural_network_manager();
        ~Neural_network_manager();
        static vector<Neural_network> neural_networks;
        static vector<Neural_network> neural_networks_temp;

        static void calcul_average_damage();

        int id_combo_ccr;
        int id_ccr;
        bool is_training_ccr;
        int offset_crr;
        bool bool_ccr;

        //QMutex mutex;
        //static vector<QTimer*> timers;

        //static vector<string> request_for_test;

    public:


        static QVector<double> learning_rate;

        static Neural_network_manager* instance();

        static void add();
        static void remove(int id);
        static void train(int id);
        static void train_all();
        static void save();
        static void load();
        static void test();
        static void clean();
        static vector<Neural_network> select_and_sort();

        static void training(int nn_id, state learning_state);
        static void calcul_clustering_rate(int nn_id, vector<state> testing_states);

        static void calcul_all_clustering_rate();

        static unsigned int get_number_of_neural_network();
        static const Neural_network* const get_neural_network(int id);

    //public slots:

        //void calcul_clustering_rate();



};

#endif // NEURAL_NETWORK_MANAGER_H
