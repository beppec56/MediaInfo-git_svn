// GUI of MediaInfo
// Copyright (C) 2011-2012 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "translate.h"
#include "editsheet.h"
#include "_Automated/ui_editsheet.h"
#include "sheet.h"
#include "columneditsheet.h"
#include "QtGui/QHBoxLayout"
#include "QtGui/QLabel"
#include "QtGui/QLineEdit"
#include "QtGui/QSpinBox"
#include "QtGui/QPushButton"
#include "QtGui/QToolButton"
#include "QtGui/QSizePolicy"
#include "QtGui/QComboBox"

EditSheet::EditSheet(Sheet* s, Core* C, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditSheet)
{
    ui->setupUi(this);
    this->sheet = s;
    this->C = C;

    initDisplay();
    refreshDisplay();
    connect(ui->pushButton,SIGNAL(clicked()),SLOT(addColumn()));
    ui->checkBoxAdapt->setChecked(s->getAdaptColumns());
}

EditSheet::~EditSheet()
{
    delete ui;
}

void EditSheet::initDisplay() {
    ui->lineEdit->setText(sheet->getName());
    for(int i=0;i<sheet->getNbColumns();i++) {
        ui->vboxLayout->addLayout(createColumn(sheet->getColumn(i)));
        emit newPos(ui->vboxLayout->count());
    }
    ui->tableWidget->setRowCount(1);
}

void EditSheet::refreshDisplay() {
    qDebug() << ui->vboxLayout->count() << " columns :";
    ui->tableWidget->setColumnCount(ui->vboxLayout->count());
    for(int i=0;i<ui->vboxLayout->count();++i) {
        //column c = sheet->getColumn(i);
        qDebug() << i << " : " << ((QLineEdit*)ui->vboxLayout->itemAt(i)->layout()->itemAt(0)->widget())->text();
        if(!ui->checkBoxAdapt)
            ui->tableWidget->setColumnWidth(i,((QSpinBox*)ui->vboxLayout->itemAt(i)->layout()->itemAt(1)->widget())->value());
        if(ui->tableWidget->horizontalHeaderItem(i))
            ui->tableWidget->horizontalHeaderItem(i)->setText(((QLineEdit*)ui->vboxLayout->itemAt(i)->layout()->itemAt(0)->widget())->text());
        else {
            QTableWidgetItem* header = new QTableWidgetItem(((QLineEdit*)ui->vboxLayout->itemAt(i)->layout()->itemAt(0)->widget())->text());
            ui->tableWidget->setHorizontalHeaderItem(i,header);
        }
    }
    if(ui->checkBoxAdapt)
        ui->tableWidget->resizeColumnsToContents();
}

QLayout* EditSheet::createColumn(column c) {
    ColumnEditSheet* col = new ColumnEditSheet(c,ui->vboxLayout->count(),ui->vboxLayout->count()+1,C);

    connect(col,SIGNAL(somethingChanged()),SLOT(refreshDisplay()));
    connect(col,SIGNAL(moveMeUp(int)),SLOT(upCol(int)));
    connect(col,SIGNAL(moveMeDown(int)),SLOT(downCol(int)));
    connect(col,SIGNAL(removeMe(int)),SLOT(delCol(int)));
    connect(this,SIGNAL(switchPos(int,int,int)),col,SLOT(posSwitched(int,int,int)));
    connect(this,SIGNAL(deletePos(int,int)),col,SLOT(posRemoved(int,int)));
    connect(this,SIGNAL(newPos(int)),col,SLOT(posChanged(int)));
    connect(ui->checkBoxAdapt,SIGNAL(toggled(bool)),col->widthBox(),SLOT(setDisabled(bool)));

    col->widthBox()->setDisabled(ui->checkBoxAdapt->isChecked());

#if QT_VERSION >= 0x403000
    col->setContentsMargins(0,0,0,0);
#endif
    col->setSpacing(0);
    col->setMargin(0);

    return col;
}

void EditSheet::upCol(int i) {
    if(i>0) {
        ui->vboxLayout->insertLayout(i-1,((QLayout*)ui->vboxLayout->takeAt(i)));
        emit switchPos(i,i-1,ui->vboxLayout->count());
    }
}

void EditSheet::downCol(int i) {
    if(i<ui->vboxLayout->count()-1) {
        ui->vboxLayout->insertLayout(i+1,((QLayout*)ui->vboxLayout->takeAt(i)));
        emit switchPos(i,i+1,ui->vboxLayout->count());
    }
}

void EditSheet::delCol(int i) {
    ColumnEditSheet* c = (ColumnEditSheet*)ui->vboxLayout->takeAt(i);
    delete c;
    emit deletePos(i,ui->vboxLayout->count());
}

void EditSheet::addColumn() {
    column c;
    c.name = Tr("CompleteName");
    c.width = 50;
    c.stream = Stream_General;
    c.key = "CompleteName";
    ui->vboxLayout->addLayout(createColumn(c));
    emit newPos(ui->vboxLayout->count());
    refreshDisplay();
}

void EditSheet::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void EditSheet::apply() {
    sheet->resetColumns();
    for(int i=0;i<ui->vboxLayout->count();++i) {
        column c;
        c.name = ((QLineEdit*)ui->vboxLayout->itemAt(i)->layout()->itemAt(0)->widget())->text();
        c.width = ((QSpinBox*)ui->vboxLayout->itemAt(i)->layout()->itemAt(1)->widget())->value();
        c.stream = (stream_t)((QComboBox*)ui->vboxLayout->itemAt(i)->layout()->itemAt(2)->widget())->itemData(((QComboBox*)ui->vboxLayout->itemAt(i)->layout()->itemAt(2)->widget())->currentIndex()).toInt();
        c.key = ((QComboBox*)ui->vboxLayout->itemAt(i)->layout()->itemAt(3)->widget())->itemData(((QComboBox*)ui->vboxLayout->itemAt(i)->layout()->itemAt(3)->widget())->currentIndex()).toString();
        sheet->addColumn(c);
    }
    sheet->setName(ui->lineEdit->text().toStdString().c_str());
    sheet->setAdaptColumns(ui->checkBoxAdapt->isChecked());
}
