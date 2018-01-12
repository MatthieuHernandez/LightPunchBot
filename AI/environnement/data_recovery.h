#ifndef IA_H
#define IA_H

#include "player.h"
#include <time.h>
#include <QSettings>

class Data_recovery
{
    private:
    bool is_reading = false;

    public:
        Data_recovery();
        ~Data_recovery();
        void find_values(int version);
        void save_addresses();
        void load_addresses();
};

#endif // IA_H
