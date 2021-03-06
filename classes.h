#include "devicesDefinitions.h"
#include "driversDefinitions.h"

#ifndef CLASSES_H
#define CLASSES_H

class Driver;
class DriverLibrary;
class Controller;
class SystemLibrary;
class Compiler;
class Queue;
class ProcessLoop;
class DeviceWindowWrapper;


class Error{

public:
  QVector<QString> getVector_string();
  int getInt();
  float getFloat();
  QString getString();
  Error();

private:
  QVector<QString> E_VECTOR_STRING = {"E", "R", "R", "O", "R"};
  int E_INT = -1000000;
  float E_FLOAT = 74.236;
  QString E_STRING = "dwavvxwčš§ů";

};

class Call{

public:
  Call();
  ~Call();
  virtual void doCommand() = 0;

protected:
  QString command;
};

class DriverCall : public Call{

public:
  DriverCall(DriverLibrary * driverLib, QString com, int portNum);
  ~DriverCall();
  void doCommand();

private:
  DriverLibrary * driverLibrary;
  int portNumber;

};

class SystemCall : public Call{

public:
  SystemCall(SystemLibrary * sysLib, QString com);
  ~SystemCall();
  void doCommand();

private:
  SystemLibrary * systemLibrary;

};

class ErrorCall : public Call{

public:
  ErrorCall(Compiler * comp, QString message, QString com);
  ~ErrorCall();
  void doCommand();

private:
  Compiler * compiler;
  QString errorMessage;

};

class Arguments{

public:
  void addArgument(QString arg);
  QString getArgument(int position);

private:
  QVector<QString> argsVector;

};

class Parser{

public:
  Parser(QString parseText);
  QVector<QString> parse();
  QString getParser();
  int getPortNumber(QString rawCommand);
  QString getCommand(QString rawCommand);
  Arguments * getArguments(QString rawCommand);

private:
  QString rawParser;

};

class DeviceParser{

public:
  QVector<QString> separateWords(QString sentence);

};

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    int x, y, width, height;

    MainWindow(int left, int top, int w, int h, QWidget *parent = nullptr);
    ~MainWindow();
};

class Editor : public QWidget{
    Q_OBJECT
public:
    int x, y, width, height;

    Editor(QWidget * par, int left, int top, int w, int h);
    Editor();
    void readOnly();
    void setText(QString text);
    QString getText();

public slots:
  void clear();

protected:
    QTextEdit * me;
    QWidget * parent;

private:
    void show();
};

class CompileButton : public QObject{
Q_OBJECT
public:
    int x, y, width, height;

    CompileButton(int left, int top, int w, int h, QWidget * par);
    void show();
    void setText(const QString text);

signals:
    void clicked(); //kliknuto na *me

private:
    QPushButton * me;
    QWidget * parent;
};

/*
class Head : public Device{
  Q_OBJECT
public:
  Head();

protected slots:
  void work();
};*/


/*class ErrorDevice : public Device{
 Q_OBJECT
public:
    ErrorDevice();

protected slots:
    void work();
};*/


class Port{
public:
    Device * device = nullptr;
    Port();
};



class SystemLibrary{

public:
  bool existCommand(QString command);
  void doCommand(QString command);
  void delay(Queue * queue);
  SystemLibrary(Controller * IC, Editor * w);

private:
  QVector<QString> functions = {"delay"};
  Controller * controller;
  Editor * window;

};


class CompilerErrorHandler{

  bool checkSyntax(Parser * parser, QString rawCommand);

};


class Compiler{
public:
    Compiler(Editor * editor);
    void validate(Parser *parser, ProcessLoop * processLoop, Controller * controller);
    void print(QString message);
    Queue * createQueue(QVector<QString> commandList, Parser * parser, Controller * controller);

private:
	Editor * window;
  QVector<QString> Errors = {"OK", "Syntax error", "Command does not exist", "Invalid data type", "Port does not exist"};
  CompilerErrorHandler * errorHandler;

  Call * createCall(Parser * parser, QString rawCommand, Controller * controller);
  QString deleteSpaces(QString command);
  void callQueue(QVector<Call*> queue, int position);
};

class Controller : public QObject{
Q_OBJECT
public:
	  DriverLibrary * driverLibrary;
    SystemLibrary * systemLibrary;
    QVector<Port *> PORTS;
    Controller(Editor * editor, Editor * driverWindow);
    int getControlPeriod();
    QVector<int> getDevicePeriods();

public slots:
    void loadText(Compiler * compiler, ProcessLoop * processLoop);   //zapocni nacteni kodu, validaci etc.
    int getNumberOfPorts();

private:
    const int period = 500;
    int portsNumber = 8;
    Editor * window;

};

class DeviceManager : public QObject{

Q_OBJECT
public:
    DeviceManager(Editor * w, Controller * IC);
    int numberOfDevices();
    void connectDevice(Device * device, int port);
    void disconnectDevice(int port);
    Device * deviceFromName(QString name);

public slots:
  void proceed();

private:
    Editor * window;
    Controller * controller;
    QVector<int> ports = {0, 0, 0, 0, 0, 0, 0, 0};
};
/*
class Driver{
public:
    virtual void execute(Device * device, QString command) = 0;
    virtual void processOutput(int deviceOutput, int port, DeviceWindowWrapper * deviceWindowWrapper ) = 0;
    int ID;
    QVector<QString> functions;
    int portNumber;

};

class HeadDriver : public Driver{
public:

    HeadDriver();
    void execute(Device * device, QString command);
    void blink(Device * device);
    void light_on(Device * device);
    void light_off(Device * device);
    void processOutput(int deviceOutput, int port, DeviceWindowWrapper * deviceWindowWrapper);
};
*/
class DriverLibrary{

public:
	bool existCommand(int pinNumber, QString command);
	void doCommand(int pinNumber, QString command);
	DriverLibrary(Controller* par, Editor * w);
  void processOutput(DeviceWindowWrapper * deviceWindowWrapper);

private:
	QVector<Driver *> drivers = {};
	Controller * controller;
  Editor * window;
};

class Queue : public QObject{
Q_OBJECT
public:
  Queue();
  ~Queue();
  void appendCommand(Call * commandCall);
  int getLength();
  Call * getCommand(int position);
  void call(int position);


public slots:
  void callQueue();

private:
  QVector<Call*> queueVector;
  int position = 0;

};

class ProcessLoop : public QObject{

Q_OBJECT
public:
  ProcessLoop(Controller * controller, DeviceWindowWrapper * deviceWindowWrapper);
  void start(Queue * q, QVector<int> periods);
  int getTickDelay();

public slots:
  void stop();

private slots:
  void cycle(Controller * controller, DeviceWindowWrapper * deviceWindowWrapper);

private:
  int tickDelay;
  QTimer * timer;
  Queue * queue;
  int calculateCommonPeriod(QVector<int> periods);
  int greatestCommonDivisor(int a, int b);

};

class DeviceWindowWrapper : public QObject{
Q_OBJECT

public:
  DeviceWindowWrapper(Editor * window);
  void setTextOnPort(int port, QString text);

public slots:
  void init(int portNum, Controller * controller);

private:
  Editor * window;

};

#endif // CLASSES_H
