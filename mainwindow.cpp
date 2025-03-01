#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QTextStream> //
#include <QTabBar>
#include "logindialog.h"
#include <QFileInfo>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentUser(nullptr)  // Inicializácia currentUser, teraz nullptr
{
    ui->setupUi(this);

    loadUsersFromFile(":/resources/resources/users.txt");  // Načítam používateľov pri spustení

    qDebug() << "Inicializácia LoginDialog...";
    LoginDialog loginDialog(this, this);
    loginDialog.setUsers(physicalPersons, companies, workers);

    if (loginDialog.exec() == QDialog::Accepted) { //prejde loggin, niekto spravne zadal heslo
        User* loggedInUser = loginDialog.getLoggedInUser(); //nastavenie prihláseného
        //s ním pracujem potom ďalej v main window
        if (loggedInUser) {
            setCurrentUser(loggedInUser);  // Nastavenie aktuálneho používateľa
            qDebug() << "Používateľ prihlásený: " << currentUser->getUsername();

            loadServicesFromFile(":/resources/resources/orders.txt");  // Načítajte služby pre používateľa (všetky)
            //načítanie objednávok z osobitého súboru pre currentUsera
            loadOrdersFromFile(currentUser->getUsername());

            this->show();  // Zobrazí hlavné okno po úspešnom prihlásení
        }

    } else {
        qDebug() << "LoginDialog: Prihlásenie zrušené.";
        close();  // Ak sa používateľ neprihlási, zavrite aplikáciu
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


//zistenie, ktorý user je prihlásený--------------------
// podľa toho sa sprístupna taby
void MainWindow::VisibleTab() {
    if (!currentUser) {
        qDebug() << "Žiaden user nie je prihlásený, z funkcie VisibleTab.";
        return;
    }

    if (currentUser->getUserType() == "physical" || currentUser->getUserType() == "company") {
        // Iba prvý tab
        ui->tabWidget->setTabEnabled(0, true); // Enable Tab 1
        ui->tabWidget->setTabEnabled(1, false); // Disable Tab 2 (or others if more tabs exist)
    } else if (currentUser->getUserType() == "worker") {
        // Iba druhý tab
        ui->tabWidget->setTabEnabled(0, false); // Disable Tab 1
        ui->tabWidget->setTabEnabled(1, true);  // Enable Tab 2

        //napln ListWidget
        fillListWidgetUsers();
    }
}




//zistenie, ktorý user je prihlásený----------------------------------





void MainWindow::loadUsersFromFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Chyba pri otváraní súboru.";
        return;
    }
    qDebug() << "Načítavam služby z: " << fileName;
    qDebug() << "pouz: " << currentUser;

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString username = in.readLine().trimmed();   // 1. riadok - meno
        QString password = in.readLine().trimmed();   // 2. riadok - heslo
        QString type = in.readLine().trimmed();       // 3. riadok - typ používateľa (physical, company, worker)

        if (type == "physical") {
            QString fullName = in.readLine().trimmed(); // 4. riadok - celé meno
            QString carBrand = in.readLine().trimmed(); // 5. riadok - značka auta
            QString carModel = in.readLine().trimmed(); // 6. riadok - model auta
            int carYear = in.readLine().toInt();        // 7. riadok - rok výroby auta
            physicalPersons.append(new PhysicalPerson(username, password, fullName, carBrand, carModel, carYear));
        }
        else if (type == "company") {
            QString companyName = in.readLine().trimmed(); // 4. riadok - názov firmy
            QString carBrand = in.readLine().trimmed();    // 5. riadok - značka auta
            QString carModel = in.readLine().trimmed();    // 6. riadok - model auta
            int carYear = in.readLine().toInt();           // 7. riadok - rok výroby auta
            int ico = in.readLine().toInt();               // 8. riadok - IČO firmy
            bool platcaDPH = in.readLine().trimmed() == "true";  // 9. riadok - platca DPH, ak tam bude string "true" nastaví sa bool ture, ak cokolvek ine -> false
            companies.append(new Company(username, password, companyName, carBrand, carModel, carYear, ico, platcaDPH));
        }
        else if (type == "worker") {
            QString position = in.readLine().trimmed(); // 4. riadok - pozícia (technician, administrator)
            workers.append(new Worker(username, password, position));
        }
        //preskoci prazdny riadok po kazdom userovi
        in.readLine();
    }

    file.close();


}



