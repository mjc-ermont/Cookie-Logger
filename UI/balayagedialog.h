#ifndef BALAYAGEDIALOG_H
#define BALAYAGEDIALOG_H

#include <balaifrequenciel.h>
#include <QDialog>
#include <QGridLayout>

class BalayageDialog : QDialog {
    public:
        BalayageDialog(BalaiFrequenciel* b);
        void show();
};


#endif // BALAYAGEDIALOG_H
