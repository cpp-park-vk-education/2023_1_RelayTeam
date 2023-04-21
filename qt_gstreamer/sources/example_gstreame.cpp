#include "example_gstreamer.h"

//static GMainLoop *loop;

static gboolean on_message(GstBus *bus, GstMessage *message, gpointer user_data) {
    switch (GST_MESSAGE_TYPE(message)) {
    case GST_MESSAGE_EOS:
        g_print("End of stream\n");
        gst_element_set_state(GST_ELEMENT(user_data), GST_STATE_NULL);
        //g_main_loop_quit(loop);
        break;

    case GST_MESSAGE_ERROR: {
        gchar *debug;
        GError *error;
        gst_message_parse_error(message, &error, &debug);
        g_free(debug);
        g_printerr("Error: %s\n", error->message);
        g_error_free(error);
        gst_element_set_state(GST_ELEMENT(user_data), GST_STATE_NULL);
        //g_main_loop_quit(loop);
        break;
    }

    default:
        break;
    }
    printf("lol");
    return TRUE;
}


int srcincast(int argc ,char** argv) {

    gst_init(&argc, &argv);


    GstElement *screen_source = gst_element_factory_make("ximagesrc", "screen");
    GstElement *video_convert = gst_element_factory_make("videoconvert", "video-convert");
    GstElement *video_sink = gst_element_factory_make("ximagesink", "video-sink");


    GstElement *pipeline1 = gst_pipeline_new("screen-capture-pipeline");
    gst_bin_add_many(GST_BIN(pipeline1), screen_source, video_convert, video_sink, NULL);
    gst_element_link_many(screen_source, video_convert, video_sink, NULL);

    if (!pipeline1 || !screen_source || !video_sink || !video_convert) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }



    g_object_set(G_OBJECT(screen_source), "use-damage", 0, "show-pointer", 1, NULL);


    gst_element_set_state(pipeline1, GST_STATE_PLAYING);

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);


    GstBus *bus = gst_element_get_bus(pipeline1);

    //gst_bus_set_sync_handler(bus, (GstBusSyncHandler)on_message, pipeline1, NULL);
   GstMessage *msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, (GstMessageType)(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
    if (msg != NULL) {
        gst_message_unref(msg);
    }

    gst_element_set_state(pipeline1, GST_STATE_NULL);

    gst_object_unref(GST_OBJECT(pipeline1));
    gst_object_unref(GST_OBJECT(bus));

    return 0;
}
