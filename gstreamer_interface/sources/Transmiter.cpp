#include "Transmiter.h"

void Transmiter::addLinkVideo() {
    GstElement *ximagesrc, *queue1, *capsfilter1, *capsfilter2, *videoscale, *videoconvert1, *videoconvert2, *rtpvp8pay, *queue2, *udpsink1;
    GstCaps *caps1, *caps2;
    if (customData.pipeline == NULL) {
        customData.pipeline = gst_pipeline_new("video");
    }
    //customData.pipeline = gst_pipeline_new("my-pipeline");
    ximagesrc = gst_element_factory_make("ximagesrc", "ximagesrc");
    // queue1 = gst_element_factory_make("queue", "queue1");
    // capsfilter1 = gst_element_factory_make("capsfilter", "capsfilterVideo1");
    videoconvert1 = gst_element_factory_make("videoconvert", "videoconvert1");
    videoscale = gst_element_factory_make("videoscale", "videoscale");
    capsfilter1 = gst_element_factory_make("capsfilter", "capsfilterVideo1");
    videoconvert2 = gst_element_factory_make("videoconvert", "videoconvert2");
    queue1 = gst_element_factory_make("queue", "queue1");
    vp8enc = gst_element_factory_make("vp8enc", "vp8enc");
    // capsfilter2 = gst_element_factory_make("capsfilter", "capsfilterVideo2");
    rtpvp8pay = gst_element_factory_make("rtpvp8pay", "rtpvp8pay");
    queue2 = gst_element_factory_make("queue", "queue2");
    udpsink1 = gst_element_factory_make("udpsink", "udpsink1");

    if (!customData.pipeline || !ximagesrc || !vp8enc || !videoconvert1 || !videoscale || !videoconvert2 || !queue2 || !queue1 ||
        !rtpvp8pay || !udpsink1 || !capsfilter1) {
        g_printerr("Not all elements could be created Transmiter\n");
        return;
    }
    caps1 =
        gst_caps_new_simple("video/x-raw", "profile", G_TYPE_STRING, "main", "width", G_TYPE_INT, 1024, "height", G_TYPE_INT, 600, NULL);

    g_object_set(G_OBJECT(capsfilter1), "caps", caps1, NULL);

    gst_caps_unref(caps1);

    gst_bin_add_many(GST_BIN(customData.pipeline), ximagesrc, videoconvert1, videoscale, capsfilter1, videoconvert2, queue1, vp8enc,
                     rtpvp8pay, queue2, udpsink1, NULL);

    if (gst_element_link_many(ximagesrc, videoconvert1, videoscale, capsfilter1, videoconvert2, queue1, vp8enc, rtpvp8pay, queue2, udpsink1,
                              NULL) != TRUE) {
        g_printerr(
            "Failed to link elements: ximagesrc -> videoscale -> videoconvert -> x264enc -> "
            "rtph264pay -> udpsink1\n");
        gst_object_unref(customData.pipeline);
        return;
    }

    g_object_set(udpsink1, "sync", FALSE, "host", representIP(ip_address), "port", video_port, NULL);
    // g_object_set(x264enc, "pass", 17, "tune", 4, "bitrate", 2000, "speed-preset", 0x00000005, NULL);
    g_object_set(G_OBJECT(vp8enc), "deadline", 1, "target-bitrate", 2000000, NULL);
}

void Transmiter::addLinkAudio() {
    GstElement *alsasrc, *audioconvert, *audioresample, *opusenc, *rtpopuspay, *udpsink2;
    if (customData.pipeline == NULL) {
        customData.pipeline = gst_pipeline_new("audio");
    }

    alsasrc = gst_element_factory_make("alsasrc", "alsasrc");
    audioconvert = gst_element_factory_make("audioconvert", "audioconvert");
    audioresample = gst_element_factory_make("audioresample", "audioresample");
    opusenc = gst_element_factory_make("opusenc", "opusenc");
    rtpopuspay = gst_element_factory_make("rtpopuspay", "rtpopuspay");
    udpsink2 = gst_element_factory_make("udpsink", "udpsink2");

    if (!customData.pipeline || !alsasrc || !audioconvert || !audioresample || !opusenc || !rtpopuspay || !udpsink2) {
        g_printerr("Not all elements could be created.Transmiter\n");
        return;
    }

    gst_bin_add_many(GST_BIN(customData.pipeline), alsasrc, audioconvert, audioresample, opusenc, rtpopuspay, udpsink2, NULL);

    if (gst_element_link_many(alsasrc, audioconvert, audioresample, opusenc, rtpopuspay, udpsink2, NULL) != TRUE) {
        g_printerr(
            "Failed to link elements: ximagesrc -> videoscale -> videoconvert -> x264enc -> "
            "rtph264pay -> udpsink1\n");
        gst_object_unref(customData.pipeline);
        return;
    }

    g_object_set(udpsink2, "sync", FALSE, "host", representIP(ip_address), "port", audio_port, NULL);
}

