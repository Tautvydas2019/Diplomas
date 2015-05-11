#include "report.h"
#include "ui_report.h"

#include "settings.h"

#include <QSqlTableModel>
#include <QString>
#include <QDate>
#include <QVariant>
#include <QSqlQuery>
#include <QPrinter>
#include <QTextDocument>
#include <QSqlRecord>
#include <QDesktopServices>
#include <QDir>

Report::Report(QWidget *parent, DatabaseManager *dbm) :
    QDialog(parent),
    ui(new Ui::Report)
{
    this->dbm = dbm;
    ui->setupUi(this);

    model_model = new QSqlTableModel(this, dbm->getDatabase());
    model_model->setTable(Settings::MODEL_TABLE);
    model_model->select();

    ui->comboBox->setModel(model_model);
    ui->comboBox->setModelColumn(1);

    ui->dateEdit->setDisplayFormat("yyyy-MM-dd");
    ui->dateEdit->setDate(QDate::currentDate());

}

Report::~Report()
{
    delete ui;
}

void Report::on_pushButton_clicked()
{
    int row = ui->comboBox->currentIndex();
    QModelIndex index;
    index = model_model->index(row, 0);
    QVariant eka_model_id = model_model->data(index);
    QVariant date = ui->dateEdit->date();

    QSqlQuery query;
    QString sql = "SELECT COUNT(*) AS total FROM `" + Settings::EKA_TABLE + "` AS e "
                  "LEFT JOIN `" + Settings::CLIENT_TABLE + "` AS c ON e.client_id = c.client_id "
                  "WHERE e.model_id = ? AND DATE(e.main_checkup) <= DATE(?) AND e.status = ?;";

    query.prepare(sql);
    query.bindValue(0, eka_model_id.toString());
    query.bindValue(1, date.toString());
    query.bindValue(2, "Dirba");

    QString total;
    if (!query.exec())
    {
        dbm->dbError(query.lastError());
        total = "0";
    }
    else
    {
        query.next();
        total = query.value("total").toString();
    }
    query.clear();

    QString html = "";

    sql = "SELECT e.serial_number, c.name, e.place_eka, e.main_checkup  FROM `" + Settings::EKA_TABLE + "` AS e "
                  "LEFT JOIN `" + Settings::CLIENT_TABLE + "` AS c ON e.client_id = c.client_id "
                  "WHERE e.model_id = ? AND DATE(e.main_checkup) <= DATE(?) AND e.status = ?;";


    query.prepare(sql);
    query.bindValue(0, eka_model_id.toString());
    query.bindValue(1, date.toString());
    query.bindValue(2, "Dirba");

    if (!query.exec())
    {
        dbm->dbError(query.lastError());
    }

    index = model_model->index(row, 1);
    QVariant eka_model_name = model_model->data(index);
    html += "<h1 align='center'>Sutarčių ataskaita</h1>";
    html += "<h2 align='center'>" + eka_model_name.toString() + "</h2>";
    html += "<table width='100%'>";
    html += "<tr>";
    html += "<th>Iš viso: " + total + "</th>";
    html += "<th></th>";
    html += "<th>" + QDate::currentDate().toString("yyyy-MM-dd") + "</th>";
    html += "</tr>";
    html += "</table>";

    html += "<table border='1' width='100%'>";
    html += "<thead>";
    html += "<tr>";
    html += "<th>Nr.</th>";
    html += "<th>EKA nr.</th>";
    html += "<th>Klientas</th>";
    html += "<th>Adresas</th>";
    html += "<th>Sut. iki</th>";
    html += "</tr>";
    html += "</thead>";

    html += "<tbody>";
    int row_number = 1;
    while (query.next())
    {
        html += "<tr>";
        html += "<td>" + QString::number(row_number) + "</td>";
        html += "<td>" + query.value("serial_number").toString() + "</td>";
        html += "<td>" + query.value("name").toString() + "</td>";
        html += "<td>" + query.value("place_eka").toString() + "</td>";
        html += "<td>" + query.value("main_checkup").toString() + "</td>";
        html += "</tr>";
        row_number++;
    }
    html += "</tbody>";
    html += "</table>";

    QTextDocument doc;
    doc.setHtml(html);

    QPrinter printer;
    QString directory = QDir::currentPath() + "/ataskaitos/";
    if (!QDir(directory).exists())
    {
        QDir().mkdir(directory);
    }
    QString file_name = "sutartys_" + QString::number(QDateTime::currentDateTime().toTime_t()) + ".pdf";
    printer.setOutputFileName(directory + file_name);
    printer.setOutputFormat(QPrinter::PdfFormat);
    doc.print(&printer);
    printer.newPage();
    doc.clear();
    QDesktopServices::openUrl(QUrl(directory + file_name));
}

