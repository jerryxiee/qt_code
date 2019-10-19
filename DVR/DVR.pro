#-------------------------------------------------
#
# Project created by QtCreator 2019-05-15T16:53:27
#
#-------------------------------------------------

QT       += core gui quickwidgets multimedia network multimediawidgets concurrent sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DVR
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

#greaterThan(QT_VERSION, 5.9):DEFINES += LUNUX_WIN

contains(QT_ARCH, x86_64) {
        message("x86_64")
        DEFINES += LUNUX_WIN
    } else {

    }

SOURCES += \
        main.cpp \
        widget.cpp \
    windows/CustomControls/animationbutton.cpp \
    windows/MyModel/mytablemodel.cpp \
    video/videocontrol.cpp \
    settings/settings.cpp \
    settings/vencset.cpp \
    video/yuvframeprovider.cpp \
    windows/mainwindow.cpp \
    windows/videodisplay.cpp \
    windows/regionmanage.cpp \
    settings/dispset.cpp \
    settings/config.cpp \
    settings/systemset.cpp \
    settings/netconfig.cpp \
    test/test.cpp \
    settings/alarmset.cpp \
    windows/playcontrols.cpp \
    common/systemlog.cpp \
    video/videofile.cpp \
    video/videosearch.cpp \
    windows/videoplaytest.cpp \
    video/mp4fileindex.cpp \
    video/mp4filetab.cpp \
    common/sqlitedatebase.cpp \
    common/logtabdata.cpp \
    common/msgqueue.cpp \
    common/schedule/BasicTaskScheduler.cpp \
    common/schedule/BasicTaskScheduler0.cpp \
    common/schedule/BasicUsageEnvironment.cpp \
    common/schedule/BasicUsageEnvironment0.cpp \
    common/schedule/DelayQueue.cpp \
    common/schedule/strDup.cpp \
    common/schedule/UsageEnvironment.cpp \
    communication/remotethread.cpp \
    streamserver/streamservermanage.cpp \
    settings/config/deviceparamconfig.cpp \
    settings/platformset.cpp \
    settings/config/platformconfigini.cpp \
    modules/platformregister.cpp \
    windows/MyModel/platformfunmodel.cpp \
    test/remotetest.cpp \
    modules/moduleinit.cpp \
    modules/positionctr.cpp \
    modules/positionreport.cpp \
    common/bcdtransform.cpp \
    video/encodetaskscheduler.cpp \
    HW/video/hiframesource.cpp \
    HW/video/hiframeconsumer.cpp \
    HW/video/hivideobase.cpp \
    HW/video/viframesource.cpp \
    HW/video/hivpsssource.cpp \
    HW/video/hivpssconsumer.cpp \
    HW/video/hivencconsumer.cpp \
    HW/video/hivoconsumer.cpp \
    HW/video/hivdaconsumer.cpp \
    HW/video/hiconsumertosource.cpp \
    HW/video/hivenctomp4.cpp







HEADERS += \
        widget.h \
    windows/CustomControls/animationbutton.h \
    windows/MyModel/mytablemodel.h \
    video/videocontrol.h \
    settings/settings.h \
    settings/vencset.h \
    video/yuvframeprovider.h \
    windows/mainwindow.h \
    windows/videodisplay.h \
    windows/regionmanage.h \
    settings/dispset.h \
    settings/config.h \
    settings/systemset.h \
    settings/netconfig.h \
    test/test.h \
    settings/alarmset.h \
    windows/playcontrols.h \
    common/systemlog.h \
    video/videofile.h \
    video/videosearch.h \
    windows/videoplaytest.h \
    video/mp4fileindex.h \
    video/mp4filetab.h \
    common/sqlitedatebase.h \
    common/logtabdata.h \
    common/msgqueue.h \
    common/schedule/BasicUsageEnvironment.hh \
    common/schedule/BasicUsageEnvironment0.hh \
    common/schedule/BasicUsageEnvironment_version.hh \
    common/schedule/Boolean.hh \
    common/schedule/DelayQueue.hh \
    common/schedule/HandlerSet.hh \
    common/schedule/NetCommon.h \
    common/schedule/strDup.hh \
    common/schedule/UsageEnvironment.hh \
    common/schedule/UsageEnvironment_version.hh \
    communication/remotethread.h \
    streamserver/streamservermanage.h \
    communication/jtt808define.h \
    settings/config/deviceparamconfig.h \
    settings/platformset.h \
    settings/config/platformconfigini.h \
    modules/platformregister.h \
    windows/MyModel/platformfunmodel.h \
    test/remotetest.h \
    modules/moduleinit.h \
    modules/positionctr.h \
    modules/positionreport.h \
    common/bcdtransform.h \
    video/encodetaskscheduler.h \
    HW/video/hiframesource.h \
    HW/video/hiframeconsumer.h \
    HW/video/viframesource.h \
    HW/video/hivpsssource.h \
    HW/video/hivpssconsumer.h \
    HW/video/hivencconsumer.h \
    HW/video/hivoconsumer.h \
    HW/video/hivdaconsumer.h \
    HW/video/hivideobase.h \
    HW/video/hiconsumertosource.h \
    HW/video/hivenctomp4.h


