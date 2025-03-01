#include "logindialog.h"
#include "ui_logindialog.h"
#include "mainwindow.h"


LoginDialog::LoginDialog(QWidget *parent,MainWindow* mainWindow)
    : QDialog(parent)
    , ui(new Ui::LoginDialog),
        mainWindow(mainWindow) //uloženie mainwindow do premennej
{
    ui->setupUi(this);
    //setUsers(const QVector<PhysicalPerson*>& physicalPersons, const QVector<Company*>& companies, const QVector<Worker*>& workers);
    naplnComboBox(); // Naplníme ComboBox používateľmi
    mainWindow->loadUsersFromFile(":/resources/resources/users.txt");
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::setUsers(const QVector<PhysicalPerson*>& physicalPersons,
                           const QVector<Company*>& companies,
                           const QVector<Worker*>& workers)
{
    this->physicalPersons = physicalPersons;
    this->companies = companies;
    this->workers = workers;
    qDebug() << "Používatelia nastavení v LoginDialog:";
    qDebug() << "Fyzické osoby:" << physicalPersons.size();
    qDebug() << "Firmy:" << companies.size();
    qDebug() << "Pracovníci:" << workers.size();
}

void LoginDialog::on_pushButtonLogin_clicked()
{
    QString username = ui->comboBoxLogin->currentText();
    QString password = ui->lineEditLogin->text();
    qDebug() << "Zadané meno: " << username;
    qDebug() << "Zadané heslo: " << password;
    qDebug() << "Počet fyzických osôb:" << this->physicalPersons.size();
    qDebug() << "Počet firiem:" << this->companies.size();
    qDebug() << "Počet pracovníkov:" << this->workers.size();

    for (PhysicalPerson* person : physicalPersons) {
        qDebug() << "Fyzická osoba: " << person->getUsername() << " Heslo: " << person->getPassword();
    }

    for (Company* company : companies) {
        qDebug() << "Firma: " << company->getUsername() << " Heslo: " << company->getPassword();
    }

    for (Worker* worker : workers) {
        qDebug() << "Pracovník: " << worker->getUsername() << " Heslo: " << worker->getPassword();
    }



    for (PhysicalPerson* person : physicalPersons) {
        if (person->getUsername() == username && person->getPassword() == password) {
            loggedInUser = person;
            mainWindow->setCurrentUser(loggedInUser);  // Nastaví aktuálneho používateľa v MainWindow
            accept(); //zavrie LoginDialog s úspešným prhlásením
            return;
        }
    }

    for (Company* company : companies) {
        if (company->getUsername() == username && company->getPassword() == password) {
            loggedInUser = company;
            mainWindow->setCurrentUser(loggedInUser);  // Nastaví aktuálneho používateľa v MainWindow
            accept();
            return;
        }
    }

    for (Worker* worker : workers) {
        if (worker->getUsername() == username && worker->getPassword() == password) {
            loggedInUser = worker;
            mainWindow->setCurrentUser(loggedInUser);  // Nastaví aktuálneho používateľa v MainWindow
            accept();
            return;
        }
    }


    // Ak používateľ nie je nájdený
    ui->labelLogin->setText("Chyba: Nesprávne meno alebo heslo");
    ui->labelLogin->setStyleSheet("color: red");


}


    /*
    // kontrola spravnosti hesla
    if (loggedIn(username, password)) {
        ui->labelLogin->setText("Prihlásenie úspešné!");
        ui->labelLogin->setStyleSheet("color: green");



        //prihlaseny, zapametanie


        // Uzavrite LoginDialog a oznámte, že bolo prihlásenie úspešné
        accept();  // Toto uzavrie dialóg a vráti QDialog::Accepted
    } else {
        ui->labelLogin->setText("Chyba: Nesprávne heslo");
        ui->labelLogin->setStyleSheet("color: red");

        // Ak prihlásenie zlyhá, zavrieme dialóg s QDialog::Rejected
        reject();  // Vráti QDialog::Rejected
    }
}

*/

void LoginDialog::naplnComboBox()
{
    ui->comboBoxLogin->clear();
    //pridanie
    for (PhysicalPerson* person : mainWindow->getPhysicalPersons()) {
        ui->comboBoxLogin->addItem(person->getUsername());
    }

    // Pridáme firmy
    for (Company* company : mainWindow->getCompanies()) {
        ui->comboBoxLogin->addItem(company->getUsername());
    }

    // Pridáme pracovníkov
    for (Worker* worker : mainWindow->getWorkers()) {
        ui->comboBoxLogin->addItem(worker->getUsername());
    }
}


/*
bool LoginDialog::loggedIn(const QString &username, const QString &password)
{
    for (PhysicalPerson* person : physicalPersons) {
        if (person->getUsername() == username && person->getPassword() == password) {
            return true;
        }
    }

    for (Company* company : companies) {
        if (company->getUsername() == username && company->getPassword() == password) {
            return true;
        }
    }

    for (Worker* worker : workers) {
        if (worker->getUsername() == username && worker->getPassword() == password) {
            return true;
        }
    }

    return false;  // Ak sme nenašli používateľa so správnymi údajmi

}
*/
