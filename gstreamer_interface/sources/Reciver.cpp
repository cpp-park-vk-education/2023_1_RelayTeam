#include "Reciver.h"

void Reciver::addLinkVideo() {
    GstElement *pipeline, *capsfilter1, *capsfilter2, *udpsrc, *queue1, *vp8dec, *rtpvp8depay, *videoconvert, *queue2, *autovideosink;
    GstCaps *caps, *caps2;
    if (data.pipeline == NULL) {
        data.pipeline = gst_pipeline_new("pipeline");
    }

    udpsrc = gst_element_factory_make("udpsrc", "udpsrc");
    queue1 = gst_element_factory_make("queue", "queue1");
    capsfilter1 = gst_element_factory_make("capsfilter", "capsfilterVideo1");
    rtpvp8depay = gst_element_factory_make("rtpvp8depay", "rtpvp8depay");
    vp8dec = gst_element_factory_make("vp8dec", "vp8dec");
    videoconvert = gst_element_factory_make("videoconvert", "videoconvert");
    // capsfilter2 = gst_element_factory_make("capsfilter", "capsfilterVideo2");
    queue2 = gst_element_factory_make("queue", "queue2");
    autovideosink = gst_element_factory_make("autovideosink", "videosink");

    if (!data.pipeline || !udpsrc || !rtpvp8depay || !vp8dec || !videoconvert || !autovideosink || !queue1 || !queue2 || !capsfilter1) {
        g_printerr("Not all elements could be created. Exiting.\n");
        return;
    }

    caps = gst_caps_new_simple("application/x-rtp", "media", G_TYPE_STRING, "video", "clock-rate", G_TYPE_INT, 90000, "encoding-name",
                               G_TYPE_STRING, "VP8", "payload", G_TYPE_INT, 96, NULL);

    caps2 = gst_caps_new_simple("video/x-raw", "width", G_TYPE_INT, 1024, "height", G_TYPE_INT, 600, NULL);

    g_object_set(G_OBJECT(capsfilter1), "caps", caps, NULL);
    // g_object_set(G_OBJECT(capsfilter2), "caps", caps2, NULL);
    gst_caps_unref(caps);
    // gst_caps_unref(caps2);

    gst_bin_add_many(GST_BIN(data.pipeline), udpsrc, queue1, capsfilter1, rtpvp8depay, vp8dec, videoconvert, queue2, autovideosink, NULL);

    if (!gst_element_link_many(udpsrc, queue1, capsfilter1, rtpvp8depay, vp8dec, videoconvert, queue2, autovideosink, NULL)) {
        g_printerr("Could not link all elements. Exiting.\n");
        return;
    }

    g_object_set(udpsrc, "address", representIP(ip_address), "port", video_port, NULL);
}

void Reciver::addLinkAudio() {
    GstElement *udpsrc, *depay, *parse, *decode, *convert, *autovideosink, *audioresample, *capsfilter, *queue;
    GstCaps* caps;
    if (data.pipeline == NULL) {
        data.pipeline = gst_pipeline_new("pipeline");
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

    if (!data.pipeline || !udpsrc || !depay || !parse || !decode || !convert || !autovideosink || !audioresample || !capsfilter || !queue ||
        !volume) {
        g_printerr("Not all elements could be created. Exiting.\n");
        return;
    }

    caps = gst_caps_new_simple("application/x-rtp", "media", G_TYPE_STRING, "audio", "clock-rate", G_TYPE_INT, 48000, "encoding-name",
                               G_TYPE_STRING, "OPUS", "payload", G_TYPE_INT, 96, NULL);

    g_object_set(G_OBJECT(capsfilter), "caps", caps, NULL);

    gst_caps_unref(caps);
    // g_object_set(volume, "volume", 0.5, NULL);

    gst_bin_add_many(GST_BIN(data.pipeline), udpsrc, queue, capsfilter, depay, parse, decode, convert, audioresample, volume, autovideosink,
                     NULL);

    if (!gst_element_link_many(udpsrc, queue, capsfilter, depay, parse, decode, convert, audioresample, volume, autovideosink, NULL)) {
        g_printerr("Could not link all elements. Exiting.\n");
        return;
    }

    g_object_set(udpsrc, "address", representIP(ip_address), "port", audio_port, NULL);
}

void Reciver::removeVideo() {
    gst_element_set_state(data.pipeline, GST_STATE_PAUSED);
    gst_bin_get_by_name(GST_BIN(data.pipeline), "udpsrcVideo");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "queue1");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "capsfilterVideo1");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "rtpvp8depay");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "vp8dec");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "videoconvert");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "queue2");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "videosink");
    gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
}

