#pragma once
// Qt basic libraries.
#include <QList>
#include <QString>
#include <QWidget>
#include <QtNetwork/QHostInfo>
// qmdnsengine
#include <qmdnsengine/hostname.h>
#include <qmdnsengine/message.h>
#include <qmdnsengine/provider.h>
#include <qmdnsengine/query.h>
#include <qmdnsengine/record.h>
#include <qmdnsengine/server.h>
#include <qmdnsengine/service.h>

class Publisher : public QObject {
private:
    Q_OBJECT
    QMdnsEngine::Server server;
    QMdnsEngine::Hostname hostname;
    QMdnsEngine::Provider provider;
    QMdnsEngine::Service service;

public:
    Publisher(const QString& device_name, QWidget* parent = nullptr);

private slots:
    void onHostnameChanged(QByteArray hostname);

public slots:
    void onChangeServiceName(QString service_name);
};
