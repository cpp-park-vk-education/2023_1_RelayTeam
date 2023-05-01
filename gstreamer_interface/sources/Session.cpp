#include <Session.h>

#include "TransmiterVideo.h"

Session::Session(const QHostAddress& local_ip6_, const qint16 video_port_, const qint16 audio_port_)
	: local_ip6(local_ip6_), video_port(video_port_), audio_port(audio_port_) {}

gboolean Session::busCallback(GstBus* bus, GstMessage* msg, gpointer data) {
	GMainLoop* loop = (GMainLoop*)data;

	switch (GST_MESSAGE_TYPE(msg)) {
		case GST_MESSAGE_ERROR: {
			GError* error;
			gchar* debug_info;

			gst_message_parse_error(msg, &error, &debug_info);
			g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src), error->message);
			g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
			g_clear_error(&error);
			g_free(debug_info);
			g_main_loop_quit(loop);
			break;
		}
		case GST_MESSAGE_WARNING: {
			GError* error;
			gchar* debug_info;

			gst_message_parse_warning(msg, &error, &debug_info);
			g_printerr("Warning received from element %s: %s\n", GST_OBJECT_NAME(msg->src), error->message);
			g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
			g_clear_error(&error);
			g_free(debug_info);
			break;
		}
		case GST_MESSAGE_EOS:
			g_print("End-Of-Stream reached.\n");
			// g_main_loop_quit(loop);

			break;

		default:
			qDebug() << "1";
			break;
	}
	return TRUE;
}
