#include "balayagedialog.h"


BalayageDialog::BalayageDialog(BalaiFrequenciel* b) {
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(b);
    setLayout(layout);
}

void BalayageDialog::show() {
    QDialog::show();
}