void Report::on_pushButton_2_clicked()
{
    QSqlQuery query;
    QString sql = "SELECT COUNT(*) AS total FROM `" + Settings::EKA_TABLE + "`;";
    query.prepare(sql);

    QString total;
    if (!query.exec())
    {
        dbm->dbError(query.lastError());
        total = "0";
    }
    else
    {
        query.next();
        total = query.value("total").toString();
    }
    query.clear();

    QString html = "";

    sql = "SELECT * FROM `" + Settings::CLIENT_TABLE + "`;";
    query.prepare(sql);

    if (!query.exec())
    {
        dbm->dbError(query.lastError());
    }

    html += "<h1 align='center'>Klientų ataskaita</h1>";
    html += "<table width='100%'>";
    html += "<tr>";
    html += "<th>Iš viso: " + total + "</th>";
    html += "<th></th>";
    html += "<th>" + QDate::currentDate().toString("yyyy-MM-dd") + "</th>";
    html += "</tr>";
    html += "</table>";

    html += "<table border='1' width='100%'>";
    html += "<thead>";
    html += "<tr>";
    html += "<th>Nr.</th>";
    html += "<th>Klientas</th>";
    html += "<th>Kodas</th>";
    html += "<th>PVM Kodas</th>";
    html += "<th>Adresas</th>";
    html += "<th>Miestas</th>";
    html += "<th>Telefonas</th>";
    html += "</tr>";
    html += "</thead>";

    html += "<tbody>";
    int row_number = 1;
    while (query.next())
    {
        html += "<tr>";
        html += "<td>" + QString::number(row_number) + "</td>";
        html += "<td>" + query.value("name").toString() + "</td>";
        html += "<td>" + query.value("code").toString() + "</td>";
        html += "<td>" + query.value("vat").toString() + "</td>";
        html += "<td>" + query.value("address").toString() + "</td>";
        html += "<td>" + query.value("city").toString() + "</td>";
        html += "<td>" + query.value("telephone").toString() + "</td>";
        html += "</tr>";
        row_number++;
    }
    html += "</tbody>";
    html += "</table>";

    QTextDocument doc;
    doc.setHtml(html);

    QPrinter printer;
    QString directory = QDir::currentPath() + "/ataskaitos/";
    if (!QDir(directory).exists())
    {
        QDir().mkdir(directory);
    }
    QString file_name = "klientai_" + QString::number(QDateTime::currentDateTime().toTime_t()) + ".pdf";
    printer.setOutputFileName(directory + file_name);
    printer.setOutputFormat(QPrinter::PdfFormat);
    doc.print(&printer);
    printer.newPage();
    doc.clear();
    QDesktopServices::openUrl(QUrl(directory + file_name));
}

void Report::on_pushButton_3_clicked()
{
    int row = ui->comboBox->currentIndex();
    QModelIndex index;
    index = model_model->index(row, 0);
    QVariant eka_model_id = model_model->data(index);
    QVariant date = ui->dateEdit->date();

    QSqlQuery query;
    QString sql = "SELECT COUNT(*) AS total FROM `" + Settings::EKA_TABLE + "` AS e "
                  "LEFT JOIN `" + Settings::CLIENT_TABLE + "` AS c ON e.client_id = c.client_id "
                  "WHERE e.model_id = ? AND (NOT (e.reg_rent IS NULL OR TRIM(e.reg_rent) = '') AND DATE(e.reg_rent) <= DATE(?)) AND e.status = ?;";

    query.prepare(sql);
    query.bindValue(0, eka_model_id.toString());
    query.bindValue(1, date.toString());
    query.bindValue(2, "Dirba");

    QString total;
    if (!query.exec())
    {
        dbm->dbError(query.lastError());
        total = "0";
    }
    else
    {
        query.next();
        total = query.value("total").toString();
    }
    query.clear();

    QString html = "";

    sql = "SELECT e.serial_number, c.name, e.place_eka, reg_rent  FROM `" + Settings::EKA_TABLE + "` AS e "
                  "LEFT JOIN `" + Settings::CLIENT_TABLE + "` AS c ON e.client_id = c.client_id "
                  "WHERE e.model_id = ? AND (NOT (e.reg_rent IS NULL OR TRIM(e.reg_rent) = '') AND DATE(e.reg_rent) <= DATE(?)) AND e.status = ?;";


    query.prepare(sql);
    query.bindValue(0, eka_model_id.toString());
    query.bindValue(1, date.toString());
    query.bindValue(2, "Dirba");

    if (!query.exec())
    {
        dbm->dbError(query.lastError());
    }
    index = model_model->index(row, 1);
    QVariant eka_model_name = model_model->data(index);
    html += "<h1 align='center'>Nuomų ataskaita</h1>";
    html += "<h2 align='center'>" + eka_model_name.toString() + "</h2>";
    html += "<table width='100%'>";
    html += "<tr>";
    html += "<th>Iš viso: " + total + "</th>";
    html += "<th></th>";
    html += "<th>" + QDate::currentDate().toString("yyyy-MM-dd") + "</th>";
    html += "</tr>";
    html += "</table>";

    html += "<table border='1' width='100%'>";
    html += "<thead>";
    html += "<tr>";
    html += "<th>Nr.</th>";
    html += "<th>EKA nr.</th>";
    html += "<th>Klientas</th>";
    html += "<th>Adresas</th>";
    html += "<th>Nuoma nuo</th>";
    html += "</tr>";
    html += "</thead>";

    html += "<tbody>";
    int row_number = 1;
    while (query.next())
    {
        html += "<tr>";
        html += "<td>" + QString::number(row_number) + "</td>";
        html += "<td>" + query.value("serial_number").toString() + "</td>";
        html += "<td>" + query.value("name").toString() + "</td>";
        html += "<td>" + query.value("place_eka").toString() + "</td>";
        html += "<td>" + query.value("reg_rent").toString() + "</td>";
        html += "</tr>";
        row_number++;
    }
    html += "</tbody>";
    html += "</table>";

    QTextDocument doc;
    doc.setHtml(html);

    QPrinter printer;
    QString directory = QDir::currentPath() + "/ataskaitos/";
    if (!QDir(directory).exists())
    {
        QDir().mkdir(directory);
    }
    QString file_name = "nuomos_" + QString::number(QDateTime::currentDateTime().toTime_t()) + ".pdf";
    printer.setOutputFileName(directory + file_name);
    printer.setOutputFormat(QPrinter::PdfFormat);
    doc.print(&printer);
    printer.newPage();
    doc.clear();
    QDesktopServices::openUrl(QUrl(directory + file_name));
}