void MainWindow::loadOrdersFromFile(const QString& username)
{
        QString filename = QString("orders_%1.txt").arg(username);  // Súbor s objednávkami pre daného používateľa
        QFile file(filename);
        QFileInfo fileInfo(filename);



        // Skontrolujeme, či súbor neexistuje
        if (!QFileInfo::exists(filename)) {
            qDebug() << "Súbor neexistuje. Bude vytvorený:" << filename;
        }

      else {
            qDebug() << "Súbor sa nachádza na ceste:" << fileInfo.absoluteFilePath();
            }


        //append, iba pridanie na koniec
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
            qDebug() << "Chyba pri otváraní súboru na zápis:" << filename;
            return;
        }


        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty()) {
                QStringList orderData = line.split(",");
                if (orderData.size() == 3) {
                    QString serviceName = orderData[0];
                    double servicePrice = orderData[1].toDouble();
                    QString orderStatus = orderData[2];

                    // Vytvoríme objednávku
                    Order order(serviceName, servicePrice, orderStatus);

                    // Pridáme objednávku do aktuálneho používateľa
                    if (currentUser) {
                        currentUser->addOrder(order);
                    }

                    // Naplniť QTableWidget s objednávkami
                    int row = ui->tableWidgetOrders->rowCount();
                    ui->tableWidgetOrders->insertRow(row);
                    ui->tableWidgetOrders->setItem(row, 0, new QTableWidgetItem(serviceName));
                    ui->tableWidgetOrders->setItem(row, 1, new QTableWidgetItem(QString::number(servicePrice, 'f', 2) + " €"));
                    ui->tableWidgetOrders->setItem(row, 2, new QTableWidgetItem(orderStatus));
                }
            }
        }
        file.close();
        qDebug() << "Objednávky boli načítané pre používateľa: " << username;
    }


/*
void MainWindow::saveUsersToFile(const QString& fileName)
{
    QFile file(fileName);

    // Otvorenie súboru na zápis
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Chyba: Nepodarilo sa otvoriť súbor na zápis!";
        qDebug() << "Cesta k súboru:" << fileName;
        qDebug() << "Chyba súboru:" << file.errorString();
        return;
    }

    QTextStream out(&file);

    // Uloženie fyzických osôb
    for (const PhysicalPerson* person : physicalPersons) {
        out << person->getUsername() << "\n";  // meno
        out << person->getPassword() << "\n";  // heslo
        out << "physical\n";  // typ používateľa
        out << person->getFullName() << "\n";  // celé meno
        out << person->getCarBrand() << "\n";  // značka auta
        out << person->getCarModel() << "\n";  // model auta
        out << person->getCarYear() << "\n";  // rok výroby
        out << "\n";  // prázdny riadok medzi používateľmi

    }

    // Uloženie firiem
    for (const Company* company : companies) {
        out << company->getUsername() << "\n";  // meno
        out << company->getPassword() << "\n";  // heslo
        out << "company\n";  // typ používateľa
        out << company->getCompanyName() << "\n";  // názov firmy
        out << company->getCarBrand() << "\n";  // značka auta
        out << company->getCarModel() << "\n";  // model auta
        out << company->getCarYear() << "\n";  // rok výroby
        out << company->getIco() << "\n";  // IČO
        out << (company->isPlatcaDPH() ? "true" : "false") << "\n";  // platca DPH
        out << "\n";  // prázdny riadok medzi používateľmi
    }

    // Uloženie zamestnancov
    for (const Worker* worker : workers) {
        out << worker->getUsername() << "\n";  // meno
        out << worker->getPassword() << "\n";  // heslo
        out << "worker\n";  // typ používateľa
        out << worker->getPosition() << "\n";  // pozícia
        out << "\n";  // prázdny riadok medzi používateľmi
    }

    file.close();
    qDebug() << "Používatelia boli úspešne uložený do súboru.";
}

/*


/*
void MainWindow::saveOrdersToFile(const QList<Order>& orders, const QString& filename)
{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);

        for (const Order& order : orders)
        {
            // Zapíšeme názov služby a cenu
            out << order.getService() << ";" << order.getPrice() << "\n";
        }

        file.close();
    }
    else
    {
        qDebug() << "Nepodarilo sa otvoriť súbor na zápis!";
    }

}

*/
void MainWindow::loadServicesFromFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Chyba pri otváraní súboru!";
        return;
    }

    if (!currentUser) {
        qDebug() << "User is not initialized!";  // Toto zobrazuje, že currentUser nie je inicializovaný
        file.close();
        return;
    } else {
        qDebug() << "Aktuálny používateľ: " << currentUser->getUsername();  // Zobrazí meno prihláseného používateľa
    }


    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            QStringList serviceData = line.split(";");  // Rozdelíme podľa bodkočiarky
            if (serviceData.size() == 2) {
                QString serviceName = serviceData[0];  // Názov služby
                qDebug() << serviceName;
                double servicePrice = serviceData[1].toDouble(); // Cena služby
                qDebug() << serviceName;

                if (currentUser->getUserType() == "company") {
                    Company* company = dynamic_cast<Company*>(currentUser);
                    if(company->isPlatcaDPH())
                        // Pridáme DPH k cene
                        servicePrice = servicePrice / 1.20;  // 20% DPH

                }

                // Naplniť ComboBox s názvom služby a upravenou cenou (ak je platca DPH)
                ui->comboBoxService->addItem(serviceName + " - " + QString::number(servicePrice, 'f', 2) + " €");
            }
        }
    }
    file.close();


}

