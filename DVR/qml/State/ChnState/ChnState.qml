import QtQuick 2.0
import QtQuick.Controls 1.2

Item {
    property int fontpixelSize : 24
    property int boxwidth: 300


    TabView{
        id: tabView
        focus: true

        anchors.fill: parent

        Tab{
            title: "Normal"
            Item{
                Text{
                    id:chnname
                    anchors.top: parent.top
                    anchors.topMargin: 50
                    anchors.left: parent.left
                    anchors.leftMargin: 100
                    text:qsTr("通道名称")
                    font.pixelSize: fontpixelSize
                }

                Text{
                    id:constate
                    anchors.left: chnname.right
                    anchors.leftMargin: 150
                    anchors.verticalCenter: chnname.verticalCenter
                    text:qsTr("连接状态")
                    font.pixelSize: fontpixelSize
                }

                Text{
                    id:recordstate
                    anchors.left: constate.right
                    anchors.leftMargin: 150
                    anchors.verticalCenter: constate.verticalCenter
                    text:qsTr("录像状态")
                    font.pixelSize: fontpixelSize
                }


                Column{
                    anchors.top:chnname.bottom
                    anchors.topMargin: 20
                    anchors.left: chnname.left
                    spacing:10
                    Repeater{
                        model: 8
                        Text{
                            font.pixelSize: fontpixelSize
                            text:qsTr("通道"+index)
                        }
                    }
                }

                Column{
                    anchors.top:constate.bottom
                    anchors.topMargin: 20
                    anchors.horizontalCenter: constate.horizontalCenter
                    spacing:14
                    Repeater{
                        model: 8
                        RadioButton{
                            enabled: false

                        }
                    }
                }

                Column{
                    anchors.top:recordstate.bottom
                    anchors.topMargin: 20
                    anchors.horizontalCenter: recordstate.horizontalCenter
                    spacing:14
                    Repeater{
                        model: 8
                        RadioButton{
                            enabled: false
                        }
                    }
                }

                RadioButton{
                    id:but1
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 20
                    anchors.left: parent.left
                    anchors.leftMargin: 100
                    text: qsTr("正常")
                    enabled: false
                    checked: true
                }

                RadioButton{
                    anchors.verticalCenter: but1.verticalCenter
                    anchors.left: but1.right
                    anchors.leftMargin: 100
                    text: qsTr("异常")
                    enabled: false
                }

            }
        }
    }




}
