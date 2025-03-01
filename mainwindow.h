#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <QList>
#include <QMessageBox>
#include <iostream>

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE



//TRIEDA OBJEDNÁVKA

class Order
{
private:
    QString service;
    double price;
    QString status; //Nová, v procese, hotová

public:
    Order(QString s, double p, QString st)
        : service(s), price(p), status(st)
    {
        qDebug() << "Vytváram Službu";
    }


    ~Order() //deštruktor, kvôli dedeniu virtual
    {
        qDebug() << "Zanika objekt triedy Služba...";
    }

    // Gettery
    QString getService() const { return service; }
    double getPrice() const { return price; }
    QString getStatus() const { return status; }


    // Funkcia na zmenu stavu objednávky
    void updateStatus(const QString &newStatus) {
        status = newStatus;
    }

    //preťaženie operátora, definícia == na porovnávanie služieb
    bool operator==(const Order &other) const {
        return (this->service == other.service) && (this->price == other.price);
    }


};

class Service
{

};








//TRIEDA OBJEDNÁVKA

//TRIEDY---------------------------------------------------------UŽÍVATELIA
//defaultný používateľ

class User
{
public:
    User(const QString& username, const QString& password)
        : username(username), password(password)
    {
        qDebug() << "Vytvára sa objekt triedy User... ";

    }
    virtual ~User() //deštruktor, kvôli dedeniu virtual
    {
        qDebug() << "Vytvára sa objekt triedy User...";
    }

    //Gettre, cez inline f
    QString getUsername() const { return username; }
    QString getPassword() const { return password; }

    // na prepis, virtualna, vypisanie informacii
    virtual QString getDetails() const = 0;
    virtual QString getUserType() const = 0;



    void addOrder(const Order &order)
    {
        orders.append(order); //pridanie novej objednávky
    }


    QVector<Order>& getOrders()
    {
            return orders; //vráti vektor objednávok
    }

    void removeOrder(const Order &order)
    {
        // Find the order in the orders list
        for (int i = 0; i < orders.size(); ++i) {
            if (orders[i] == order) {  // Assuming you have an `operator==` to compare orders
                orders.removeAt(i);     // Remove the order at index i
                qDebug() << "Order removed: " << order.getService();
                return;  // Exit after removing the order
            }
        }

        qDebug() << "Order not found: " << order.getService();  // Order not found in the list
    }


 // protected, lebo ich posuva dalej, "dieťaťu"
protected:
    QString username;
    QString password;
    QVector<Order> orders; //vektor objednávok zákazníka

};

//Dieťa User triedy
class PhysicalPerson: public User
{
public:
    PhysicalPerson(const QString& username, const QString& password, const QString& fullName,
                   const QString& carBrand, const QString& carModel, int carYear)
        : User(username, password), fullName(fullName),
        carBrand(carBrand), carModel(carModel), carYear(carYear)
    {
        qDebug() << "Vytvára sa objekt triedy Fyzická osoba... ";

    }

    //Gettre
    QString getFullName() const { return fullName; }
    QString getCarBrand() const { return carBrand; }
    QString getCarModel() const { return carModel; }
    int getCarYear() const { return carYear; }

    //vypis informacii, prepis defaultnej f v user class
    QString getDetails() const override {
        return "Fyzická osoba: " + fullName + ", Auto: " + carBrand + " " + carModel + " (" + QString::number(carYear) + ")";


    }

    // Setters for car information
    void setCarBrand(const QString& brand) { carBrand = brand; }
    void setCarModel(const QString& model) { carModel = model; }
    void setCarYear(int year) { carYear = year; }

    QString getUserType() const override { return "physical"; }
//private lebo budú to už len jeho dáta, žiadny dedič
private:
    QString fullName;
    QString carBrand;
    QString carModel;
    int carYear;

};


class Company: public User
{
public:
    Company(const QString& username, const QString& password, const QString& companyName,
            const QString& carBrand, const QString& carModel, int carYear, int ico, bool platcaDPH)
        : User(username, password), companyName(companyName), carBrand(carBrand), carModel(carModel),
        carYear(carYear), ico(ico), platcaDPH(platcaDPH)
    {
        qDebug() << "Vytvára sa objekt triedy Firma... ";
    }

    // Getter functions
    QString getCompanyName() const { return companyName; }
    QString getCarBrand() const { return carBrand; }
    QString getCarModel() const { return carModel; }
    int getCarYear() const { return carYear; }
    int getIco() const { return ico; }
    bool isPlatcaDPH() const { return platcaDPH; } //bool type, platca alebo nie


    QString getDetails() const override {
        return "Firma: " + companyName + ", IČO: " + QString::number(ico) + ", " + (platcaDPH ? "Platca DPH" : "Neplatca DPH");
    }


    QString getUserType() const override { return "company"; }

    // Setters for car information
    void setCarBrand(const QString& brand) { carBrand = brand; }
    void setCarModel(const QString& model) { carModel = model; }
    void setCarYear(int year) { carYear = year; }

private:
    QString companyName;
    QString carBrand;
    QString carModel;
    int carYear;
    int ico;
    bool platcaDPH;


};

// od Usera
class Worker : public User
{
public:
    Worker(const QString& username, const QString& password, const QString& position)
        : User(username, password), position(position)
    {
        qDebug() << "Vytvára sa objekt triedy Pracovník... ";
    }

    QString getPosition() const { return position; }

    //na akej pozíciií?
    bool isAdministrator() const { return position == "administrator"; }
    bool isTechnician() const { return position == "technician"; }


    QString getDetails() const override {
        return "Pracovník: " + getUsername() + ", pozícia: " + position;
    }

    QString getUserType() const override { return "worker"; }

private:
    QString position; //technik, administrátor
};


//TRIEDY---------------------------------------------------------UŽÍVATELIA


//------LOAD a SAVE OBJEDNÁVKY do SÚBORU







//------LOAD a SAVE OBJEDNÁVKY do SÚBORU




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //Vektory používateľov
    QVector<PhysicalPerson*> physicalPersons;
    QVector<Company*> companies;
    QVector<Worker*> workers;

    //Funkcie na načítanie a prácu so súbormi
    void loadUsersFromFile(const QString& fileName);
    //bool isLoggedIn(const QString& username, const QString& password);
    void saveOrdersToFile(const QString& username);
    void loadServicesFromFile(const QString& fileName);

    void loadOrdersFromFile(const QString& username);



    //Gettre pre vektory používateľov
    QVector<PhysicalPerson*> getPhysicalPersons() const { return physicalPersons; }
    QVector<Company*> getCompanies() const { return companies; }
    QVector<Worker*> getWorkers() const { return workers; }

    void setCurrentUser(User* user);
    void fillCarInfo(const User &user);

    //void saveUsersToFile(const QString& fileName);
    void VisibleTab();
    void fillListWidgetUsers();

public slots:
    void on_pushButtonCarOK_clicked();
    void on_pushButtonAddService_clicked();
    void on_pushButtonRemoveOrder_clicked();
    void on_pushButtonFaktura_clicked();
    void on_pushButtonLoggOutUser_clicked(); //zakaznik


private:
    Ui::MainWindow *ui;
    User* currentUser;
};
#endif // MAINWINDOW_H