void MainWindow::setCurrentUser(User* user) {
    currentUser = user;

    if (currentUser) {
        fillCarInfo(*currentUser);  // Naplniť údaje o aute
        qDebug() << "Logged-in user set: " << currentUser;
        VisibleTab(); //sprístupné potrebný Tab
    }

}


//-----informácie o aute do Linedit

void MainWindow::fillCarInfo(const User &user)
    {
        // Prepnúť na prvý tab
        ui->tabWidget->setCurrentIndex(0);

        // Zistenie, či ide o fyzickú osobu alebo firmu
        if (user.getUserType() == "physical") {
            // Ak je to fyzická osoba, naplni údaje auta
            const PhysicalPerson *person = dynamic_cast<const PhysicalPerson*>(&user);

            if (person) {
                ui->lineEditCarBrand->setText(person->getCarBrand());
                ui->lineEditCarModel->setText(person->getCarModel());
                ui->lineEditCarYear->setText(QString::number(person->getCarYear()));
            } else {
                qDebug() << "Chyba: Objekt nie je typu PhysicalPerson!";
            }

        } else if (user.getUserType() == "company") {
            // Ak je to firma, naplni údaje o firme
            const Company *company = dynamic_cast<const Company*>(&user);

            if (company) {
                // Vymaž všetky údaje, pretože firma nemá údaje o aute
                ui->lineEditCarBrand->clear();
                ui->lineEditCarModel->clear();
                ui->lineEditCarYear->clear();
            } else {
                qDebug() << "Chyba: Objekt nie je typu Company!";
        }
    }
}


//-----informácie o aute do Linedit


//Fill list Widget so vsetkymi usermi
void MainWindow::fillListWidgetUsers()
{
    // Vymazanie existujúceho obsahu zoznamu
    ui->listWidgetUsers->clear();
    // Naplnenie zoznamu fyzickými osobami
    for (int i = 0; i < physicalPersons.size(); ++i) {
        PhysicalPerson* person = physicalPersons[i];
        QString itemText = QString("Username: %1\ncelé meno: %2\ntyp: Fyzická osoba")
                               .arg(person->getUsername())
                               .arg(person->getFullName());
        QListWidgetItem* item = new QListWidgetItem(itemText);
        ui->listWidgetUsers->addItem(item);
    }

    // Naplnenie zoznamu firmami
    for (int i = 0; i < companies.size(); ++i) {
        Company* company = companies[i];
        QString itemText = QString("username: %1\nnázov: %2\ntyp: firma")
                               .arg(company->getUsername())
                               .arg(company->getCompanyName());
        QListWidgetItem* item = new QListWidgetItem(itemText);
        ui->listWidgetUsers->addItem(item);
    }

    // Naplnenie zoznamu zamestnancami
    for (int i = 0; i < workers.size(); ++i) {
        Worker* worker = workers[i];
        QString itemText = QString("username: %1\npozícia: %2")
                               .arg(worker->getUsername())
                               .arg(worker->getPosition());
        QListWidgetItem* item = new QListWidgetItem(itemText);
        ui->listWidgetUsers->addItem(item);
    }

    qDebug() << "ListWidgetUsers uspesne naplneny v Tab2.";
}




//Fill list Widget so vsetkymi usermi