INCLUDEPATH +=/home/abhw/git/qt_code/DVR/common/schedule \
                /home/abhw/haisi/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/include \
                /home/abhw/haisi/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/extdrv/nvp6134_ex \
                /home/abhw/haisi/software/board/Hi3531DV100_SDK_V2.0.4.0/mpp/extdrv/tlv320aic31 \
                /home/abhw/nfsroot/lib/install/include \
                /home/abhw/git/qt_code/DVR/live555/BasicUsageEnvironment/include \
                /home/abhw/git/qt_code/DVR/live555/groupsock/include \
                /home/abhw/git/qt_code/DVR/live555/liveMedia/include \
                /home/abhw/git/qt_code/DVR/live555/UsageEnvironment/include \
                /home/abhw/libdir/ffmpeg-hisi/include


if(contains(DEFINES,LUNUX_WIN)){
        message("linux platform")
}else{

INCLUDEPATH +=/opt/hisi-linux/x86-arm/arm-hisiv600-linux/target/usr/include


SOURCES +=common/sample_common_sys.cpp \
            common/loadbmp.c \
            common/sample_common_vpss.cpp \
            common/sample_common_vdec.cpp \
#            common/sample_common_vio.cpp \
            common/sample_common_venc.cpp \
            common/sample_common_vi.cpp \
            common/sample_common_vo.cpp \
            live555/BasicUsageEnvironment/BasicHashTable.cpp \
            live555/groupsock/GroupEId.cpp \
            live555/groupsock/Groupsock.cpp \
            live555/groupsock/GroupsockHelper.cpp \
            live555/groupsock/IOHandlers.cpp \
            live555/groupsock/NetAddress.cpp \
            live555/groupsock/NetInterface.cpp \
            live555/liveMedia/AC3AudioFileServerMediaSubsession.cpp \
            live555/liveMedia/AC3AudioRTPSink.cpp \
            live555/liveMedia/AC3AudioRTPSource.cpp \
            live555/liveMedia/AC3AudioStreamFramer.cpp \
            live555/liveMedia/ADTSAudioFileServerMediaSubsession.cpp \
            live555/liveMedia/ADTSAudioFileSource.cpp \
            live555/liveMedia/AMRAudioFileServerMediaSubsession.cpp \
            live555/liveMedia/AMRAudioFileSink.cpp \
            live555/liveMedia/AMRAudioFileSource.cpp \
            live555/liveMedia/AMRAudioRTPSink.cpp \
            live555/liveMedia/AMRAudioRTPSource.cpp \
            live555/liveMedia/AMRAudioSource.cpp \
            live555/liveMedia/AudioInputDevice.cpp \
            live555/liveMedia/AudioRTPSink.cpp \
            live555/liveMedia/AVIFileSink.cpp \
            live555/liveMedia/Base64.cpp \
            live555/liveMedia/BasicUDPSink.cpp \
            live555/liveMedia/BasicUDPSource.cpp \
            live555/liveMedia/BitVector.cpp \
            live555/liveMedia/ByteStreamFileSource.cpp \
            live555/liveMedia/ByteStreamMemoryBufferSource.cpp \
            live555/liveMedia/ByteStreamMultiFileSource.cpp \
            live555/liveMedia/DeviceSource.cpp \
            live555/liveMedia/DigestAuthentication.cpp \
            live555/liveMedia/DVVideoFileServerMediaSubsession.cpp \
            live555/liveMedia/DVVideoRTPSink.cpp \
            live555/liveMedia/DVVideoRTPSource.cpp \
            live555/liveMedia/DVVideoStreamFramer.cpp \
            live555/liveMedia/EBMLNumber.cpp \
            live555/liveMedia/FileServerMediaSubsession.cpp \
            live555/liveMedia/FileSink.cpp \
            live555/liveMedia/FramedFileSource.cpp \
            live555/liveMedia/FramedFilter.cpp \
            live555/liveMedia/FramedSource.cpp \
            live555/liveMedia/GenericMediaServer.cpp \
            live555/liveMedia/GSMAudioRTPSink.cpp \
            live555/liveMedia/H261VideoRTPSource.cpp \
            live555/liveMedia/H263plusVideoFileServerMediaSubsession.cpp \
            live555/liveMedia/H263plusVideoRTPSink.cpp \
            live555/liveMedia/H263plusVideoRTPSource.cpp \
            live555/liveMedia/H263plusVideoStreamFramer.cpp \
            live555/liveMedia/H263plusVideoStreamParser.cpp \
            live555/liveMedia/H264or5VideoFileSink.cpp \
            live555/liveMedia/H264or5VideoRTPSink.cpp \
            live555/liveMedia/H264or5VideoStreamDiscreteFramer.cpp \
            live555/liveMedia/H264or5VideoStreamFramer.cpp \
            live555/liveMedia/H264VideoFileServerMediaSubsession.cpp \
            live555/liveMedia/H264VideoFileSink.cpp \
            live555/liveMedia/H264VideoRTPSink.cpp \
            live555/liveMedia/H264VideoRTPSource.cpp \
            live555/liveMedia/H264VideoStreamDiscreteFramer.cpp \
            live555/liveMedia/H264VideoStreamFramer.cpp \
            live555/liveMedia/H265VideoFileServerMediaSubsession.cpp \
            live555/liveMedia/H265VideoFileSink.cpp \
            live555/liveMedia/H265VideoRTPSink.cpp \
            live555/liveMedia/H265VideoRTPSource.cpp \
            live555/liveMedia/H265VideoStreamDiscreteFramer.cpp \
            live555/liveMedia/H265VideoStreamFramer.cpp \
            live555/liveMedia/InputFile.cpp \
            live555/liveMedia/JPEG2000VideoRTPSink.cpp \
            live555/liveMedia/JPEG2000VideoRTPSource.cpp \
            live555/liveMedia/JPEGVideoRTPSink.cpp \
            live555/liveMedia/JPEGVideoRTPSource.cpp \
            live555/liveMedia/JPEGVideoSource.cpp \
            live555/liveMedia/Locale.cpp \
            live555/liveMedia/MatroskaDemuxedTrack.cpp \
            live555/liveMedia/MatroskaFile.cpp \
            live555/liveMedia/MatroskaFileParser.cpp \
            live555/liveMedia/MatroskaFileServerDemux.cpp \
            live555/liveMedia/MatroskaFileServerMediaSubsession.cpp \
            live555/liveMedia/Media.cpp \
            live555/liveMedia/MediaSession.cpp \
            live555/liveMedia/MediaSink.cpp \
            live555/liveMedia/MediaSource.cpp \
            live555/liveMedia/MP3ADU.cpp \
            live555/liveMedia/MP3ADUdescriptor.cpp \
            live555/liveMedia/MP3ADUinterleaving.cpp \
            live555/liveMedia/MP3ADURTPSink.cpp \
            live555/liveMedia/MP3ADURTPSource.cpp \
            live555/liveMedia/MP3ADUTranscoder.cpp \
            live555/liveMedia/MP3AudioFileServerMediaSubsession.cpp \
            live555/liveMedia/MP3AudioMatroskaFileServerMediaSubsession.cpp \
            live555/liveMedia/MP3FileSource.cpp \
            live555/liveMedia/MP3Internals.cpp \
            live555/liveMedia/MP3InternalsHuffman.cpp \
            live555/liveMedia/MP3InternalsHuffmanTable.cpp \
            live555/liveMedia/MP3StreamState.cpp \
            live555/liveMedia/MP3Transcoder.cpp \
            live555/liveMedia/MPEG1or2AudioRTPSink.cpp \
            live555/liveMedia/MPEG1or2AudioRTPSource.cpp \
            live555/liveMedia/MPEG1or2AudioStreamFramer.cpp \
            live555/liveMedia/MPEG1or2Demux.cpp \
            live555/liveMedia/MPEG1or2DemuxedElementaryStream.cpp \
            live555/liveMedia/MPEG1or2DemuxedServerMediaSubsession.cpp \
            live555/liveMedia/MPEG1or2FileServerDemux.cpp \
            live555/liveMedia/MPEG1or2VideoFileServerMediaSubsession.cpp \
            live555/liveMedia/MPEG1or2VideoRTPSink.cpp \
            live555/liveMedia/MPEG1or2VideoRTPSource.cpp \
            live555/liveMedia/MPEG1or2VideoStreamDiscreteFramer.cpp \
            live555/liveMedia/MPEG1or2VideoStreamFramer.cpp \
            live555/liveMedia/MPEG2IndexFromTransportStream.cpp \
            live555/liveMedia/MPEG2TransportFileServerMediaSubsession.cpp \
            live555/liveMedia/MPEG2TransportStreamAccumulator.cpp \
            live555/liveMedia/MPEG2TransportStreamFramer.cpp \
            live555/liveMedia/MPEG2TransportStreamFromESSource.cpp \
            live555/liveMedia/MPEG2TransportStreamFromPESSource.cpp \
            live555/liveMedia/MPEG2TransportStreamIndexFile.cpp \
            live555/liveMedia/MPEG2TransportStreamMultiplexor.cpp \
            live555/liveMedia/MPEG2TransportStreamTrickModeFilter.cpp \
            live555/liveMedia/MPEG2TransportUDPServerMediaSubsession.cpp \
            live555/liveMedia/MPEG4ESVideoRTPSink.cpp \
            live555/liveMedia/MPEG4ESVideoRTPSource.cpp \
            live555/liveMedia/MPEG4GenericRTPSink.cpp \
            live555/liveMedia/MPEG4GenericRTPSource.cpp \
            live555/liveMedia/MPEG4LATMAudioRTPSink.cpp \
            live555/liveMedia/MPEG4LATMAudioRTPSource.cpp \
            live555/liveMedia/MPEG4VideoFileServerMediaSubsession.cpp \
            live555/liveMedia/MPEG4VideoStreamDiscreteFramer.cpp \
            live555/liveMedia/MPEG4VideoStreamFramer.cpp \
            live555/liveMedia/MPEGVideoStreamFramer.cpp \
            live555/liveMedia/MPEGVideoStreamParser.cpp \
            live555/liveMedia/MultiFramedRTPSink.cpp \
            live555/liveMedia/MultiFramedRTPSource.cpp \
            live555/liveMedia/OggDemuxedTrack.cpp \
            live555/liveMedia/OggFile.cpp \
            live555/liveMedia/OggFileParser.cpp \
            live555/liveMedia/OggFileServerDemux.cpp \
            live555/liveMedia/OggFileServerMediaSubsession.cpp \
            live555/liveMedia/OggFileSink.cpp \
            live555/liveMedia/OnDemandServerMediaSubsession.cpp \
            live555/liveMedia/ourMD5.cpp \
            live555/liveMedia/OutputFile.cpp \
            live555/liveMedia/PassiveServerMediaSubsession.cpp \
            live555/liveMedia/ProxyServerMediaSession.cpp \
            live555/liveMedia/QCELPAudioRTPSource.cpp \
            live555/liveMedia/QuickTimeFileSink.cpp \
            live555/liveMedia/QuickTimeGenericRTPSource.cpp \
            live555/liveMedia/RawVideoRTPSink.cpp \
            live555/liveMedia/RawVideoRTPSource.cpp \
            live555/liveMedia/RTCP.cpp \
            live555/liveMedia/RTPInterface.cpp \
            live555/liveMedia/RTPSink.cpp \
            live555/liveMedia/RTPSource.cpp \
            live555/liveMedia/RTSPClient.cpp \
            live555/liveMedia/RTSPCommon.cpp \
            live555/liveMedia/RTSPRegisterSender.cpp \
            live555/liveMedia/RTSPServer.cpp \
            live555/liveMedia/RTSPServerRegister.cpp \
            live555/liveMedia/RTSPServerSupportingHTTPStreaming.cpp \
            live555/liveMedia/ServerMediaSession.cpp \
            live555/liveMedia/SimpleRTPSink.cpp \
            live555/liveMedia/SimpleRTPSource.cpp \
            live555/liveMedia/SIPClient.cpp \
            live555/liveMedia/StreamParser.cpp \
            live555/liveMedia/StreamReplicator.cpp \
            live555/liveMedia/T140TextRTPSink.cpp \
            live555/liveMedia/TCPStreamSink.cpp \
            live555/liveMedia/TextRTPSink.cpp \
            live555/liveMedia/TheoraVideoRTPSink.cpp \
            live555/liveMedia/TheoraVideoRTPSource.cpp \
            live555/liveMedia/uLawAudioFilter.cpp \
            live555/liveMedia/VideoRTPSink.cpp \
            live555/liveMedia/VorbisAudioRTPSink.cpp \
            live555/liveMedia/VorbisAudioRTPSource.cpp \
            live555/liveMedia/VP8VideoRTPSink.cpp \
            live555/liveMedia/VP8VideoRTPSource.cpp \
            live555/liveMedia/VP9VideoRTPSink.cpp \
            live555/liveMedia/VP9VideoRTPSource.cpp \
            live555/liveMedia/WAVAudioFileServerMediaSubsession.cpp \
            live555/liveMedia/WAVAudioFileSource.cpp \
            live555/UsageEnvironment/HashTable.cpp \
            live555/groupsock/inet.c \
            live555/liveMedia/rtcp_from_spec.c \
            live555/mytest/h264videosource.cpp \
            live555/mytest/h264mediasubsession.cpp \
            video/vio.cpp \
            video/videoplay.cpp \
            video/regionctr.cpp \
            video/record.cpp \
            video/mp4file.cpp \
            video/recordtomp4.cpp \
            video/videodetect.cpp \
            video/videoplaylist.cpp \
            video/videolink.cpp \
            video/realtimestream.cpp \
            video/videostreampro.cpp \
            test/venctomp4test.cpp \
            video/filetabfind.cpp  \
            video/mp4play.cpp  \
            HW/mpivenc.cpp  \
            HW/mpivdec.cpp \
            HW/mpivpss.cpp

HEADERS +=common/loadbmp.h \
            common/sample_common_sys.h \
            common/sample_common_vpss.h \
            common/sample_common_vdec.h \
#            common/sample_common_vio.h \
            common/sample_common_venc.h \
            common/sample_common_vi.h \
            common/sample_common_vo.h \
            live555/BasicUsageEnvironment/include/BasicHashTable.hh \
            live555/groupsock/include/GroupEId.hh \
            live555/groupsock/include/Groupsock.hh \
            live555/groupsock/include/groupsock_version.hh \
            live555/groupsock/include/GroupsockHelper.hh \
            live555/groupsock/include/IOHandlers.hh \
            live555/groupsock/include/NetAddress.hh \
            live555/groupsock/include/NetCommon.h \
            live555/groupsock/include/NetInterface.hh \
            live555/groupsock/include/TunnelEncaps.hh \
            live555/liveMedia/include/AC3AudioFileServerMediaSubsession.hh \
            live555/liveMedia/include/AC3AudioRTPSink.hh \
            live555/liveMedia/include/AC3AudioRTPSource.hh \
            live555/liveMedia/include/AC3AudioStreamFramer.hh \
            live555/liveMedia/include/ADTSAudioFileServerMediaSubsession.hh \
            live555/liveMedia/include/ADTSAudioFileSource.hh \
            live555/liveMedia/include/AMRAudioFileServerMediaSubsession.hh \
            live555/liveMedia/include/AMRAudioFileSink.hh \
            live555/liveMedia/include/AMRAudioFileSource.hh \
            live555/liveMedia/include/AMRAudioRTPSink.hh \
            live555/liveMedia/include/AMRAudioRTPSource.hh \
            live555/liveMedia/include/AMRAudioSource.hh \
            live555/liveMedia/include/AudioInputDevice.hh \
            live555/liveMedia/include/AudioRTPSink.hh \
            live555/liveMedia/include/AVIFileSink.hh \
            live555/liveMedia/include/Base64.hh \
            live555/liveMedia/include/BasicUDPSink.hh \
            live555/liveMedia/include/BasicUDPSource.hh \
            live555/liveMedia/include/BitVector.hh \
            live555/liveMedia/include/ByteStreamFileSource.hh \
            live555/liveMedia/include/ByteStreamMemoryBufferSource.hh \
            live555/liveMedia/include/ByteStreamMultiFileSource.hh \
            live555/liveMedia/include/DeviceSource.hh \
            live555/liveMedia/include/DigestAuthentication.hh \
            live555/liveMedia/include/DVVideoFileServerMediaSubsession.hh \
            live555/liveMedia/include/DVVideoRTPSink.hh \
            live555/liveMedia/include/DVVideoRTPSource.hh \
            live555/liveMedia/include/DVVideoStreamFramer.hh \
            live555/liveMedia/include/FileServerMediaSubsession.hh \
            live555/liveMedia/include/FileSink.hh \
            live555/liveMedia/include/FramedFileSource.hh \
            live555/liveMedia/include/FramedFilter.hh \
            live555/liveMedia/include/FramedSource.hh \
            live555/liveMedia/include/GenericMediaServer.hh \
            live555/liveMedia/include/GSMAudioRTPSink.hh \
            live555/liveMedia/include/H261VideoRTPSource.hh \
            live555/liveMedia/include/H263plusVideoFileServerMediaSubsession.hh \
            live555/liveMedia/include/H263plusVideoRTPSink.hh \
            live555/liveMedia/include/H263plusVideoRTPSource.hh \
            live555/liveMedia/include/H263plusVideoStreamFramer.hh \
            live555/liveMedia/include/H264or5VideoFileSink.hh \
            live555/liveMedia/include/H264or5VideoRTPSink.hh \
            live555/liveMedia/include/H264or5VideoStreamDiscreteFramer.hh \
            live555/liveMedia/include/H264or5VideoStreamFramer.hh \
            live555/liveMedia/include/H264VideoFileServerMediaSubsession.hh \
            live555/liveMedia/include/H264VideoFileSink.hh \
            live555/liveMedia/include/H264VideoRTPSink.hh \
            live555/liveMedia/include/H264VideoRTPSource.hh \
            live555/liveMedia/include/H264VideoStreamDiscreteFramer.hh \
            live555/liveMedia/include/H264VideoStreamFramer.hh \
            live555/liveMedia/include/H265VideoFileServerMediaSubsession.hh \
            live555/liveMedia/include/H265VideoFileSink.hh \
            live555/liveMedia/include/H265VideoRTPSink.hh \
            live555/liveMedia/include/H265VideoRTPSource.hh \
            live555/liveMedia/include/H265VideoStreamDiscreteFramer.hh \
            live555/liveMedia/include/H265VideoStreamFramer.hh \
            live555/liveMedia/include/InputFile.hh \
            live555/liveMedia/include/JPEG2000VideoRTPSink.hh \
            live555/liveMedia/include/JPEG2000VideoRTPSource.hh \
            live555/liveMedia/include/JPEGVideoRTPSink.hh \
            live555/liveMedia/include/JPEGVideoRTPSource.hh \
            live555/liveMedia/include/JPEGVideoSource.hh \
            live555/liveMedia/include/liveMedia.hh \
            live555/liveMedia/include/liveMedia_version.hh \
            live555/liveMedia/include/Locale.hh \
            live555/liveMedia/include/MatroskaFile.hh \
            live555/liveMedia/include/MatroskaFileServerDemux.hh \
            live555/liveMedia/include/Media.hh \
            live555/liveMedia/include/MediaSession.hh \
            live555/liveMedia/include/MediaSink.hh \
            live555/liveMedia/include/MediaSource.hh \
            live555/liveMedia/include/MediaTranscodingTable.hh \
            live555/liveMedia/include/MP3ADU.hh \
            live555/liveMedia/include/MP3ADUinterleaving.hh \
            live555/liveMedia/include/MP3ADURTPSink.hh \
            live555/liveMedia/include/MP3ADURTPSource.hh \
            live555/liveMedia/include/MP3ADUTranscoder.hh \
            live555/liveMedia/include/MP3AudioFileServerMediaSubsession.hh \
            live555/liveMedia/include/MP3FileSource.hh \
            live555/liveMedia/include/MP3Transcoder.hh \
            live555/liveMedia/include/MPEG1or2AudioRTPSink.hh \
            live555/liveMedia/include/MPEG1or2AudioRTPSource.hh \
            live555/liveMedia/include/MPEG1or2AudioStreamFramer.hh \
            live555/liveMedia/include/MPEG1or2Demux.hh \
            live555/liveMedia/include/MPEG1or2DemuxedElementaryStream.hh \
            live555/liveMedia/include/MPEG1or2DemuxedServerMediaSubsession.hh \
            live555/liveMedia/include/MPEG1or2FileServerDemux.hh \
            live555/liveMedia/include/MPEG1or2VideoFileServerMediaSubsession.hh \
            live555/liveMedia/include/MPEG1or2VideoRTPSink.hh \
            live555/liveMedia/include/MPEG1or2VideoRTPSource.hh \
            live555/liveMedia/include/MPEG1or2VideoStreamDiscreteFramer.hh \
            live555/liveMedia/include/MPEG1or2VideoStreamFramer.hh \
            live555/liveMedia/include/MPEG2IndexFromTransportStream.hh \
            live555/liveMedia/include/MPEG2TransportFileServerMediaSubsession.hh \
            live555/liveMedia/include/MPEG2TransportStreamAccumulator.hh \
            live555/liveMedia/include/MPEG2TransportStreamFramer.hh \
            live555/liveMedia/include/MPEG2TransportStreamFromESSource.hh \
            live555/liveMedia/include/MPEG2TransportStreamFromPESSource.hh \
            live555/liveMedia/include/MPEG2TransportStreamIndexFile.hh \
            live555/liveMedia/include/MPEG2TransportStreamMultiplexor.hh \
            live555/liveMedia/include/MPEG2TransportStreamTrickModeFilter.hh \
            live555/liveMedia/include/MPEG2TransportUDPServerMediaSubsession.hh \
            live555/liveMedia/include/MPEG4ESVideoRTPSink.hh \
            live555/liveMedia/include/MPEG4ESVideoRTPSource.hh \
            live555/liveMedia/include/MPEG4GenericRTPSink.hh \
            live555/liveMedia/include/MPEG4GenericRTPSource.hh \
            live555/liveMedia/include/MPEG4LATMAudioRTPSink.hh \
            live555/liveMedia/include/MPEG4LATMAudioRTPSource.hh \
            live555/liveMedia/include/MPEG4VideoFileServerMediaSubsession.hh \
            live555/liveMedia/include/MPEG4VideoStreamDiscreteFramer.hh \
            live555/liveMedia/include/MPEG4VideoStreamFramer.hh \
            live555/liveMedia/include/MPEGVideoStreamFramer.hh \
            live555/liveMedia/include/MultiFramedRTPSink.hh \
            live555/liveMedia/include/MultiFramedRTPSource.hh \
            live555/liveMedia/include/OggFile.hh \
            live555/liveMedia/include/OggFileServerDemux.hh \
            live555/liveMedia/include/OggFileSink.hh \
            live555/liveMedia/include/OnDemandServerMediaSubsession.hh \
            live555/liveMedia/include/ourMD5.hh \
            live555/liveMedia/include/OutputFile.hh \
            live555/liveMedia/include/PassiveServerMediaSubsession.hh \
            live555/liveMedia/include/ProxyServerMediaSession.hh \
            live555/liveMedia/include/QCELPAudioRTPSource.hh \
            live555/liveMedia/include/QuickTimeFileSink.hh \
            live555/liveMedia/include/QuickTimeGenericRTPSource.hh \
            live555/liveMedia/include/RawVideoRTPSink.hh \
            live555/liveMedia/include/RawVideoRTPSource.hh \
            live555/liveMedia/include/RTCP.hh \
            live555/liveMedia/include/RTPInterface.hh \
            live555/liveMedia/include/RTPSink.hh \
            live555/liveMedia/include/RTPSource.hh \
            live555/liveMedia/include/RTSPClient.hh \
            live555/liveMedia/include/RTSPCommon.hh \
            live555/liveMedia/include/RTSPRegisterSender.hh \
            live555/liveMedia/include/RTSPServer.hh \
            live555/liveMedia/include/RTSPServerSupportingHTTPStreaming.hh \
            live555/liveMedia/include/ServerMediaSession.hh \
            live555/liveMedia/include/SimpleRTPSink.hh \
            live555/liveMedia/include/SimpleRTPSource.hh \
            live555/liveMedia/include/SIPClient.hh \
            live555/liveMedia/include/StreamReplicator.hh \
            live555/liveMedia/include/T140TextRTPSink.hh \
            live555/liveMedia/include/TCPStreamSink.hh \
            live555/liveMedia/include/TextRTPSink.hh \
            live555/liveMedia/include/TheoraVideoRTPSink.hh \
            live555/liveMedia/include/TheoraVideoRTPSource.hh \
            live555/liveMedia/include/uLawAudioFilter.hh \
            live555/liveMedia/include/VideoRTPSink.hh \
            live555/liveMedia/include/VorbisAudioRTPSink.hh \
            live555/liveMedia/include/VorbisAudioRTPSource.hh \
            live555/liveMedia/include/VP8VideoRTPSink.hh \
            live555/liveMedia/include/VP8VideoRTPSource.hh \
            live555/liveMedia/include/VP9VideoRTPSink.hh \
            live555/liveMedia/include/VP9VideoRTPSource.hh \
            live555/liveMedia/include/WAVAudioFileServerMediaSubsession.hh \
            live555/liveMedia/include/WAVAudioFileSource.hh \
            live555/liveMedia/EBMLNumber.hh \
            live555/liveMedia/H263plusVideoStreamParser.hh \
            live555/liveMedia/MatroskaDemuxedTrack.hh \
            live555/liveMedia/MatroskaFileParser.hh \
            live555/liveMedia/MatroskaFileServerMediaSubsession.hh \
            live555/liveMedia/MP3ADUdescriptor.hh \
            live555/liveMedia/MP3AudioMatroskaFileServerMediaSubsession.hh \
            live555/liveMedia/MP3Internals.hh \
            live555/liveMedia/MP3InternalsHuffman.hh \
            live555/liveMedia/MP3StreamState.hh \
            live555/liveMedia/MPEGVideoStreamParser.hh \
            live555/liveMedia/OggDemuxedTrack.hh \
            live555/liveMedia/OggFileParser.hh \
            live555/liveMedia/OggFileServerMediaSubsession.hh \
            live555/liveMedia/rtcp_from_spec.h \
            live555/liveMedia/StreamParser.hh \
            live555/UsageEnvironment/include/HashTable.hh \
            live555/mytest/h264videosource.h \
            live555/mytest/h264mediasubsession.h \
            video/vio.h \
            video/videoplay.h \
            video/regionctr.h \
            video/record.h \
            video/mp4file.h \
            video/recordtomp4.h \
            video/videodetect.h \
            video/videoplaylist.h \
            video/videolink.h \
            video/realtimestream.h \
            video/videostreampro.h \
            test/venctomp4test.h \
            video/filetabfind.h  \
            video/mp4play.h  \
            HW/mpivenc.h  \
            HW/mpivdec.h \
            HW/mpivpss.h


LIBS += -lpthread

LIBS += /usr/lib/libmpi.so \
        /usr/lib/libhdmi.so \
        /usr/lib/libjpeg.so  \
        /usr/lib/libVoiceEngine.so \
        /usr/lib/libupvqe.so  \
        /usr/lib/libdnvqe.so  \
        /usr/lib/libtde.so \
        /usr/lib/libive.so \
        /usr/lib/libmd.so \
        /usr/lib/libpciv.so \
        /usr/lib/libaacenc.so \
        /usr/lib/libaacdec.so \
        /home/abhw/nfsroot/lib/install/lib/libfreetype.so \
        /home/abhw/nfsroot/lib/install/lib/libfreetype.so.6 \
        /home/abhw/nfsroot/lib/install/lib/libSDL2_ttf-2.0.so.0 \
        /home/abhw/nfsroot/lib/install/lib/libSDL2-2.0.so.0  \
        /home/abhw/libdir/ffmpeg-hisi/lib/libavformat.so.58 \
        /home/abhw/libdir/ffmpeg-hisi/lib/libavcodec.so \
        /home/abhw/libdir/ffmpeg-hisi/lib/libavdevice.so \
        /home/abhw/libdir/ffmpeg-hisi/lib/libavfilter.so \
        /home/abhw/libdir/ffmpeg-hisi/lib/libavutil.so \
        /home/abhw/libdir/ffmpeg-hisi/lib/libswscale.so \
        /home/abhw/libdir/ffmpeg-hisi/lib/libswresample.so

    message("arm platform")

}

FORMS += \
        widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qml.qrc
