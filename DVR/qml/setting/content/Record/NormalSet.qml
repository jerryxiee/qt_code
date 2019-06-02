import QtQuick 2.0
import QtQuick.Controls 1.2
//import QtQuick.Controls.Styles 1.1

Item {
    property int fontpixelSize : 32
    property int boxwidth: 300
    property int index: 0
    Connections{
        target: record
        onPressed:{
            if(setindex == index){
                VideoControl.setVencAttr(setchnBox.currentText,setStreamBox.currentText,setStreamTypeBox.currentText,resolutionBox.currentText
                                                         ,bitRateTyprBox.currentText,imageQualityBox.currentText,frameRateBox.currentText,bitRateBox.currentText)
            }
        }
    }

    Row{
//        anchors.left: parent.left
//        anchors.top: parent.top
//        anchors.leftMargin:200
//        anchors.margins:50
        anchors.top: parent.top
        anchors.topMargin:50
        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.centerIn: record
        spacing:150
        Column{

            spacing:10
            Text{
                id:setchn
                text:qsTr("通道选择")
//                color:"white"
                font.pixelSize: fontpixelSize
            }
            Text{
                id:setStream
                text:qsTr("码流选择")
//                color:"white"
                font.pixelSize: fontpixelSize
            }
            Text{
                id:setStreamType
                text:qsTr("码流类型")
//                color:"white"
                font.pixelSize: fontpixelSize
            }
            Text{
                id: resolution
                text:qsTr("分辨率")
//                color:"white"
                font.pixelSize: fontpixelSize
            }
            Text{
                id: bitRateTypr
                text:qsTr("码率类型")
//                color:"white"
                font.pixelSize: fontpixelSize
            }
            Text{
                id: imageQuality
                text:qsTr("图像质量")
//                color:"white"
                font.pixelSize: fontpixelSize
            }

            Text{
                id: frameRate
                text:qsTr("视频帧率")
//                color:"white"
                font.pixelSize: fontpixelSize
            }
            Text{
                id: bitRate
                text:qsTr("码流设置")
//                color:"white"
                font.pixelSize: fontpixelSize
            }
//            Text{
//                id: recordPlay
//                text:qsTr("录像时间配置")
////                color:"white"
//                font.pixelSize: fontpixelSize
//            }

//            Text{
//                id: otherSet
//                text:qsTr("其他配置")
////                color:"white"
//                font.pixelSize: fontpixelSize
//            }


        }
        Column{
            spacing:10
            ComboBox{
                id:setchnBox
                width: boxwidth
                height:setchn.height
                model: ListModel{
                    ListElement{text:qsTr("模拟通道0")}
                    ListElement{text:qsTr("模拟通道1")}
                    ListElement{text:qsTr("模拟通道2")}
                    ListElement{text:qsTr("模拟通道3")}
                    ListElement{text:qsTr("模拟通道4")}
                    ListElement{text:qsTr("模拟通道5")}
                    ListElement{text:qsTr("模拟通道6")}
                    ListElement{text:qsTr("模拟通道7")}
                }
            }

            ComboBox{
                id:setStreamBox
                width: boxwidth
                height:setchn.height
                model: ListModel{
                    ListElement{text:qsTr("主码流")}
                    ListElement{text:qsTr("子码流")}

                }
            }

            ComboBox{
                id:setStreamTypeBox
                width: boxwidth
                height:setchn.height
                model: ListModel{
                    ListElement{text:qsTr("视频流")}
                    ListElement{text:qsTr("复合流")}

                }
            }
            ComboBox{
                id:resolutionBox
                width: boxwidth
                height:setchn.height
                model: ListModel{
                    ListElement{text:qsTr("720P")}
                    ListElement{text:qsTr("CIF")}

                }
            }
            ComboBox{
                id:bitRateTyprBox
                width: boxwidth
                height:setchn.height
                model: ListModel{
                    ListElement{text:qsTr("定码率")}
                    ListElement{text:qsTr("变码率")}

                }
            }

            ComboBox{
                id:imageQualityBox
                width: boxwidth
                height:setchn.height
                model: ListModel{
                    ListElement{text:qsTr("低等")}
                    ListElement{text:qsTr("中等")}
                    ListElement{text:qsTr("高等")}

                }
            }

            ComboBox{
                id:frameRateBox
                width: boxwidth
                height:setchn.height
                editable : true
                model: ListModel{
                    ListElement{text:qsTr("15")}
                    ListElement{text:qsTr("20")}
                    ListElement{text:qsTr("25")}
                }
            }

            ComboBox{
                id:bitRateBox
                width: boxwidth
                height:setchn.height
                editable : true
                model: ListModel{
                    ListElement{text:qsTr("2048")}
                    ListElement{text:qsTr("1024")}

                }
            }

        }

    }

}

