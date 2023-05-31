#ifndef MANAGER_H
#define MANAGER_H

#include <QLabel>
#include <QPixmap>
#include <QShortcut>
#include <QDesktopWidget>
#include <QtGui/QScreen>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QLabel>
#include <QApplication>
#include <QBluetoothServer>
#include <QBluetoothAddress>
#include <QBluetoothLocalDevice>
#include <QTimer>

#include "bluetoothformatimage.h"
#include "client.h"
#include "server.h"

class Manager : public QWidget
{
    Q_OBJECT

public:

    BluetoothFormatImage* getImg();
    void setImg();
    explicit Manager(QWidget *parent = nullptr);
    ~Manager();

signals:
    void sendMessage(const QString &data);

public slots:
    void onStartTransmitt(){};
    void onStopTransmitt(){};

private slots:
    void onConnectClicked();
    void onSendStarted();
    void onSendStopped();

    void onShowMessage(const QString &sender, const QString &message);

    void onClientConnected(const QString &name);
    void onClientDisconnected(const QString &name);
    void onClientDisconnected();
    void onConnected(const QString &name);
    void onReactOnSocketError(const QString &error);

    void onNewAdapterSelected();

private:
    int adapterFromUserSelection() const;
    void captureScreen();
    int current_adapter_index = 0;

    bool translation_status;
    QTimer* timer;
    Server *server;
    QList<Client *> clients;
    QList<QBluetoothHostInfo> local_adapters;
    QLabel* label;
    QString local_name;
    BluetoothFormatImage *img = nullptr;
};
#endif // MANAGER_H
