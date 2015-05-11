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
   static const QString TEXT_NAUJASEKA_NAME;
   static const QString TEXT_KLIENTUPAIESKA_NAME;
   static const QString TEXT_EKAEDITING_NAME;
   static const QString TEXT_REPORT_NAME;
   static const QList<QString> DB_TABLES;
   static const QList<QString> TEMP_DB_TABLES;
   static const QString CLIENT_TABLE;
   static const QString MODEL_TABLE;
   static const QString EKA_TABLE;
   static const QString DB_TYPE;
   static const QString DB_LOCATION;
   static const QString TEMP_EKA_TABLE;
   static const QString TEMP_CLIENT_TABLE;
   static const QString TEMP_MODEL_TABLE;

};
#endif // SETTINGS_H
