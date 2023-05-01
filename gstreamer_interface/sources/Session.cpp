#include <Session.h>

#include "TransmiterVideo.h"

Session::Session(const QHostAddress local_ip6_, const qint16 video_port_, const qint16 audio_port_)
	: local_ip6(local_ip6_), video_port(video_port_), audio_port(audio_port_) {}

Session::Session(const QHostAddress local_ip6_, const qint16 audio_port_) : local_ip6(local_ip6_), audio_port(audio_port_) {}

Session::Session(const qint16 video_port_, const qint16 audio_port_) : video_port(video_port_), audio_port(audio_port_) {}

Session::Session(const qint16 audio_port_) : audio_port(audio_port_) {}