void MainWindow::on_pushButtonCarOK_clicked()
{
    // Overíme, či je používateľ prihlásený
    if (!currentUser) {
        qDebug() << "Používateľ nie je prihlásený, z funkcie pushButtonCarOK!";
        return;
    }

    QString newCarBrand = ui->lineEditCarBrand->text();
    QString newCarModel = ui->lineEditCarModel->text();
    int newCarYear = ui->lineEditCarYear->text().toInt();

    // Určíme, aký typ používateľa máme
    if (currentUser->getUserType() == "physical") {
        // Ak ide o fyzickú osobu, upravíme údaje o aute
        PhysicalPerson* person = dynamic_cast<PhysicalPerson*>(currentUser);
        if (person) {
            person->setCarBrand(newCarBrand);
            person->setCarModel(newCarModel);
            person->setCarYear(newCarYear);
            qDebug() << "Údaje o aute boli upravené pre fyzickú osobu.";
        }
    } else if (currentUser->getUserType() == "company") {
        Company* company = dynamic_cast<Company*>(currentUser);
        if (company) {
            company->setCarBrand(newCarBrand);
            company->setCarModel(newCarModel);
            company->setCarYear(newCarYear);
            qDebug() << "Údaje o aute boli upravené pre fyzickú osobu.";
        }
    }

    // Môžeme tiež zobraziť zmenené údaje v GUI
    fillCarInfo(*currentUser);
}






//PRIDANIE, ODSTRANENIE SLUZBY Z TABULKY ----------------------------------------------------
void MainWindow::on_pushButtonAddService_clicked()
{
    // Získame vybranú službu z ComboBoxu
    QString selectedService = ui->comboBoxService->currentText();
    // "Služba - 20.00 €"
    QStringList serviceParts = selectedService.split(" - ");
    double servicePrice;
    if (serviceParts.size() == 2) {
        servicePrice = serviceParts[1].remove(" €").toDouble(); // Odstránime symbol €
    }

    // Získame stav objednávky (prednastavený ako "nová")
    QString orderStatus = "nová";

    // Vytvoríme novú objednávku
    Order newOrder(selectedService, servicePrice, orderStatus);


    // Pridáme objednávku do zákazníka
    if (currentUser) {
        currentUser->addOrder(newOrder);  // Uložíme objednávku do QVectoru zákazníka
    }
    // Pridáme nový riadok do QTableWidget pre zobrazenie objednávky
    int row = ui->tableWidgetOrders->rowCount();  // Získame aktuálny počet riadkov
    ui->tableWidgetOrders->insertRow(row);  // Vložte nový riadok

    // Pridáme dáta do jednotlivých buniek (sloupce: názov služby, cena, stav objednávky)
    ui->tableWidgetOrders->setItem(row, 0, new QTableWidgetItem(serviceParts[0]));  // Názov služby
    ui->tableWidgetOrders->setItem(row, 1, new QTableWidgetItem(QString::number(servicePrice, 'f', 2) + " €"));  // Cena služby
    ui->tableWidgetOrders->setItem(row, 2, new QTableWidgetItem(orderStatus));  // Stav objednávky

    qDebug() << "Objednávka pridaná: " << selectedService << " - " << servicePrice << " €";
}

void MainWindow::on_pushButtonRemoveOrder_clicked()
{
    int row = ui->tableWidgetOrders->currentRow();

    // vybrana sluzby z tabulky
    //ošetrenie, že môžem zrušiť iba nové objednávky
    QString status = ui->tableWidgetOrders->item(row, 2)->text();
    if (status != "nová")
    {
        qDebug() << "Nemôžete odobrať objednávku, ktorá nie je nová!!. ";
        return;
    }



    QString serviceName = ui->tableWidgetOrders->item(row, 0)->text(); // nazov služby
    double price = ui->tableWidgetOrders->item(row, 1)->text().remove(" €").toDouble(); // cena


    // If no row is selected, do nothing
    if (row == -1) {
        qDebug() << "No order selected for removal!";
        return;
    }

    // Get the list of orders for the current user
    QVector<Order>* userOrders = nullptr;

    // pristup do oderlistu
    if (currentUser->getUserType() == "physical") {
        PhysicalPerson* person = dynamic_cast<PhysicalPerson*>(currentUser);
        userOrders = &person->getOrders();
    } else if (currentUser->getUserType() == "company") {
        Company* company = dynamic_cast<Company*>(currentUser);
        userOrders = &company->getOrders();
    }

    // odstranenie z objenavok
    Order orderToRemove(serviceName, price, status);

    // odstran z listu objednavok
    currentUser->removeOrder(orderToRemove);

    // odstran z tablewidgetu
    ui->tableWidgetOrders->removeRow(row);

    // do debugu
    qDebug() << "Order removed: " << serviceName << " - " << price << " €";
}

