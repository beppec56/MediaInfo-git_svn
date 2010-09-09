#include "preferences.h"
#include "translate.h"
#include "_Automated/ui_preferences.h"
#include <QtGui/QLabel>
#include "views.h"
#include "sheet.h"
#include "configtreetext.h"
#include "editsheet.h"
#include "editconfigtreetext.h"

Preferences::Preferences(QSettings* settings, Core* C, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferences)
{
    this->settings=settings;
    this->C = C;

    ui->setupUi(this);
    ui->treeWidget->setColumnHidden(1,true);
    ui->treeWidget->expandAll();

    for(int v=VIEW_EASY;v<NB_VIEW;v++) {
        ui->comboBox_defaultview->addItem(nameView((ViewMode)v),v);
    }

    ui->showMenu->setChecked(settings->value("showMenu",true).toBool());
    ui->showToolbar->setChecked(settings->value("showToolbar",true).toBool());
    ui->closeAllBeforeOpen->setChecked(settings->value("closeBeforeOpen",true).toBool());
    ui->comboBox_defaultview->setCurrentIndex(settings->value("defaultView",VIEW_EASY).toInt());
    ui->checkForNewVersion->setChecked(settings->value("checkForNewVersion",true).toBool());
    ui->rememberToolBarPosition->setChecked(settings->value("rememberToolBarPosition",true).toBool());
    ui->rememberGeometry->setChecked(settings->value("rememberGeometry",false).toBool());

    ui->showToolbar->setEnabled(ui->showMenu->isChecked());
    ui->showMenu->setEnabled(ui->showToolbar->isChecked());
    ui->rememberToolBarPosition->setEnabled(ui->showToolbar->isChecked());

    refreshDisplay();

    ui->stackedWidget->setCurrentIndex(0);
}

Preferences::~Preferences()
{
    delete ui;
}

void Preferences::refreshDisplay() {
    // Text&Tree
    ConfigTreeText::fillComboBox(ui->treeTextComboBox);
    ui->pushButton_editTreeText->setEnabled(ui->treeTextComboBox->itemData(ui->treeTextComboBox->currentIndex()).toInt()>0);
    ui->pushButton_deleteTreeText->setEnabled( (ConfigTreeText::getNbConfigTreeTexts()>1) && (ui->treeTextComboBox->itemData(ui->treeTextComboBox->currentIndex()).toInt()>0) );

    // Sheets
    ui->comboBoxSheet->clear();
    for(int i=0;i<Sheet::getNbSheets();i++) {
        ui->comboBoxSheet->addItem(Sheet::get(i)->getName(),i);
    }
    ui->comboBoxSheet->setCurrentIndex(Sheet::getIndex());
    ui->pushButton_deleteSheet->setEnabled(Sheet::getNbSheets()>1);
}

void Preferences::saveSettings() {
    settings->setValue("showMenu",ui->showMenu->isChecked());
    settings->setValue("showToolbar",ui->showToolbar->isChecked());
    settings->setValue("closeBeforeOpen",ui->closeAllBeforeOpen->isChecked());
    settings->setValue("checkForNewVersion",ui->checkForNewVersion->isChecked());
    settings->setValue("defaultView",ui->comboBox_defaultview->currentIndex());
    settings->setValue("rememberToolBarPosition",ui->rememberToolBarPosition->isChecked());
    settings->setValue("rememberGeometry",ui->rememberGeometry->isChecked());
    Sheet::setDefault(ui->comboBoxSheet->itemData(ui->comboBoxSheet->currentIndex()).toInt());
    Sheet::save(settings);
    ConfigTreeText::setDefault(ui->treeTextComboBox->itemData(ui->treeTextComboBox->currentIndex()).toInt());
    ConfigTreeText::save(settings);
}

void Preferences::changeEvent(QEvent *e)
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

void Preferences::on_treeWidget_itemSelectionChanged()
{
    if(!ui->treeWidget->selectedItems().first()->data(1,Qt::DisplayRole).isNull())
        ui->stackedWidget->setCurrentIndex(ui->treeWidget->selectedItems().first()->data(1,Qt::DisplayRole).toInt());
}

void Preferences::on_pushButton_editSheet_clicked()
{
    EditSheet es(Sheet::get(ui->comboBoxSheet->itemData(ui->comboBoxSheet->currentIndex()).toInt()), C, this);
    if(es.exec() == QDialog::Accepted) {
        es.apply();
        refreshDisplay();
    } else
        qDebug("sheet editing cancelled");
}

void Preferences::on_pushButton_newSheet_clicked()
{
    Sheet* s = Sheet::add("newsheet");
    s->addColumn(Tr("File Name").toStdString().c_str(),300,Stream_General,"CompleteName");
    EditSheet es(s, C, this);
    if(es.exec() == QDialog::Accepted) {
        es.apply();
        refreshDisplay();
    } else {
        Sheet::removeLast();
        qDebug("new sheet cancelled");
    }
}

void Preferences::on_pushButton_deleteSheet_clicked()
{
    Sheet::remove(ui->comboBoxSheet->itemData(ui->comboBoxSheet->currentIndex()).toInt());
    refreshDisplay();
}

void Preferences::on_pushButton_editTreeText_clicked()
{
    EditConfigTreeText ectt(ConfigTreeText::get(ui->treeTextComboBox->itemData(ui->treeTextComboBox->currentIndex()).toInt()), C, this);
    if(ectt.exec() == QDialog::Accepted) {
        ectt.apply();
        refreshDisplay();
    } else
        qDebug("config editing cancelled");
}

void Preferences::on_pushButton_newTreeText_clicked()
{
    ConfigTreeText* ctt = ConfigTreeText::add("newconfig");
    EditConfigTreeText ectt(ctt, C, this);
    if(ectt.exec() == QDialog::Accepted) {
        ectt.apply();
        refreshDisplay();
    } else {
        ConfigTreeText::removeLast();
        qDebug("new config cancelled");
    }
}

void Preferences::on_pushButton_deleteTreeText_clicked()
{
    ConfigTreeText::remove(ui->treeTextComboBox->itemData(ui->treeTextComboBox->currentIndex()).toInt());
    refreshDisplay();
}

void Preferences::on_treeTextComboBox_currentIndexChanged(int index)
{
    ui->pushButton_editTreeText->setEnabled(index>0);
    ui->pushButton_deleteTreeText->setEnabled( (ConfigTreeText::getNbConfigTreeTexts()>1) && (index>0) );
}
