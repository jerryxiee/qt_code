/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.1
import "qrc:/qml/Controls"

Item {
    id:record
//    width: parent.width
//    height: parent.height
    property int fontpixelSize : 32
    property int boxwidth: 300


//Column{
    Row{
//        anchors.left: parent.left
//        anchors.top: parent.top
//        anchors.leftMargin:200
//        anchors.margins:50
        anchors.top: record.top
        anchors.topMargin:50
        anchors.horizontalCenter: record.horizontalCenter
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
            Text{
                id: recordPlay
                text:qsTr("录像时间配置")
//                color:"white"
                font.pixelSize: fontpixelSize
            }

            Text{
                id: otherSet
                text:qsTr("其他配置")
//                color:"white"
                font.pixelSize: fontpixelSize
            }


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

//        Row{
//            anchors.bottom:parent.bottom
//            anchors.right:parent.right
//            width:parent.width
//            height:40

//            Button{
//                anchors.bottom:parent.bottom
//                anchors.right:parent.right
//                text:qsTr("应用")
//            }
//        }
    }

    Connections{
        target: setparent
        onSuresignal:{

            VideoControl.setVencAttr(setchnBox.currentText,setStreamBox.currentText,setStreamTypeBox.currentText,resolutionBox.currentText,
                                     bitRateTyprBox.currentText,imageQualityBox.currentText,frameRateBox.currentText,bitRateBox.currentText)
            console.log("sure press")
        }
        onCancelsignal:{

            console.log("cancel press")
        }
    }

//    Row{
//        Button{
//            anchors.bottom:record.bottom
//            anchors.right:record.right
//            text:qsTr("应用")
//        }

//    }
//}
//    Rectangle{
//        width: record.width
//        height: 40
//        color: "#333333"
//        anchors.bottom: record.bottom

//        Button{
//            id:recordsure
//            width:120
//            height:40
////            anchors.bottom:record.bottom
//            anchors.rightMargin:400
//            anchors.right:parent.right
////            anchors.bottomMargin:40
//            text:qsTr("应用")

//            onClicked:{
//                VideoControl.setVencAttr(setchnBox.currentText,setStreamBox.currentText,setStreamTypeBox.currentText,resolutionBox.currentText
//                                         ,bitRateTyprBox.currentText,imageQualityBox.currentText,frameRateBox.currentText,bitRateBox.currentText)
//            }
//        }

//        Button{
//            id:recordcancel
//            width:120
//            height:40
////            anchors.bottom:record.bottom
//            anchors.leftMargin:10
//            anchors.left:recordsure.right
////            anchors.bottomMargin:40
//            text:qsTr("取消")

//            onClicked:{
//                stackview.pop()
//            }
//        }

//    }


//    function set(){
//        console.log("enter set")
//    }

//    function




}
