#include <gst/gst.h>

gboolean bus_callback(GstBus *bus, GstMessage *msg, gpointer data) {
    GMainLoop *loop = (GMainLoop *) data;

    switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_ERROR: {
            GError *error;
            gchar *debug_info;

            gst_message_parse_error(msg, &error, &debug_info);
            g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src), error->message);
            g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
            g_clear_error(&error);
            g_free(debug_info);
            g_main_loop_quit(loop);
            break;
        }
        case GST_MESSAGE_WARNING: {
            GError *error;
            gchar *debug_info;

            gst_message_parse_warning(msg, &error, &debug_info);
            g_printerr("Warning received from element %s: %s\n", GST_OBJECT_NAME(msg->src), error->message);
            g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
            g_clear_error(&error);
            g_free(debug_info);
            break;
        }
        case GST_MESSAGE_EOS:
            g_print("End-Of-Stream reached.\n");
            g_main_loop_quit(loop);
            break;

        default:
            break;
    }

    return TRUE;
}


int main(int argc, char *argv[]) {
    GstElement *pipeline, *udpsrc1, *udpsrc2, *depay1, *depay2, *parse1, *parse2, *decode1, *decode2, *convert1, *convert2, *autovideosink1, *autovideosink2, *audioresample, *capsfilter1, *capsfilter2, *queue1, *queue2;
    GstBus *bus;
    GstMessage *msg;
    GstCaps *caps1, *caps2;
    GMainLoop *loop;

    gst_init(&argc, &argv);

    pipeline = gst_pipeline_new("pipeline");
    udpsrc1 = gst_element_factory_make("udpsrc", "udpsrc1");
    queue1 = gst_element_factory_make("queue", "buffer1");
    capsfilter1 = gst_element_factory_make("capsfilter", "capsfilter1");
    depay1 = gst_element_factory_make("rtph264depay", "depay1");
    parse1 = gst_element_factory_make("h264parse", "parse1");
    decode1 = gst_element_factory_make("avdec_h264", "decode1");
    convert1 = gst_element_factory_make("videoconvert", "convert1");
    autovideosink1 = gst_element_factory_make("autovideosink", "autovideosink1");
    
    udpsrc2 = gst_element_factory_make("udpsrc", "udpsrc2");
    queue2 = gst_element_factory_make("queue", "buffer2");
    capsfilter2 = gst_element_factory_make("capsfilter", "capsfilter2");
    depay2 = gst_element_factory_make("rtpopusdepay", "depay2");  
    parse2 = gst_element_factory_make("opusparse", "parse2");
    decode2 = gst_element_factory_make("avdec_opus", "decode2"); 
    convert2 = gst_element_factory_make("audioconvert", "convert2");
    audioresample = gst_element_factory_make("audioresample", "audioresample"); 
    autovideosink2 = gst_element_factory_make("autoaudiosink", "autovideosink2");
    
    
    if (!pipeline || !udpsrc1 || !udpsrc2 || !depay1 || !depay2 || !parse1 || !parse2 || !decode1 || !decode2 || !convert1 || !convert2 || !autovideosink1 || !autovideosink2 || !audioresample || !capsfilter2 || !capsfilter1 || !queue1 || !queue2) {
        g_printerr("Not all elements could be created. Exiting.\n");
        return -1;
    }
    
    
    caps1 = gst_caps_new_simple("application/x-rtp",
    "media",G_TYPE_STRING ,"video", 
    "clock-rate",G_TYPE_INT ,90000,
    "encoding-name", G_TYPE_STRING, "H264",
    "payload",G_TYPE_INT ,96,
    NULL);
    
    caps2 = gst_caps_new_simple("application/x-rtp",
    "media",G_TYPE_STRING ,"audio", 
    "clock-rate",G_TYPE_INT ,48000,
    "encoding-name", G_TYPE_STRING, "OPUS",
    "payload",G_TYPE_INT ,96,
    NULL);
    
    g_object_set(G_OBJECT(capsfilter1), "caps", caps1, NULL);
    g_object_set(G_OBJECT(capsfilter2), "caps", caps2, NULL);
    
    gst_caps_unref(caps1);
    gst_caps_unref(caps2);
    

    GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "pipeline_RECEIVE");
    gst_bin_add_many(GST_BIN(pipeline), udpsrc1,queue1 , capsfilter1, depay1, parse1, decode1, convert1, autovideosink1, udpsrc2, queue2, capsfilter2, depay2, parse2, decode2, convert2, audioresample, autovideosink2, NULL);

 GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "pipeline_RECEIVE");

    if (!gst_element_link_many(udpsrc1, queue1 ,capsfilter1 , depay1, parse1, decode1, convert1, autovideosink1, NULL) ||
        !gst_element_link_many(udpsrc2, queue2, capsfilter2 , depay2, parse2, decode2, convert2, audioresample, autovideosink2, NULL)) {
        g_printerr("Could not link all elements. Exiting.\n");
        return -1;
    }
    
    g_object_set(udpsrc1, "port", 5001, NULL);
    g_object_set(udpsrc2, "port", 5000, NULL);

    GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "pipeline_RECEIVE");
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    loop = g_main_loop_new(NULL, FALSE);

    bus = gst_element_get_bus(pipeline);
    
    gst_bus_add_watch(bus, (GstBusFunc)bus_callback, loop);
    
    gst_object_unref(bus);

    g_main_loop_run(loop);

    g_main_loop_unref(loop);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    return 0;
}












