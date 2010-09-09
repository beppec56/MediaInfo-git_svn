#ifndef CONFIGTREETEXT_H
#define CONFIGTREETEXT_H

#include <QtCore/QVector>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QSettings>
#include <QtGui/QComboBox>
#include <QtCore/QDebug>

class ConfigTreeText
{
public:
    static ConfigTreeText* getConfigTreeText();
    static ConfigTreeText* get(int i);
    static int getNbConfigTreeTexts();
    static int getIndex();
    static void setDefault(int i);
    static ConfigTreeText* add(QString name);
    static void load(QSettings* settings);
    static void save(QSettings* settings);
    static void remove(int i);
    static void removeLast();

    static void fillComboBox(QComboBox* qcb);

    QString getName();
    void setName(QString n);
    QList<QStringList> getFields();
    QStringList getFields(int i);
    void setFields(int i, QStringList fs);
    void addField(int i, QString f);
    void removeField(int i, QString f);

private:
    ConfigTreeText(QString name);
    QString name;
    QList<QStringList> fields;

    static QVector<ConfigTreeText*> configs;
    static QStringList names;
    static int indexDefault;
};

#endif // CONFIGTREETEXT_H
