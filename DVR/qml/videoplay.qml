import QtQuick 2.0
import QtQuick.Controls 1.2
import MyTableViewModel 1.0
import QtQuick.Dialogs 1.2
import "Controls"
//import FileInfo 1.0

Rectangle {
    id:rootVideo



    property var years
    property var month
    property var days
    property var hours
    property var minutes
    property var seconds
    property var week
//    property var ndate

    property int mwidth: 256
    property int mhight: 680
    function getCurDate()
    {
         var d = new Date();

         switch (d.getDay()){
         case 1: week="星期一"; break;
         case 2: week="星期二"; break;
         case 3: week="星期三"; break;
         case 4: week="星期四"; break;
         case 5: week="星期五"; break;
         case 6: week="星期六"; break;
         default: week="星期天";
         }
         years = d.getFullYear();
         month = add_zero(d.getMonth()+1);
         days = add_zero(d.getDate());
         hours = add_zero(d.getHours());
         minutes = add_zero(d.getMinutes());
         seconds=add_zero(d.getSeconds());
//         ndate = years+"-"+month+"-"+days+"\n"+hours+":"+minutes+":"+seconds;
//         return ndate;
    }

    function add_zero(temp)
    {
         if(temp<10) return "0"+temp;
         else return temp;
    }


    MessageDialog{
        id:warning
        text:"paramter error"

        standardButtons:    StandardButton.Yes
        onYes: console.log("yes")
    }

    MyTableViewModel {
        id:dataModel
        roles: ["name","gender","na"]
        objectName:"MyFileTable"
        onFileNameChanged:{
            videoDispSignal(name)
            console.log("display"+name)
        }

        onPathChanged: {

            filepath.text = mCurrentPath

        }

    }

    Row{
        Rectangle{
            id:buttonEare
            width:mwidth
            height: mhight
//            color: "blue"

//            Column{
////                id:row1
//                anchors.top: buttonEare.top
//                anchors.topMargin: 100

                Label{
                    id:findtype
//                    anchors.top: buttonEare.top
//                    x:20
                    y:100
//                    anchors.topMargin: 100
//                    anchors.rightMargin: 20
                    text: qsTr("查询方式")
                    font.pixelSize: 18

                }
                ComboBox{
                    id:findtypeBox
                    x:findtype.width + 20; y:findtype.y
                    width: 150
                    model: ListModel{
                        ListElement{text:qsTr("常规查询")}
                    }
                }

                Label{
                    id:chnSelect
                    anchors.top: findtype.bottom
                    anchors.topMargin: 20
                    text: qsTr("通道选择")
                    font.pixelSize: 18

                }

                ComboBox{
                    id:chnSelectBox
                    width: 150
                    x:chnSelect.width + 20; y:chnSelect.y
                    model: ListModel{
                        ListElement{text:qsTr("Chn0")}
                        ListElement{text:qsTr("Chn1")}
                        ListElement{text:qsTr("Chn2")}
                        ListElement{text:qsTr("Chn3")}
                        ListElement{text:qsTr("Chn4")}
                        ListElement{text:qsTr("Chn5")}
                        ListElement{text:qsTr("Chn6")}
                        ListElement{text:qsTr("Chn7")}
                    }
                }
                Label{
                    id:fileType
                    anchors.top: chnSelect.bottom
                    anchors.topMargin: 20
                    text: qsTr("文件类型")
                    font.pixelSize: 18

                }
//
                ComboBox{
                    id:fileTypeBox
                    width: 150
                    x:fileType.width + 20; y:fileType.y
                    model: ListModel{
                        ListElement{text:qsTr("全部文件")}
                        ListElement{text:qsTr("移动侦测")}
                    }
                }

                Label{
                    id:starttime
                    anchors.top: fileType.bottom
                    anchors.topMargin: 20
                    text: qsTr("开始时间")
                    font.pixelSize: 18

                }

//                Column{
//                    x:starttime.width + 20; y:starttime.y
//                    Row{

//                        SpinBox{
//                            id:yearBox
//                            minimumValue:2019
//                            maximumValue: 2050
////                            value: years


//                        }
//                        SpinBox{
//                            id:monthBox
//                            minimumValue:1
//                            maximumValue: 12
////                            value: month

//                        }
//                        SpinBox{
//                            id:dayBox
//                            minimumValue:1
//                            maximumValue: 31
////                            value: days


//                        }
//                    }
//                    DatePicker{
//                        id:datetext
//                        x:starttime.width + 20; y:starttime.y
//                        width: 150
//                               dateValue: (new Date()).toLocaleString(Qt.locale(), "yyyy-MM-dd")
//                               onDateValueChanged: {
//                                   console.log("x:"+x)
//                                   text = dateValue
//                               }
//                    }
                    MyDateTime{
                        id:startd
                        x:starttime.width + 20; y:starttime.y
                        width: 150
//                        height: starttime.height + 5
                        text: (new Date()).toLocaleString(Qt.locale(), "yyyy/MM/dd")

                //        onPressed: vkb.visible = true;

                        onValueChanged: console.log("test value:"+value)
                    }

                    MyDateTime{
                        id:startt
                        anchors.top: startd.bottom
                        anchors.topMargin: 5
                        anchors.left: startd.left
                        type: "time"
                        width: 150
//                        height: starttime.height + 5
                        text: (new Date()).toLocaleString(Qt.locale(), "hh/mm/ss")

                //        onPressed: vkb.visible = true;

                        onValueChanged: console.log("test value:"+value)
                    }


//                    Row{
//                        anchors.top: datetext.bottom
//                        anchors.left: datetext.left
//                        SpinBox{
//                            id:hBox
//                            minimumValue:0
//                            maximumValue: 23
//                        }
//                        SpinBox{
//                            id:mBox
//                            minimumValue:0
//                            maximumValue: 59

//                        }
//                        SpinBox{
//                            id:sBox
//                            minimumValue:0
//                            maximumValue: 59

//                        }
//                    }
//                }

                Label{
                    id:endtime
                    anchors.top: starttime.bottom
                    anchors.topMargin: 50
                    text: qsTr("结束时间")
                    font.pixelSize: 18

                }
//                Column{
//                    x:endtime.width + 20; y:endtime.y

                    MyDateTime{
                        id:endd
                        x:endtime.width + 20; y:endtime.y
                        width: 150
//                        height: endtime.height + 5
                        text: (new Date()).toLocaleString(Qt.locale(), "yyyy/MM/dd")

                //        onPressed: vkb.visible = true;

                        onValueChanged: console.log("test value:"+value)
                    }

                    MyDateTime{
                        id:endt
                        anchors.top: endd.bottom
                        anchors.topMargin: 5
                        anchors.left: endd.left
                        type: "time"
                        width: 150
//                        height: endtime.height + 5
                        text: (new Date()).toLocaleString(Qt.locale(), "hh/mm/ss")

                //        onPressed: vkb.visible = true;

                        onValueChanged: console.log("test value:"+value)
                    }


//                    Row{

//                        SpinBox{
//                            id:year1Box
//                            minimumValue:2019
//                            maximumValue: 2050
////                            value: years
//                        }
//                        SpinBox{
//                            id:month1Box
//                            minimumValue:1
//                            maximumValue: 12
////                            value: month
//                        }
//                        SpinBox{
//                            id:day1Box
//                            minimumValue:1
//                            maximumValue: 31
////                            value: days
//                        }
//                    }

//                    Row{
//                        SpinBox{
//                            id:h1Box
//                            minimumValue:0
//                            maximumValue: 23
//                            value: maximumValue
//                        }
//                        SpinBox{
//                            id:m1Box
//                            minimumValue:0
//                            maximumValue: 59
//                            value: maximumValue
//                        }
//                        SpinBox{
//                            id:s1Box
//                            minimumValue:0
//                            maximumValue: 59
//                            value: maximumValue
//                        }
//                    }
//                }

                Button{
                    id:play
                    text: qsTr("播放")
                    anchors.top: endtime.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    anchors.topMargin: 60
                    onClicked: {
                        if(dataModel.playVideoList(findtypeBox.currentIndex,chnSelectBox.currentIndex,fileTypeBox.currentIndex,
                                                    startd.text+" "+startt.text,endd.text+" "+endt.text) > 0){
                            console.log("videodisplist")
                            videoDispList();
                        }
//                        console.log(dataModel.mFileInfoList)

                    }

                }

                Button{
                    id:check
                    text: qsTr("查询")
                    anchors.verticalCenter: play.verticalCenter
                    anchors.left: play.right
                    anchors.leftMargin: 30
//                    x:video.width + 20; y:video.y

                    onClicked: {
                        dataModel.searchFile(findtypeBox.currentIndex,chnSelectBox.currentIndex,fileTypeBox.currentIndex,
                                                    startd.text+" "+startt.text,endd.text+" "+endt.text)
                    }

                }

                Button{
                    id:preview
                    text: qsTr("预览")
                    anchors.top: play.bottom
                    anchors.topMargin: 20
                    anchors.left: play.left
//                    anchors.leftMargin: 20
                    onClicked: dataModel.preViewFile()

                }

                Button{
                    id:cancel
                    text: qsTr("取消")
                    anchors.verticalCenter: preview.verticalCenter
                    anchors.left: preview.right
                    anchors.leftMargin: 30
//                    x:video.width + 20; y:video.y

                }


        }
        Column{
            Rectangle{
                height: 40
                width: rootVideo.width - mwidth
                NaviButton {
                    id: filebackButton

            //        edge: Qt.TopEdge
                    enabled: true
                    imageSource: "qrc:/images/left1.png"
                    anchors.left: parent.left
                    imageheight: 40
                    imagewidth: 40
                    imageRotation:180
                    onClicked: {
                        dataModel.onBackButtonClickedSlot()
                    }
                }
                Label{
                    id:filepath
                    text: dataModel.mCurrentPath
                    anchors.left:filebackButton.right
                    anchors.verticalCenter: filebackButton.verticalCenter
                    font.pixelSize: 24
                }
            }
            TableView {
    //            anchors.right: parent.right
    //            anchors.fill: parent
                model: dataModel
                sortIndicatorVisible:true
                width: rootVideo.width - mwidth
                height: mhight - 40

                TableViewColumn { role: "name"; title: "名称"; width: 500;delegate:itemDelegateText}
                TableViewColumn { role: "gender"; title: "修改时间";width: 400;delegate:itemDelegateText}
                TableViewColumn { role: "na"; title: "文件大小";width: 100;delegate:itemDelegateText}

                onDoubleClicked: {
                    dataModel.oncellDoubleClickedSlot(row,0)
                }

                Component{
                    id:itemDelegateText
                    Text {
                        id: name
                        text: styleData.value
                        font.pointSize: 10
                    }

                }


            }
        }
    }

//    Component.onCompleted: console.log("component")


}
