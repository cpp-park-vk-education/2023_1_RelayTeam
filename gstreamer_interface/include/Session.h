#pragma once

#include <QThread>
#include <QString>
#include <gst/gst.h>


class Session : public QThread
{
public:
    void run() = 0;

protected:
    QString ip;//fix_it local_ip
    QString port;

};