void Report::on_pushButton_4_clicked()
{
    int row = ui->comboBox->currentIndex();
    QModelIndex index;
    index = model_model->index(row, 0);
    QVariant eka_model_id = model_model->data(index);
    QVariant date = ui->dateEdit->date();

    QSqlQuery query;
    QString sql = "SELECT COUNT(*) AS total FROM `" + Settings::EKA_TABLE + "` AS e "
                  "LEFT JOIN `" + Settings::CLIENT_TABLE + "` AS c ON e.client_id = c.client_id "
                  "WHERE e.model_id = ? AND e.status = ?;";

    query.prepare(sql);
    query.bindValue(0, eka_model_id.toString());
    query.bindValue(1, "Nedirba");

    QString total;
    if (!query.exec())
    {
        dbm->dbError(query.lastError());
        total = "0";
    }
    else
    {
        query.next();
        total = query.value("total").toString();
    }
    query.clear();

    QString html = "";

    sql = "SELECT e.serial_number, c.name, e.place_eka, e.main_checkup  FROM `" + Settings::EKA_TABLE + "` AS e "
                  "LEFT JOIN `" + Settings::CLIENT_TABLE + "` AS c ON e.client_id = c.client_id "
                  "WHERE e.model_id = ?  AND e.status = ?;";


    query.prepare(sql);
    query.bindValue(0, eka_model_id.toString());
    query.bindValue(1, "Nedirba");

    if (!query.exec())
    {
        dbm->dbError(query.lastError());
    }
    index = model_model->index(row, 1);
    QVariant eka_model_name = model_model->data(index);
    html += "<h1 align='center'>Sustabdytos veiklos ataskaita</h1>";
    html += "<h2 align='center'>" + eka_model_name.toString() + "</h2>";
    html += "<table width='100%'>";
    html += "<tr>";
    html += "<th>Iš viso: " + total + "</th>";
    html += "<th></th>";
    html += "<th>" + QDate::currentDate().toString("yyyy-MM-dd") + "</th>";
    html += "</tr>";
    html += "</table>";

    html += "<table border='1' width='100%'>";
    html += "<thead>";
    html += "<tr>";
    html += "<th>Nr.</th>";
    html += "<th>EKA nr.</th>";
    html += "<th>Klientas</th>";
    html += "<th>Adresas</th>";
    html += "<th>Sut. iki</th>";
    html += "</tr>";
    html += "</thead>";

    html += "<tbody>";
    int row_number = 1;
    while (query.next())
    {
        html += "<tr>";
        html += "<td>" + QString::number(row_number) + "</td>";
        html += "<td>" + query.value("serial_number").toString() + "</td>";
        html += "<td>" + query.value("name").toString() + "</td>";
        html += "<td>" + query.value("place_eka").toString() + "</td>";
        html += "<td>" + query.value("main_checkup").toString() + "</td>";
        html += "</tr>";
        row_number++;
    }
    html += "</tbody>";
    html += "</table>";

    QTextDocument doc;
    doc.setHtml(html);

    QPrinter printer;
    QString directory = QDir::currentPath() + "/ataskaitos/";
    if (!QDir(directory).exists())
    {
        QDir().mkdir(directory);
    }
    QString file_name = "sustabdyta_veikla_" + QString::number(QDateTime::currentDateTime().toTime_t()) + ".pdf";
    printer.setOutputFileName(directory + file_name);
    printer.setOutputFormat(QPrinter::PdfFormat);
    doc.print(&printer);
    printer.newPage();
    doc.clear();
    QDesktopServices::openUrl(QUrl(directory + file_name));
}
