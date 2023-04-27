#include <gst/gst.h>
#include <stdio.h>

void startSend();

struct CustomData {
  GstElement *pipeline;
  GstBus *bus;
  GstMessage *msg;
  GMainLoop *loop;
} data;

gboolean on_bus_message (GstBus *bus, GstMessage *message, gpointer user_data)
{
  GError *error = NULL;
  gchar *debug_info = NULL;

  switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_ERROR:
      gst_message_parse_error (message, &error, &debug_info);
      g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (message->src), error->message);
      g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
      g_clear_error (&error);
      g_free (debug_info);
      break;
    case GST_MESSAGE_WARNING:
      gst_message_parse_warning (message, &error, &debug_info);
      g_printerr ("Warning received from element %s: %s\n", GST_OBJECT_NAME (message->src), error->message);
      g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
      g_clear_error (&error);
      g_free (debug_info);
      break;
    default:
      break;
  }

  return TRUE;
}



void addLinkVideo()
{
    GstElement *ximagesrc, *videoscale, *videoconvert, *x264enc, *h264parse, *rtph264pay, *udpsink1, *capsfilter1, *capsfilter2;
    GstCaps *caps1, *caps2;
	
    gst_init(0,nullptr);
    
    if (data.pipeline == NULL){
    	data.pipeline = gst_pipeline_new("pipeline");
    }
    
    ximagesrc = gst_element_factory_make("ximagesrc", "ximagesrc");
    capsfilter1 = gst_element_factory_make("capsfilter", "capsfilter1");
    videoscale = gst_element_factory_make("videoscale", "videoscale");
    videoconvert = gst_element_factory_make("videoconvert", "videoconvert");
    x264enc = gst_element_factory_make("x264enc", "x264enc");
    capsfilter2 = gst_element_factory_make("capsfilter", "capsfilter2");
    h264parse = gst_element_factory_make("h264parse", "h264parse");
    rtph264pay = gst_element_factory_make("rtph264pay", "rtph264pay");
    udpsink1 = gst_element_factory_make("udpsink", "udpsink1");
    
    if (!data.pipeline || !h264parse || !ximagesrc || !capsfilter1 || !capsfilter2 || !videoscale || !videoconvert || !x264enc || !rtph264pay || !udpsink1  ) {
        g_printerr("Not all elements could be created\n");
        return ;
    }
    
     caps1 = gst_caps_new_simple("video/x-raw",
    "framerate", GST_TYPE_FRACTION, 30, 1,
    
    NULL);
    
    caps2 = gst_caps_new_simple("video/x-h264",
    "profile", G_TYPE_STRING, "main",
     "width", G_TYPE_INT, 1024,
    "height", G_TYPE_INT, 600,
    NULL);
    
    g_object_set(G_OBJECT(capsfilter1), "caps", caps1, NULL);
    g_object_set(G_OBJECT(capsfilter2), "caps", caps2, NULL);
    
    gst_caps_unref(caps1);
    gst_caps_unref(caps2);
    
       gst_bin_add_many(GST_BIN(data.pipeline), ximagesrc,capsfilter1,  videoscale, videoconvert, x264enc, capsfilter2 ,h264parse, rtph264pay, udpsink1, NULL);
       
     if (gst_element_link_many(ximagesrc, capsfilter1 , videoscale, videoconvert, x264enc, capsfilter2 , h264parse, rtph264pay, udpsink1, NULL) !=TRUE)  {
    	 g_printerr("Failed to link elements: ximagesrc -> videoscale -> videoconvert -> x264enc -> rtph264pay -> udpsink1\n");
        gst_object_unref(data.pipeline);
        return ;
    }
    
    g_object_set(udpsink1,"sync", FALSE, "host", "127.0.0.1", "port", 5001, NULL); 
    g_object_set(x264enc, "pass", 17,"tune",4,"bitrate",2200,NULL);
}

void addLinkAudio()
{
    GstElement  *alsasrc, *audioconvert, *audioresample, *opusenc, *rtpopuspay, *udpsink2;

    gst_init(0, nullptr);
    if (data.pipeline == NULL){
    	data.pipeline = gst_pipeline_new("pipeline");
    }
    
    alsasrc = gst_element_factory_make("alsasrc", "alsasrc");
    audioconvert = gst_element_factory_make("audioconvert", "audioconvert");
    audioresample = gst_element_factory_make("audioresample", "audioresample");
    opusenc = gst_element_factory_make("opusenc", "opusenc");
    rtpopuspay = gst_element_factory_make("rtpopuspay", "rtpopuspay");
    udpsink2 = gst_element_factory_make("udpsink", "udpsink2");
    
   if ( !data.pipeline || !alsasrc || !audioconvert || !audioresample || !opusenc || !rtpopuspay || !udpsink2 ) {
        g_printerr("Not all elements could be created\n");
        return ;
    }
    
      gst_bin_add_many(GST_BIN(data.pipeline), alsasrc, audioconvert, audioresample, opusenc, rtpopuspay, udpsink2, NULL);
      
     if (gst_element_link_many(alsasrc, audioconvert, audioresample, opusenc, rtpopuspay, udpsink2, NULL) !=TRUE)  {
    	 g_printerr("Failed to link elements: ximagesrc -> videoscale -> videoconvert -> x264enc -> rtph264pay -> udpsink1\n");
        gst_object_unref(data.pipeline);
        return ;
    } 
    
    g_object_set(udpsink2,"sync", FALSE, "host", "127.0.0.1", "port", 5000, NULL);
     
}
void startAudioSession()
{
  addLinkAudio();
  startSend();
	
}

void startVideoSession()
{
  addLinkVideo();
  addLinkAudio();
  startSend();
}

void startSend()
{
    data.bus = gst_element_get_bus(data.pipeline);
    
    gst_bus_add_watch(data.bus, (GstBusFunc) on_bus_message, NULL );
    
    gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
    gst_bus_timed_pop_filtered (data.bus, GST_CLOCK_TIME_NONE, (GstMessageType)(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
}

void killVideoSession()
{
    gst_object_unref(data.bus);
    gst_element_set_state(data.pipeline, GST_STATE_NULL);
    gst_object_unref(data.pipeline);
}

void killAudioSession()
{
    gst_object_unref(data.bus);
    gst_element_set_state(data.pipeline, GST_STATE_NULL);
    gst_object_unref(data.pipeline);
}


int main(int argc, char* argv[])
{
	startVideoSession();
	killVideoSession();
	//startAudioSession();
	//killAudioSession();
	
    return 0;	
}











