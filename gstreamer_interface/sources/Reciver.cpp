#include "Reciver.h"

#include "qevent.h"

void Reciver::addLinkVideo() {
    qDebug() << "onEnableVideo RECIVER";
    GstElement *pipeline, *capsfilter1, *capsfilter2, *udpsrc, *queue1, *vp8dec, *rtpvp8depay, *videoconvert, *queue2, *autovideosink;
    GstCaps *caps, *caps2;
    if (customData.pipeline == NULL) {
        customData.pipeline = gst_pipeline_new("video");
    }

    udpsrc = gst_element_factory_make("udpsrc", "udpsrc");
    queue1 = gst_element_factory_make("queue", "queue1");
    capsfilter1 = gst_element_factory_make("capsfilter", "capsfilterVideo1");
    rtpvp8depay = gst_element_factory_make("rtpvp8depay", "rtpvp8depay");
    vp8dec = gst_element_factory_make("vp8dec", "vp8dec");
    videoconvert = gst_element_factory_make("videoconvert", "videoconvert");
    capsfilter2 = gst_element_factory_make("capsfilter", "capsfilterVideo2");
    queue2 = gst_element_factory_make("queue", "queue2");
    autovideosink = gst_element_factory_make("glimagesink", "videosink");

    if (!customData.pipeline || !udpsrc || !rtpvp8depay || !vp8dec || !videoconvert || !autovideosink || !queue1 || !queue2 ||
        !capsfilter1 || !capsfilter2) {
        g_printerr("Not all elements could be created. Exiting.\n");
        return;
    }

    caps = gst_caps_new_simple("application/x-rtp", "media", G_TYPE_STRING, "video", "clock-rate", G_TYPE_INT, 90000, "encoding-name",
                               G_TYPE_STRING, "VP8", "payload", G_TYPE_INT, 96, NULL);

    caps2 = gst_caps_new_simple("video/x-raw", "width", G_TYPE_INT, 1024, "height", G_TYPE_INT, 600, NULL);

    g_object_set(G_OBJECT(capsfilter1), "caps", caps, NULL);
    g_object_set(G_OBJECT(capsfilter2), "caps", caps2, NULL);
    gst_caps_unref(caps);
    gst_caps_unref(caps2);

    gst_bin_add_many(GST_BIN(customData.pipeline), udpsrc, queue1, capsfilter1, rtpvp8depay, vp8dec, videoconvert, queue2, capsfilter2,
                     autovideosink, NULL);

    if (!gst_element_link_many(udpsrc, queue1, capsfilter1, rtpvp8depay, vp8dec, videoconvert, queue2, capsfilter2, autovideosink, NULL)) {
        g_printerr("Could not link all elements. Exiting.\n");
        return;
    }

    g_object_set(udpsrc, "address", representIP(ip_address), "port", video_port, NULL);

    window = new QWidget();
    window->resize(1024, 600);
    window->show();
    WId xwinid = window->winId();
    gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(autovideosink), xwinid);
    window->installEventFilter(this);
}

void Reciver::addLinkAudio() {
    GstElement *udpsrc, *depay, *parse, *decode, *convert, *autovideosink, *audioresample, *capsfilter, *queue;
    GstCaps* caps;
    if (customData.pipeline == NULL) {
        customData.pipeline = gst_pipeline_new("audio");
    }
    udpsrc = gst_element_factory_make("udpsrc", "udpsrcAudio");
    queue = gst_element_factory_make("queue", "bufferAudio");
    capsfilter = gst_element_factory_make("capsfilter", "capsfilterAudio");
    depay = gst_element_factory_make("rtpopusdepay", "depayAudio");
    parse = gst_element_factory_make("opusparse", "parseAudio");
    decode = gst_element_factory_make("avdec_opus", "decodeAudio");
    convert = gst_element_factory_make("audioconvert", "convertAudio");
    audioresample = gst_element_factory_make("audioresample", "audioresampleAudio");
    volume = gst_element_factory_make("volume", "volumeAudio");
    autovideosink = gst_element_factory_make("autoaudiosink", "autovideosinkAudio");

    if (!customData.pipeline || !udpsrc || !depay || !parse || !decode || !convert || !autovideosink || !audioresample || !capsfilter ||
        !queue || !volume) {
        g_printerr("Not all elements could be created. Exiting.\n");
        return;
    }

    caps = gst_caps_new_simple("application/x-rtp", "media", G_TYPE_STRING, "audio", "clock-rate", G_TYPE_INT, 48000, "encoding-name",
                               G_TYPE_STRING, "OPUS", "payload", G_TYPE_INT, 96, NULL);

    g_object_set(G_OBJECT(capsfilter), "caps", caps, NULL);

    gst_caps_unref(caps);
    g_object_set(volume, "volume", 0.5, NULL);

    gst_bin_add_many(GST_BIN(customData.pipeline), udpsrc, queue, capsfilter, depay, parse, decode, convert, audioresample, volume,
                     autovideosink, NULL);

    if (!gst_element_link_many(udpsrc, queue, capsfilter, depay, parse, decode, convert, audioresample, volume, autovideosink, NULL)) {
        g_printerr("Could not link all elements. Exiting.\n");
        return;
    }

    g_object_set(udpsrc, "address", representIP(ip_address), "port", audio_port, NULL);
}

