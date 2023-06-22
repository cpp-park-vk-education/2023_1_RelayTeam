#include "Transmiter.h"

void Transmiter::addLinkVideo() {
    GstElement *ximagesrc, *queue1, *capsfilter1, *capsfilter2, *videoscale, *videoconvert1, *videoconvert2, *rtpvp8pay, *queue2, *udpsink1;
    GstCaps *caps1, *caps2;
    if (data.pipeline == NULL) {
        data.pipeline = gst_pipeline_new("pipeline");
    }
    data.pipeline = gst_pipeline_new("my-pipeline");
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

    if (!data.pipeline || !ximagesrc || !vp8enc || !videoconvert1 || !videoscale || !videoconvert2 || !queue2 || !queue1 || !rtpvp8pay ||
        !udpsink1) {
        qDebug() << !data.pipeline << !ximagesrc << !vp8enc << !videoconvert1 << !videoscale << !videoconvert2 << !queue2 << !queue1
                 << !rtpvp8pay << !udpsink1;
        g_printerr("Not all elements could be created\n");
        return;
    }

    // caps1 = gst_caps_new_simple("video/x-raw", "framerate", GST_TYPE_FRACTION, 30, 1, NULL);

    caps1 = gst_caps_new_simple("video/x-raw", "profile", G_TYPE_STRING, "main", "width", G_TYPE_INT, 1024, "height", G_TYPE_INT, 600, NULL,
                                nullptr);

    // g_object_set(G_OBJECT(vp8enc), "width", 1024, "height", 600, NULL);
    // g_object_set(vp8enc, "min-quantizer", 10, NULL);

    g_object_set(G_OBJECT(capsfilter1), "caps", caps1, NULL, nullptr);
    // g_object_set(G_OBJECT(capsfilter2), "caps", caps2, NULL);
    // g_object_set(videoscale, "add-borders", TRUE, 0x00000000, "width", 1024, "height", 600, NULL);

    gst_caps_unref(caps1);
    //
    // gst_caps_unref(caps2);

    gst_bin_add_many(GST_BIN(data.pipeline), ximagesrc, videoconvert1, videoscale, capsfilter1, videoconvert2, queue1, vp8enc, rtpvp8pay,
                     queue2, udpsink1, NULL, nullptr);

    if (gst_element_link_many(ximagesrc, videoconvert1, videoscale, capsfilter1, videoconvert2, queue1, vp8enc, rtpvp8pay, queue2, udpsink1,
                              NULL, nullptr) != TRUE) {
        g_printerr(
            "Failed to link elements: ximagesrc -> videoscale -> videoconvert -> x264enc -> "
            "rtph264pay -> udpsink1\n");
        gst_object_unref(data.pipeline);
        return;
    }

    //    gst_debug_bin_to_dot_file(GST_BIN(data.pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "pipeline.dot");

    g_object_set(udpsink1, "sync", FALSE, "host", representIP(ip_address), "port", video_port, NULL, nullptr);
    // g_object_set(x264enc, "pass", 17, "tune", 4, "bitrate", 2000, "speed-preset", 0x00000005, NULL);
    g_object_set(G_OBJECT(vp8enc), "deadline", 1, "target-bitrate", 2000000, NULL, nullptr);
}

void Transmiter::addLinkAudio() {
    GstElement *alsasrc, *audioconvert, *audioresample, *opusenc, *rtpopuspay, *udpsink2;
    if (data.pipeline == NULL) {
        data.pipeline = gst_pipeline_new("pipeline");
    }

    alsasrc = gst_element_factory_make("alsasrc", "alsasrc");
    audioconvert = gst_element_factory_make("audioconvert", "audioconvert");
    audioresample = gst_element_factory_make("audioresample", "audioresample");
    opusenc = gst_element_factory_make("opusenc", "opusenc");
    rtpopuspay = gst_element_factory_make("rtpopuspay", "rtpopuspay");
    udpsink2 = gst_element_factory_make("udpsink", "udpsink2");

    if (!data.pipeline || !alsasrc || !audioconvert || !audioresample || !opusenc || !rtpopuspay || !udpsink2) {
        g_printerr("Not all elements could be created\n");
        return;
    }

    gst_bin_add_many(GST_BIN(data.pipeline), alsasrc, audioconvert, audioresample, opusenc, rtpopuspay, udpsink2, NULL, nullptr);

    if (gst_element_link_many(alsasrc, audioconvert, audioresample, opusenc, rtpopuspay, udpsink2, NULL, nullptr) != TRUE) {
        g_printerr(
            "Failed to link elements: ximagesrc -> videoscale -> videoconvert -> x264enc -> "
            "rtph264pay -> udpsink1\n");
        gst_object_unref(data.pipeline);
        return;
    }

    g_object_set(udpsink2, "sync", FALSE, "host", representIP(ip_address), "port", audio_port, NULL, nullptr);
}

