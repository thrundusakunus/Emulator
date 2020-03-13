#include "classes.h"
#include "tests.h"
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QVector>

void labels(MainWindow * w){

    QLabel * controllerLabel = new QLabel(w);
    controllerLabel->setText("Řadič");
    controllerLabel->setGeometry(140, 20, 200, 20);
    controllerLabel->show();

    QLabel * compilerLabel = new QLabel(w);
    compilerLabel->setText("Kompilátor");
    compilerLabel->setGeometry(370, 20, 200, 20);
    compilerLabel->show();

    QLabel * driverLabel = new QLabel(w);
    driverLabel->setText("Ovladač");
    driverLabel->setGeometry(620, 20, 200, 20);
    driverLabel->show();

    QLabel * deviceLabel = new QLabel(w);
    deviceLabel->setText("Zařízení");
    deviceLabel->setGeometry(870, 20, 200, 20);
    deviceLabel->show();

}


int main(int argc, char *argv[])
{
    Test * test = new Test;
    test->runTests();

    QApplication a(argc, argv);
    MainWindow * w = new MainWindow(100, 100, 1050, 800);
    w->show();


    labels(w);


    Editor * driverWindow = new Editor(w, 550, 50, 200, 500);
    driverWindow->readOnly();

    Editor * deviceWindow = new Editor(w, 800, 50, 200, 500);
    deviceWindow->readOnly();


    CompileButton * compileButton = new CompileButton(100, 570, 100, 30, w);
    compileButton->setText("Kompilovat");
    compileButton->show();


    Compiler * compiler = new Compiler( new Editor(w, 300, 50, 200, 500) );
    Controller * controller = new Controller(new Editor(w, 50, 50, 200, 500), compiler, driverWindow);
	   compiler->controller = controller;

     QObject::connect(compileButton, &CompileButton::clicked, driverWindow, &Editor::clear);
    //propojeni tlacitka a radice
    QObject::connect(compileButton, &CompileButton::clicked, controller, &Controller::loadText);

    DeviceManager * deviceManager = new DeviceManager(new Editor(w, 300, 650, 400, 100), controller);

    CompileButton * managerButton = new CompileButton(750,670,150,60,w);
    managerButton->setText("Spravovat zařízení");
    managerButton->show();

    QObject::connect(managerButton, &CompileButton::clicked, deviceManager, [deviceManager, deviceWindow]{ deviceManager->proceed(deviceWindow); });

    deviceManager->connectDevice(new Head(deviceWindow), 2);
    //deviceManager->disconnectDevice(3);


    return a.exec();
}
