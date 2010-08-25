#include "export.h"
#include "translate.h"
#include "_Automated/ui_export.h"
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include "configtreetext.h"

Export::Export(QString filename, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Export)
{
    ui->setupUi(this);
    ui->lineEdit->setText(filename);
    for(int i=0;i<Export::NB_EXPORT_MODE;i++) {
        ui->comboBoxMode->addItem(name(i),i);
    }
    ConfigTreeText::fillComboBox(ui->comboBoxConfig);
}

Export::~Export()
{
    delete ui;
}

void Export::changeEvent(QEvent *e)
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

void Export::on_toolButton_pressed()
{
    ui->lineEdit->setText(QFileDialog::getOpenFileName(this,"",QDir::home().absolutePath(),Tr("All files (*.*)")));
}

int Export::getExportMode() {
    return ui->comboBoxMode->itemData(ui->comboBoxMode->currentIndex()).toInt();
}

int Export::getExportConfig() {
    if(ui->comboBoxConfig->isEnabled())
        return ui->comboBoxConfig->itemData(ui->comboBoxConfig->currentIndex()).toInt();
    else
        return 0;
}

QString Export::getFileName() {
    return ui->lineEdit->text();
}

QIODevice::OpenMode Export::getOpenMode() {
    if(ui->appendToFile->isChecked())
        return QIODevice::Append;
    else
        return QIODevice::WriteOnly;
}

bool Export::isAdvancedChecked() {
    return ui->checkBoxAdvanced->isChecked();
}

QString Export::extension(int mode) {
    switch(mode) {
    default:
    case TEXT: return "txt";
        break;
    case HTML: return "html";
        break;
    case XML: return "xml";
        break;
    case PBCORE: return "xml";
        break;
    case MPEG7: return "xml";
        break;
    case CSV: return "csv";
        break;
    }
}

QString Export::name(int mode) {
    switch(mode) {
    default:
    case TEXT: return Tr("Text");
        break;
    case HTML: return Tr("HTML");
        break;
    case XML: return Tr("XML");
        break;
    case PBCORE: return Tr("PBCore");
        break;
    case MPEG7: return Tr("MPEG-7");
        break;
    case CSV: return Tr("CSV");
        break;
    }
}

void Export::on_comboBoxMode_currentIndexChanged(int index)
{
    QStringList filename = ui->lineEdit->text().split(".");
    filename[filename.size()-1] = extension(index);
    ui->lineEdit->setText(filename.join("."));

    switch(index) {
    default:
    case TEXT: ui->comboBoxConfig->setDisabled(ui->checkBoxAdvanced->isChecked());
        break;
    case HTML: ui->comboBoxConfig->setEnabled(false);
        break;
    case XML: ui->comboBoxConfig->setEnabled(false);
        break;
    case PBCORE: ui->comboBoxConfig->setEnabled(false);
        break;
    case MPEG7: ui->comboBoxConfig->setEnabled(false);
        break;
    case CSV: ui->comboBoxConfig->setEnabled(false);
        break;
    }
}

void Export::on_checkBoxAdvanced_toggled(bool checked)
{
    ui->comboBoxConfig->setEnabled( (!checked) && (ui->comboBoxMode->itemData(ui->comboBoxMode->currentIndex()).toInt()==TEXT) );
}