void Transmiter::removeVideo() {
    gst_element_set_state(data.pipeline, GST_STATE_PAUSED);
    gst_bin_get_by_name(GST_BIN(data.pipeline), "ximagesrc");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "videoconvert1");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "videoscale");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "capsfilterVideo1");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "videoconvert2");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "queue1");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "vp8enc");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "rtpvp8pay");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "queue2");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "udpsink1");
    gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
}

void Transmiter::removeAudio() {
    gst_element_set_state(data.pipeline, GST_STATE_PAUSED);
    gst_bin_get_by_name(GST_BIN(data.pipeline), "alsasrc");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "audioconvert");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "audioresample");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "opusenc");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "rtpopuspay");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "udpsink2");
    gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
}

void Transmiter::startSend() {
    data.bus = gst_element_get_bus(data.pipeline);
    // qDebug() << data.pipeline;

    gst_bus_add_watch(data.bus, static_cast<GstBusFunc>(busCallback), this);

    gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
    gst_bus_timed_pop_filtered(data.bus, GST_CLOCK_TIME_NONE, static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
}

Transmiter::Transmiter(const QHostAddress& ip_address_, const qint32 video_port_, const qint32 audio_port_)
    : Session(ip_address_, video_port_, audio_port_) {
    qDebug() << "Constructor TransmiterVideo" << this;
}

Transmiter::Transmiter(const QHostAddress& ip_address_, const qint32 audio_port_) : Session(ip_address_, audio_port_) {
    qDebug() << "Constructor TransmiterAudio" << this;
}

Transmiter::~Transmiter() {
    qDebug() << "Transmiter Destructor"
             << "(" << this << ")";
    onKillSession();
    // gst_element_set_state(data.pipeline, GST_STATE_NULL);
}

void Transmiter::onEnableVideo() {
    if (!data.pipeline) {
        gst_init(nullptr, nullptr);
        addLinkVideo();
        startSend();
    } else {
        gst_object_unref(data.bus);
        gst_object_unref(data.loop);
        gst_element_set_state(data.pipeline, GST_STATE_PAUSED);

        addLinkVideo();
        startSend();
    }
}

void Transmiter::onEnableAudio() {
    if (!data.pipeline) {
        gst_init(nullptr, nullptr);
        addLinkAudio();
        startSend();
    } else {
        gst_object_unref(data.bus);
        gst_object_unref(data.loop);
        gst_element_set_state(data.pipeline, GST_STATE_PAUSED);

        addLinkAudio();
        startSend();
    }
}

void Transmiter::onDisableVideo() {
    removeVideo();
}

void Transmiter::onDisableAudio() {
    removeAudio();
}

void Transmiter::onStartSession() {
    /*на данный момент не используемая функция*/
    /*qDebug() << "Starting video transmition";

     gst_init(nullptr, nullptr);
     addLinkVideo();
     addLinkAudio();
     startSend();*/
}

void Transmiter::onKillSession() {
    gst_element_set_state(data.pipeline, GST_STATE_NULL);
    // if (data.loop) g_main_loop_unref(data.loop);
    // if (data.bus) gst_object_unref(data.bus);
    // if (data.pipeline) gst_object_unref(data.pipeline);
}

void Transmiter::onSetVolume(float volume) {}

void Transmiter::onSetBitrate(const int bitrate) {
    qDebug() << "set bitrate";
    gst_element_set_state(data.pipeline, GST_STATE_PAUSED);
    g_object_set(G_OBJECT(vp8enc), "target-bitrate", bitrate, NULL);

    gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
}

// void Transmiter::onSetVolume(float volume) {}
