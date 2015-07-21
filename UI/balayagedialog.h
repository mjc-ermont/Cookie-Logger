#ifndef BALAYAGEDIALOG_H
#define BALAYAGEDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <balaifrequenciel.h>

class BalayageDialog : QDialog {
    public:
        BalayageDialog(BalaiFrequenciel* b);
        void show();
};


#endif // BALAYAGEDIALOG_H
