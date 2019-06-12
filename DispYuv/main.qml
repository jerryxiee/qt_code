import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtMultimedia 5.0
//import com.yourcompany.FrameProvider 1.0


Window {
    visible: true
    width: 1280
    height: 720
    title: qsTr("Hello World")

//    FrameProvider{
//            objectName: "provider"
//            id: provider
//        }


//    MediaPlayer {
//            id: player
//            source: "file:///home/abhw/Videos/dde-introduction.mp4"
//            autoPlay: true
//        }
//    Camera{
//        id:camera

//    }

//    FrameProvider{
//        id:provider
//    }
        VideoOutput {
            id: videoOutput
            source: frameProvider
            anchors.fill: parent
        }
}