void Reciver::startReceive() {
    gst_element_set_state(customData.pipeline, GST_STATE_NULL);
    gst_element_set_state(customData.pipeline, GST_STATE_PLAYING);
    customData.loop = g_main_loop_new(NULL, FALSE);
    customData.bus = gst_element_get_bus(customData.pipeline);

    gst_bus_add_watch(customData.bus, static_cast<GstBusFunc>(busCallback), this);

    gst_object_unref(customData.bus);

    g_main_loop_run(customData.loop);
}

Reciver::Reciver(const QHostAddress& ip_address_, const qint16 video_port, const qint16 audio_port)
    : Session(ip_address_, video_port, audio_port) {
    qDebug() << "Constructor Reciver" << this;
    if (!g_thread_supported()) {
        g_thread_init(NULL);
    }
}

Reciver::Reciver(const QHostAddress& ip_address_, const qint16 audio_port) : Session(ip_address_, audio_port) {
    qDebug() << "Constructor Reciver" << this;
    if (!g_thread_supported()) {
        g_thread_init(NULL);
    }
}

Reciver::~Reciver() {
    qDebug() << "Destructor Receiver"
             << "(" << this << ")";
    onKillSession();
    if (window != nullptr){
      delete window;
    }
}

bool Reciver::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::Close) {
        QString pipelineName = gst_element_get_name(customData.pipeline);
        emit closeWindow(ip_address, pipelineName);
        return true;
    }

    return QObject::eventFilter(obj, event);
}

void Reciver::onEbableCamera() {
    qDebug() << "onEbableCamera Reciver";
    gst_init(nullptr, nullptr);

    GstElement *source, *queue1, *udpsrc, *depay, *decoder, *queue2, *sink;
    GstCaps *caps, *caps2;
    if (customData.pipeline == NULL) {
        customData.pipeline = gst_pipeline_new("camera");
    }

    source = gst_element_factory_make("udpsrc", "udp-source");
    queue1 = gst_element_factory_make("queue", "queue1");
    depay = gst_element_factory_make("rtpvp8depay", "rtpvp8-depay");
    decoder = gst_element_factory_make("vp8dec", "vp8-decoder");
    queue2 = gst_element_factory_make("queue", "queue3");
    sink = gst_element_factory_make("glimagesink", "video-sink");

    if (!customData.pipeline || !source || !queue1 || !depay || !decoder || !queue2 || !sink) {
        g_printerr("Not all elements could be created. Exiting.\n");
        return;
    }

    g_object_set(G_OBJECT(source), "caps",
                 gst_caps_new_simple("application/x-rtp", "encoding-name", G_TYPE_STRING, "VP8", "payload", G_TYPE_INT, 96, NULL), NULL);

    gst_bin_add_many(GST_BIN(customData.pipeline), source, queue1, depay, decoder, queue2, sink, NULL);

    if (!gst_element_link_many(source, queue1, depay, decoder, queue2, sink, NULL)) {
        g_printerr("Could not link all elements. Reciver. Exiting.\n");
        return;
    }
    g_object_set(source, "port", 5001, NULL);
    window = new QWidget();
    window->resize(1024, 600);
    window->show();
    WId xwinid = window->winId();
    gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(sink), xwinid);
    window->installEventFilter(this);

    startReceive();
}

void Reciver::onSetVolume(float volume_) {
    g_object_set(volume, "volume", volume_, NULL);
}

void Reciver::onEnableVideo() {
    qDebug() << "onEnableVideo RECIVER";
    if (!customData.pipeline) {
        gst_init(nullptr, nullptr);
        addLinkVideo();
        startReceive();
    } else {
        gst_object_unref(customData.bus);
        gst_object_unref(customData.loop);
        gst_element_set_state(customData.pipeline, GST_STATE_PAUSED);

        addLinkVideo();
        startReceive();
    }
}

void Reciver::onEnableAudio() {
    if (!customData.pipeline) {
        gst_init(nullptr, nullptr);
        addLinkAudio();
        startReceive();
    } else {
        gst_object_unref(customData.bus);
        gst_object_unref(customData.loop);
        gst_element_set_state(customData.pipeline, GST_STATE_PAUSED);

        addLinkAudio();
        startReceive();
    }
}

void Reciver::onStartSession() {
}

void Reciver::onKillSession() {
    //removeVideo();
    gst_element_set_state(customData.pipeline, GST_STATE_NULL);
}
