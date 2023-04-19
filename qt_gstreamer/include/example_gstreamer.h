#pragma once

#include <QString>
#include <gst/gst.h>
#include <string.h>

typedef struct _CustomData CustomData;

QString get_QString();
static void cb_message(GstBus*, GstMessage*, CustomData*);
int display(int, char**);
