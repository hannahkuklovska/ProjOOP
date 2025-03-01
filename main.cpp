#include "mainwindow.h"
#include "logindialog.h"

#include <QApplication>
#include <QDebug>




/*
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();

    LoginDialog loginDialog(nullptr, &w);

    loginDialog.setUsers(w.getPhysicalPersons(), w.getCompanies(), w.getWorkers());

    int loginResult = loginDialog.exec(); // Spustí dialóg ako modal

    // Ak je prihlásenie úspešné, spustite MainWindow
    if (loginResult == QDialog::Accepted) {
        w.show();  // Zobrazte hlavné okno
    } else {
        return 0;  // Ak prihlásenie zlyhá, aplikácia sa ukončí
    }

    return a.exec();
}

*/



/*
#include <QFile>
#include <QDebug>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Testovanie načítania súboru z resources
    QFile file(":/resources/resources/users.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Chyba: Súbor sa nepodarilo otvoriť!";
        return -1;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        qDebug() << line; // Vypíše riadky na konzolu
    }
    file.close();

    MainWindow w;
    w.show();

    return a.exec();
}
*/
// Ak používateľ nie je nájdený


/*
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow mainWindow;

    qDebug() << "Inicializácia LoginDialog...";
    LoginDialog loginDialog(nullptr, &mainWindow);  // Vytvorenie objektu LoginDialog
    loginDialog.setUsers(mainWindow.physicalPersons, mainWindow.companies, mainWindow.workers);


    // Zobrazte LoginDialog a overte, či je výsledok správny
    if (loginDialog.exec() == QDialog::Accepted) {
        qDebug() << "LoginDialog: Prihlásenie úspešné.";

        // Ak sa LoginDialog uzavrie s úspechom, spustite MainWindow
        //MainWindow mainWindow;
        mainWindow.show();

        return app.exec();
    } else {
        qDebug() << "LoginDialog: Prihlásenie zrušené alebo zlyhalo.";
    }

    return 0;  // Aplikácia sa ukončí, ak prihlásenie zlyhá alebo je zrušené
}

*/

/*
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow mainWindow;

    // Inicializácia LoginDialog
    qDebug() << "Inicializácia LoginDialog...";
    LoginDialog loginDialog(nullptr, &mainWindow);  // Vytvorenie objektu LoginDialog
    loginDialog.setUsers(mainWindow.physicalPersons, mainWindow.companies, mainWindow.workers);

    if (loginDialog.exec() == QDialog::Accepted) {
        qDebug() << "LoginDialog: Prihlásenie úspešné.";

        // Ak je prihlásený používateľ, nastavíme ho v MainWindow
        User* loggedInUser = loginDialog.getLoggedInUser();
        if (loggedInUser) {
            mainWindow.setCurrentUser(loggedInUser);  // Nastavenie prihláseného používateľa
            mainWindow.show();
        }

        return app.exec();  // Spustíme aplikáciu po zobrazení hlavného okna
    } else {
        qDebug() << "LoginDialog: Prihlásenie zrušené alebo zlyhalo.";
    }

    return 0;  // Aplikácia sa ukončí, ak prihlásenie zlyhá alebo je zrušené
}
*/

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}

