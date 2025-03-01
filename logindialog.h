#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H


#include <QDialog>
#include <QString>
#include <QVector>
#include "mainwindow.h" //pristup k triedam

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr,  MainWindow *mainWindow =nullptr);
    ~LoginDialog();

    // Settery na prístup k rôznym zoznamom používateľov
    void setUsers(const QVector<PhysicalPerson*>& physicalPersons,
                  const QVector<Company*>& companies,
                  const QVector<Worker*>& workers);
    void naplnComboBox();
    //bool loggedIn(const QString& username, const QString& password);
    User* getLoggedInUser() const {
        if (loggedInUser) {
            return loggedInUser;
        }
        qDebug() << "loggedInUser is nullptr!";
        return nullptr;
    }


signals:
    void loginSuccessful(User* loggedInUser); // Signal na oznámenie úspešného prihlásenia

private slots:
    void on_pushButtonLogin_clicked();
private:
    Ui::LoginDialog *ui;
    QVector<PhysicalPerson*> physicalPersons; // Zoznam fyzických osôb
    QVector<Company*> companies;             // Zoznam firiem
    QVector<Worker*> workers;                // Zoznam zamestnancov
    MainWindow* mainWindow; // Premenná pre ukladanie odkazu na objekt MainWindow
    User* loggedInUser = nullptr;



};

#endif // LOGINDIALOG_H
