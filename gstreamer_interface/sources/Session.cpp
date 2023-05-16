#include <Session.h>

Session::Session(const QHostAddress& ip_address_, const qint32 video_port_, const qint32 audio_port_)
    : ip_address(ip_address_), video_port(video_port_), audio_port(audio_port_) {
}

Session::Session(const QHostAddress& ip_address_, const qint32 audio_port_) : ip_address(ip_address_), audio_port(audio_port_) {
}

Session::Session(const qint32 video_port_, const qint32 audio_port_) : video_port(video_port_), audio_port(audio_port_) {
}

Session::Session(const qint32 audio_port_) : audio_port(audio_port_) {
}

Session::~Session() {
}

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

            if (strcmp(error->message, "Output widget was destroyed") == 0) {
                Session* session = static_cast<Session*>(data);
                emit session->onKillSession();
                delete session;
            }

            g_printerr("Error from element %s: %s\n", GST_OBJECT_NAME(msg->src), error->message);
            g_printerr("Debugging info: %s\n", debug_info ? debug_info : "none");
            g_clear_error(&error);
            g_free(debug_info);
            break;
        }
        case GST_MESSAGE_EOS:
            g_print("End-Of-Stream reached.\n");

            break;

        default:
            qDebug() << "1";
            break;
    }
    return TRUE;
}

const char* Session::representIP(const QHostAddress& ext_ip_address) {
	char* ip_char_string = nullptr;
	if (ext_ip_address.protocol() == QAbstractSocket::IPv4Protocol) {
		ip_char_string = new char[ext_ip_address.toString().size()];
		strcpy(ip_char_string, ext_ip_address.toString().toStdString().c_str());
	}
	if (ext_ip_address.protocol() == QAbstractSocket::IPv6Protocol) {  // needs processing
		ip_char_string = new char[ext_ip_address.toString().size()];
		strcpy(ip_char_string, ext_ip_address.toString().toStdString().c_str());
	}
	return ip_char_string;
}
