#include "session_manager.h"

void start();

gboolean SessionManager::on_bus_message(GstBus *bus, GstMessage *message) {
    GError *error = NULL;
    gchar *debug_info = NULL;

    switch (GST_MESSAGE_TYPE(message)) {
    case GST_MESSAGE_ERROR:
        gst_message_parse_error(message, &error, &debug_info);
        g_printerr("Error received from element %s: %s\n",
                   GST_OBJECT_NAME(message->src), error->message);
        g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
        g_clear_error(&error);
        g_free(debug_info);
        break;
    case GST_MESSAGE_WARNING:
        gst_message_parse_warning(message, &error, &debug_info);
        g_printerr("Warning received from element %s: %s\n",
                   GST_OBJECT_NAME(message->src), error->message);
        g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
        g_clear_error(&error);
        g_free(debug_info);
        break;
    default:
        break;
    }

    return TRUE;
}

void SessionManager::addLinkVideo(const QString& local_ip) {
    GstElement *alsasrc, *audioconvert, *audioresample, *opusenc, *rtpopuspay,
        *udpsink2;

    gst_init(0, nullptr);
    if (data.pipeline == NULL) {
        data.pipeline = gst_pipeline_new("pipeline");
    }

    alsasrc = gst_element_factory_make("alsasrc", "alsasrc");
    audioconvert = gst_element_factory_make("audioconvert", "audioconvert");
    audioresample = gst_element_factory_make("audioresample", "audioresample");
    opusenc = gst_element_factory_make("opusenc", "opusenc");
    rtpopuspay = gst_element_factory_make("rtpopuspay", "rtpopuspay");
    udpsink2 = gst_element_factory_make("udpsink", "udpsink2");

    if (!data.pipeline || !alsasrc || !audioconvert || !audioresample ||
        !opusenc || !rtpopuspay || !udpsink2) {
        g_printerr("Not all elements could be created\n");
        return;
    }

    gst_bin_add_many(GST_BIN(data.pipeline), alsasrc, audioconvert, audioresample,
                     opusenc, rtpopuspay, udpsink2, NULL);

    if (gst_element_link_many(alsasrc, audioconvert, audioresample, opusenc,
                              rtpopuspay, udpsink2, NULL) != TRUE) {
        g_printerr("Failed to link elements: ximagesrc -> videoscale -> "
                   "videoconvert -> x264enc -> rtph264pay -> udpsink1\n");
        gst_object_unref(data.pipeline);
        return;
    }

    g_object_set(udpsink2, "sync", FALSE, "host", "127.0.0.1", "port", 5000,
                 NULL);
}

void SessionManager::addLinkAudio(const QString& local_ip) {
    GstElement *alsasrc, *audioconvert, *audioresample, *opusenc, *rtpopuspay,
        *udpsink2;

    gst_init(0, nullptr);
    if (data.pipeline == NULL) {
        data.pipeline = gst_pipeline_new("pipeline");
    }

    alsasrc = gst_element_factory_make("alsasrc", "alsasrc");
    audioconvert = gst_element_factory_make("audioconvert", "audioconvert");
    audioresample = gst_element_factory_make("audioresample", "audioresample");
    opusenc = gst_element_factory_make("opusenc", "opusenc");
    rtpopuspay = gst_element_factory_make("rtpopuspay", "rtpopuspay");
    udpsink2 = gst_element_factory_make("udpsink", "udpsink2");

    if (!data.pipeline || !alsasrc || !audioconvert || !audioresample ||
        !opusenc || !rtpopuspay || !udpsink2) {
        g_printerr("Not all elements could be created\n");
        return;
    }

    gst_bin_add_many(GST_BIN(data.pipeline), alsasrc, audioconvert, audioresample,
                     opusenc, rtpopuspay, udpsink2, NULL);

    if (gst_element_link_many(alsasrc, audioconvert, audioresample, opusenc,
                              rtpopuspay, udpsink2, NULL) != TRUE) {
        g_printerr("Failed to link elements: ximagesrc -> videoscale -> "
                   "videoconvert -> x264enc -> rtph264pay -> udpsink1\n");
        gst_object_unref(data.pipeline);
        return;
    }

    g_object_set(udpsink2, "sync", FALSE, "host", "127.0.0.1", "port", 5000, NULL);
}

void SessionManager::startVideoSession(const QString& local_ip) {
    addLinkVideo(local_ip);
    addLinkAudio(local_ip);
    startSend();
}

void SessionManager::startAudioSession(const QString& local_ip) {
    addLinkAudio(local_ip);
    startSend();
}

void SessionManager::killVideoSession(const QString& local_ip)
{
    gst_object_unref(data.bus);
    gst_element_set_state(data.pipeline, GST_STATE_NULL);
    gst_object_unref(data.pipeline);
}

void SessionManager::killAudioSession(const QString& local_ip)
{
    gst_object_unref(data.bus);
    gst_element_set_state(data.pipeline, GST_STATE_NULL);
    gst_object_unref(data.pipeline);
}

void SessionManager::startSend() {
    data.bus = gst_element_get_bus(data.pipeline);

    gst_bus_add_watch(data.bus, (GstBusFunc)SessionManager::on_bus_message(data.bus,data.msg), NULL);

    gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
    gst_bus_timed_pop_filtered(
        data.bus, GST_CLOCK_TIME_NONE,
        (GstMessageType)(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
}

