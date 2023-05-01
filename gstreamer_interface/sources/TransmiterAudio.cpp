#include "TransmiterAudio.h"

#include <QDebug>

TransmiterAudio::TransmiterAudio(const QHostAddress& local_ip6_, const qint16 audio_port_) : Session(local_ip6_, -1, audio_port_) {}

TransmiterAudio::~TransmiterAudio() {
    // gst_object_unref(data.bus);
    // gst_element_set_state(data.pipeline, GST_STATE_NULL);
    // gst_object_unref(data.pipeline);
}

void TransmiterAudio::addLinkAudio() {
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

	gst_bin_add_many(GST_BIN(data.pipeline), alsasrc, audioconvert, audioresample, opusenc, rtpopuspay, udpsink2, NULL);

	if (gst_element_link_many(alsasrc, audioconvert, audioresample, opusenc, rtpopuspay, udpsink2, NULL) != TRUE) {
		g_printerr(
			"Failed to link elements: ximagesrc -> videoscale -> videoconvert -> x264enc -> "
			"rtph264pay -> udpsink1\n");
        gst_object_unref(data.pipeline);
        return;
    }

	g_object_set(udpsink2, "sync", FALSE, "host", local_ip6.toString().toLocal8Bit().constData(), "port", audio_port, NULL);
}

void TransmiterAudio::onStartSession() {
	qDebug() << "Starting audio transmition";
	gst_init(0, nullptr);
    addLinkAudio();
    startSend();
}

void TransmiterAudio::startSend() {
    data.bus = gst_element_get_bus(data.pipeline);
    qDebug() << data.pipeline;

    gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
	gst_bus_timed_pop_filtered(data.bus, GST_CLOCK_TIME_NONE, static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
}

void TransmiterAudio::onKillSession() {
    gst_object_unref(data.bus);
    gst_element_set_state(data.pipeline, GST_STATE_NULL);
    gst_object_unref(data.pipeline);
}
