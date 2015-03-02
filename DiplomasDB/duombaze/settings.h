#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QList>

class Settings
{
public:

   static const QString TEXT_MAINWINDOW_NAME;
   static const QString TEXT_NAUJASKLIENTAS_NAME;
   static const QString TEXT_NAUJASMODELIS_NAME;
   static const QList<QString> DB_TABLES;
   static const QString CLIENT_TABLE;
   static const QString MODEL_TABLE;
   static const QString DB_TYPE;
   static const QString DB_LOCATION;

};
#endif // SETTINGS_H
