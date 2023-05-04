#include <gtest/gtest.h>

#include "TransmiterVideo.h"

TEST(CustomDataTest, PipelineExists) {
    QHostAddress address("127.0.0.1");
    qint32 video_port = 5000;
    qint32 audio_port = 5001;
    TransmiterVideo videoTest(address, video_port, audio_port);

    ASSERT_TRUE(videoTest.isPipelineCreated());
}