void Transmiter::startSend() {
    customData.loop = g_main_loop_new(NULL, FALSE);
    customData.bus = gst_element_get_bus(customData.pipeline);
    // qDebug() << data.pipeline;

    gst_bus_add_watch(customData.bus, static_cast<GstBusFunc>(busCallback), this);

    // gst_element_set_state(data.pipeline, GST_STATE_NULL);
    gst_element_set_state(customData.pipeline, GST_STATE_PLAYING);
    gst_bus_timed_pop_filtered(customData.bus, GST_CLOCK_TIME_NONE, static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
}

Transmiter::Transmiter(const QHostAddress& ip_address_, const qint32 video_port_, const qint32 audio_port_)
    : Session(ip_address_, video_port_, audio_port_) {
    qDebug() << "Constructor Transmiter" << this;
}

Transmiter::Transmiter(const QHostAddress& ip_address_, const qint32 audio_port_) : Session(ip_address_, audio_port_) {
    qDebug() << "Constructor Transmiter" << this;
}

Transmiter::~Transmiter() {
    qDebug() << "Destructor Transmiter"
             << "(" << this << ")";
    QString pipelineName = gst_element_get_name(customData.pipeline);
    emit closeWindow(ip_address, pipelineName);
    onKillSession();
}

void Transmiter::onEbableCamera() {
    qDebug() << "onEbableCamera Transmiter ";
    GstElement *v4l2src, *videoconvert, *videoscale, *vp8enc, *rtpvp8pay, *udpsink;

    gst_init(nullptr, nullptr);
    if (customData.pipeline == NULL) {
        customData.pipeline = gst_pipeline_new("camera");
    }

    v4l2src = gst_element_factory_make("v4l2src", "v4l2src");
    videoconvert = gst_element_factory_make("videoconvert", "videoconvert");
    videoscale = gst_element_factory_make("videoscale", "videoscale");
    // capsfilter = gst_element_factory_make("capsfilter", "capsfilterVideo1");
    vp8enc = gst_element_factory_make("vp8enc", "vp8enc");
    rtpvp8pay = gst_element_factory_make("rtpvp8pay", "rtpvp8pay");
    udpsink = gst_element_factory_make("udpsink", "udpsink1");
    if (!customData.pipeline || !v4l2src || !vp8enc || !videoconvert || !videoscale || !rtpvp8pay || !udpsink) {
        g_printerr("Not all elements could be created Transmiter\n");
        return;
    }

    g_object_set(v4l2src, "device", "/dev/video0", NULL);

    g_object_set(udpsink, "sync", FALSE, "host", representIP(ip_address), "port", 5001, NULL);

    gst_bin_add_many(GST_BIN(customData.pipeline), v4l2src, videoconvert, videoscale, vp8enc, rtpvp8pay, udpsink, NULL);

    if (gst_element_link_many(v4l2src, videoconvert, videoscale, vp8enc, rtpvp8pay, udpsink, NULL) != TRUE) {
        g_printerr(
            "Failed to link elements: v4l2src -> videoscale -> videoconvert -> x264enc -> "
            "rtph264pay -> udpsink1\n");
        gst_object_unref(customData.pipeline);
        return;
    }

    startSend();
}

void Transmiter::onEnableVideo() {
    qDebug() << "onEnableVideo TRANSMITER";
    if (!customData.pipeline) {
        gst_init(nullptr, nullptr);
        addLinkVideo();
        startSend();
    } else {
        gst_object_unref(customData.bus);
        gst_object_unref(customData.loop);
        gst_element_set_state(customData.pipeline, GST_STATE_PAUSED);

        addLinkVideo();
        startSend();
    }
}

void Transmiter::onEnableAudio() {
    if (!customData.pipeline) {
        gst_init(nullptr, nullptr);
        addLinkAudio();
        startSend();
    } else {
        gst_object_unref(customData.bus);
        gst_object_unref(customData.loop);
        gst_element_set_state(customData.pipeline, GST_STATE_PAUSED);

        addLinkAudio();
        startSend();
    }
}

void Transmiter::onStartSession() {}

void Transmiter::onKillSession() {
    gst_element_set_state(customData.pipeline, GST_STATE_NULL);
}

void Transmiter::onSetBitrate(const int bitrate) {
    qDebug() << "set bitrate";
    gst_element_set_state(customData.pipeline, GST_STATE_PAUSED);
    g_object_set(G_OBJECT(vp8enc), "target-bitrate", bitrate, NULL);

    gst_element_set_state(customData.pipeline, GST_STATE_PLAYING);
}
