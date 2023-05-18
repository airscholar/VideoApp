//
// Created by Yusuf Ganiyu on 5/18/23.
//

#include <cstdlib>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <inttypes.h>
}

bool loadFrame(const char *filename, unsigned char **data_out, int *width_out, int *height_out) {

    // open the file using libavformat
    AVFormatContext *pFormatCtx = avformat_alloc_context();

    if (!pFormatCtx) {
        printf("Couldn't create AVFormatContext\n");
        return false;
    }

    if (avformat_open_input(&pFormatCtx, filename, NULL, NULL) != 0) {
        printf("Couldn't open file\n");
        free(pFormatCtx);
        return false;
    }
    int videoStream = -1;
    AVCodecParameters *av_codec_params;
    const AVCodec *av_codec;

    // find the video stream
    for (int i = 0; i < pFormatCtx->nb_streams; ++i) {
        auto stream = pFormatCtx->streams[i];

        // parse the stream with libavcodec
        av_codec_params = stream->codecpar;
        av_codec = avcodec_find_decoder(av_codec_params->codec_id);
        if (!av_codec) {
            continue;
        }

        if (av_codec_params->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            break;
        }
    }

    if (videoStream == -1) {
        printf("Couldn't find video stream\n");
        return false;
    }

    AVCodecContext *av_codec_ctx = avcodec_alloc_context3(av_codec);
    if (!av_codec_ctx) {
        printf("Couldn't create AVCodecContext\n");
        return false;
    }

    if (avcodec_parameters_to_context(av_codec_ctx, av_codec_params) < 0) {
        printf("Couldn't initialize AVCodecContext\n");
        return false;
    }

    if (avcodec_open2(av_codec_ctx, av_codec, NULL) < 0) {
        printf("Couldn't open codec\n");
        return false;
    }

    AVFrame *pFrame = av_frame_alloc();
    if (!pFrame) {
        printf("Couldn't allocate AVFrame\n");
        return false;
    }

    AVPacket *packet = av_packet_alloc();
    if (!packet) {
        printf("Couldn't allocate AVPacket\n");
        return false;
    }

    int response;
    while (av_read_frame(pFormatCtx, packet) >= 0) {
        if (packet->stream_index != videoStream) {
            continue;
        }
        response = avcodec_send_packet(av_codec_ctx, packet);
        if (response < 0) {
            printf("Error while sending a packet to the decoder: %s\n", av_err2str(response));
            return false;
        }

        response = avcodec_receive_frame(av_codec_ctx, pFrame);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
            continue;
        } else if (response < 0) {
            printf("Error while receiving a frame from the decoder: %s\n", av_err2str(response));
            return false;
        }

        av_packet_unref(packet);
        break;
    }

    // save frame data
    unsigned char* data = (unsigned char *) malloc(pFrame->width * pFrame->height * 3);
    for (int x = 0; x < pFrame->width; ++x) {
        for (int y = 0; y < pFrame->height; ++y) {
            data[y * pFrame->width * 3 + x * 3 + 0] = pFrame->data[0][y * pFrame->linesize[0] + x];
            data[y * pFrame->width * 3 + x * 3 + 1] = pFrame->data[0][y * pFrame->linesize[0] + x];
            data[y * pFrame->width * 3 + x * 3 + 2] = pFrame->data[0][y * pFrame->linesize[0] + x];
        }
    }

    *data_out = data;
    *width_out = pFrame->width;
    *height_out = pFrame->height;

    av_frame_free(&pFrame);
    avcodec_free_context(&av_codec_ctx);
    avformat_close_input(&pFormatCtx);
    free(pFormatCtx);
    return true;
}
