#pragma once

#include <QString>
#include <gst/gst.h>
#include <gst/video/video.h>
#include <stdio.h>

static gboolean on_message(GstBus*, GstMessage*, gpointer );

int srcincast(int argc=0,char** argv = nullptr);