//PRIDANIE, ODSTRANENIE SLUZBY Z TABULKY



//FAKTURA---------------------------------------------------------
void MainWindow::on_pushButtonFaktura_clicked()
{
    QFile file("faktura.txt"); //uloží tam, kde sa aplikácia spúšťa
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "súbor na faktúru sa nepodarilo otvoriť!");
        return;
    }

    QTextStream out(&file);
    double totalPrice = 0.0;

    out << "Faktúra\n";
    out << "-------------------------\n";

    for (const Order &order : currentUser->getOrders()) {
        out << order.getService()<< "\t"
            << QString::number(order.getPrice(), 'f', 2) << " €\n";
        totalPrice += order.getPrice();
    }
    out << "-------------------------\n";
    out << "Celková cena: " << QString::number(totalPrice, 'f', 2) << " €\n";

    file.close();
    QMessageBox::information(this, "Success", "Faktura bola vytvorena.");
}
//FAKTURA---------------------------------------------------------




//Ukladanie objednavok pre kazdeho uzivatela do osobitneho

void MainWindow::saveOrdersToFile(const QString& username) {
    QString filename = QString("orders_%1.txt").arg(username);
    QFile file(filename);

    // Skontrolujeme, či súbor neexistuje
    if (!QFileInfo::exists(filename)) {
        qDebug() << "Súbor neexistuje. Bude vytvorený:" << filename;
    }

    // Otvorenie súboru na zápis alebo pridanie
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        qDebug() << "Chyba pri otváraní súboru na zápis:" << filename;
        return;
    }

    QTextStream out(&file);

    // Pridáme objednávky do súboru
    for (const Order& order : currentUser->getOrders()) {
        out << order.getService() << "," << order.getPrice() << "," << order.getStatus() << "\n";
    }

    file.close();
}



//Ukladanie objednavok pre kazdeho uzivatela do osobitneho suboru



void MainWindow::on_pushButtonLoggOutUser_clicked()
{
    // je prihlásený?
    if (currentUser) {
        QVector<Order> orders;

        // pretypovanie
        // iba company a ph.p môžu robiť objednávky
        if (currentUser->getUserType() == "physical") {
            PhysicalPerson* person = dynamic_cast<PhysicalPerson*>(currentUser);
            orders = person->getOrders();
        } else if (currentUser->getUserType() == "company") {
            Company* company = dynamic_cast<Company*>(currentUser);
            orders = company->getOrders();
        }

        // Pre pracovníka neukladám objednávky
        else if (currentUser->getUserType() == "worker") {
            qDebug() << "Pracovník objednávky neukladám.";
        }


        //ukladá objednávky, iba ak to nie je worker
        if (currentUser->getUserType() != "worker") {
            saveOrdersToFile(currentUser->getUsername());
        }

        // nulový pointer, neukazuje na nikoho, nikto nie je prihlásený
        currentUser = nullptr;

        qDebug() << "Používateľ bol odhlásení.";

        // mainwindow sa zakryje
        this->hide();

        // Vyčistenie existujúcich dát pred načítaním
        physicalPersons.clear();
        companies.clear();
        workers.clear();

        // Načítanie používateľov zo súboru
        loadUsersFromFile(":/resources/resources/users.txt");

        //znova inicializácia loggindialogu, rovnaka logika, ako hore
        LoginDialog loginDialog(this, this);
        loginDialog.setUsers(physicalPersons, companies, workers);

        if (loginDialog.exec() == QDialog::Accepted) {
            User* loggedInUser = loginDialog.getLoggedInUser();
            if (loggedInUser) {
                setCurrentUser(loggedInUser);  // Set currentUsera, úspešne prihláseného
                qDebug() << "User logged in: " << currentUser->getUsername();
                loadServicesFromFile(":/resources/resources/orders.txt");
                loadOrdersFromFile(currentUser->getUsername());
                this->show();
            }
        } else {
            qDebug() << "LoginDialog: Login canceled.";
            close();  // zatvor aplikáciu, nepodarilo sa
        }
    } else {
        qDebug() << "No user is logged in.";
    }
}