void Reciver::removeAudio() {
    gst_element_set_state(data.pipeline, GST_STATE_PAUSED);
    gst_bin_get_by_name(GST_BIN(data.pipeline), "udpsrcAudio");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "bufferAudio");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "capsfilterAudio");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "depayAudio");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "parseAudio");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "decodeAudio");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "convertAudio");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "audioresampleAudio");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "volumeAudio");
    gst_bin_get_by_name(GST_BIN(data.pipeline), "autovideosinkAudio");
    gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
}

void Reciver::startReceive() {
    gst_element_set_state(data.pipeline, GST_STATE_NULL);
    // gst_element_set_state(data.pipeline, GST_STATE_READY);
    gst_element_set_state(data.pipeline, GST_STATE_PLAYING);

    data.loop = g_main_loop_new(NULL, FALSE);

    data.bus = gst_element_get_bus(data.pipeline);

    gst_bus_add_watch(data.bus, static_cast<GstBusFunc>(busCallback), this);

    gst_object_unref(data.bus);

    g_main_loop_run(data.loop);
}

Reciver::Reciver(const qint16 video_port, const qint16 audio_port) : Session(video_port, audio_port) {
    qDebug() << "constr Reciver VIDEO" << this;
}

Reciver::Reciver(const qint16 audio_port) : Session(audio_port) {
    qDebug() << "constr Reciver AUDIO" << this;
}

Reciver::~Reciver() {
    qDebug() << "Destruct Receiver"
             << "(" << this << ")";
    onKillSession();
}

void Reciver::onSetVolume(float volume_) {
    g_object_set(volume, "volume", volume_, NULL);
}

void Reciver::onSetBitrate(const int bitrate) {}

void Reciver::onEnableVideo() {
    if (!data.pipeline) {
        gst_init(nullptr, nullptr);
        addLinkVideo();
        startReceive();
    } else {
        gst_object_unref(data.bus);
        gst_object_unref(data.loop);
        gst_element_set_state(data.pipeline, GST_STATE_PAUSED);

        addLinkVideo();
        startReceive();
    }
}

void Reciver::onEnableAudio() {
    if (!data.pipeline) {
        gst_init(nullptr, nullptr);
        addLinkAudio();
        startReceive();
    } else {
        gst_object_unref(data.bus);
        gst_object_unref(data.loop);
        gst_element_set_state(data.pipeline, GST_STATE_PAUSED);

        addLinkAudio();
        startReceive();
    }
}

void Reciver::onDisableVideo() {
    removeVideo();
}

void Reciver::onDisableAudio() {
    removeAudio();
}

void Reciver::onStartSession() {
    /*на данный момент ен используется*/
    /* qDebug() << "Starting video receiver";
     gst_init(nullptr, nullptr);
     addLinkVideo();
     addLinkAudio();
     startReceive();*/
}

void Reciver::onKillSession() {
    removeVideo();
    gst_element_set_state(data.pipeline, GST_STATE_NULL);
    // if (data.loop) g_main_loop_unref(data.loop);
    // if (data.bus) gst_object_unref(data.bus);
    // if (data.pipeline) gst_object_unref(data.pipeline);
}
