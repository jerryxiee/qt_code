import QtQuick 2.0
//import QtQml 2.12
//import QtQuick.Window 2.0
//import QtQuick.Controls 1.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import MyTableViewModel 1.0
import QtQuick.Controls.Styles 1.2
import QtQuick.VirtualKeyboard 2.2
import "./"
import "Controls/"
//import "Datepicker.qml"

//Window{
Rectangle {

//    color:"black"

    property string dateValue
    property string str

//    Calendar{
//        id: calendar
////        anchors.topMargin: 0
////        anchors.top: parent.bottom
//        visible: false
//        activeFocusOnTab: true
//        onReleased: {
//            str = date;
//            str = str.substr(0, 10);
//            dateValue = str;
//            visible = false;
//        }
//    }
//    TextField{
//        id:dat
//        text: (new Date()).toLocaleString(Qt.locale(), "yyyy-MM-dd")

//    }
//    Button{
//        id: downBtn
//        width: 22
////        anchors.right: parent.right
////        anchors.rightMargin: 0
////        anchors.bottom: parent.bottom
////        anchors.bottomMargin: 0
////        anchors.top: parent.top
////        anchors.topMargin: 0
////        iconSource: "images/arrow_bc.png"
//        anchors.left: dat.right
//        anchors.top: dat.top
//        onClicked: {
////            calendar.top = dat.bottom
////            calendar.left = dat.left
//            calendar.x = dat.x
//            calendar.y = dat.height
//            calendar.visible = !calendar.visible
//        }
//    }
//    onDateValueChanged: {
//        dat.text = dateValue;
//        calendar.selectedDate = dateValue;
//    }





//    MyComboBox {
//        id: comboBoxButton
////        text:qsTr("text")
//        model:fruitModel
//        backgroundColor:"black"
//        textColor:"white"
////        currentIndex:0
////        anchors.fill: parent;
////        text: typeof(listModel) == "undefined"? "":typeof( listModel.get(currentIndex).attributes)=="undefined"?listModel.get(comboBox.currentIndex).name:listModel.get(currentIndex).attributes.get(currentIndex2).description
////        textSize: parent.height-4
////        textColor: comboBox.activeFocus ? comboBox.focus_textColor : comboBox.textColor
////        borderColor: comboBox.activeFocus ? comboBox.focus_borderColor : comboBox.borderColor
////        backgroundColor: comboBox.activeFocus ? comboBox.focus_backgroundColor : comboBox.backgroundColor
////        onClicked: comboxClick()
//    }

    ListModel {
            id: fruitModel

            ListElement { name: "Core" }
            ListElement { name: "Deciduous" }

    }
    Mycombobox{
        id:mycombobox
        listModel: fruitModel/*;anchors.centerIn: parent*/
    }
//    ListModel {
//        id: fruitModel
//        ListElement { name: "Apple"
//            attributes: [
//                ListElement { description: "Core" },
//                ListElement { description: "Deciduous" }
//            ]
//        }
//        ListElement {
//            name: "Orange"
//            attributes: [
//                ListElement { description: "Citrus" }
//            ]
//        }
//        ListElement {
//            name: "mango"
//        }
//        ListElement {
//            name: "Banana"
//            attributes: [
//                ListElement { description: "Tropical" },
//                ListElement { description: "Seedless" },
//                ListElement { description: "Yellow" }
//            ]
//        }
//    }


//    TabBar{
//            id:bar;
//            width:100*count

//            onCurrentIndexChanged: console.log("index "+currentIndex)



//            TabButton{
//                text:qsTr("Home");

//            }
//            TabButton{

//                text:qsTr("Discover");
//            }
//            TabButton{
//                text:qsTr("Activity");
//            }
//        }


//        StackLayout{
//            width:parent.width;
//            height: parent.height - bar.height
//            anchors.top: bar.bottom
//            currentIndex:bar.currentIndex;
//            Item{
//                id:homeTab
////                Button{
////                    style:ButtonStyle{
////                        label:Text {
////                            id: name
////                            text: qsTr("text")
////                            color: control.pressed?"red":"blue"
////                        }
////                    }
////                }
//                Text {
//                    anchors.centerIn: parent
//                    color: "white"

//                    text: qsTr("text")
//                    font.pixelSize: 24
//                }
//            }
//            Item{
//                id:discoverTab
//                Text {
//                    anchors.centerIn: parent
//                    color: "white"
//                    text: qsTr("text1")
//                    font.pixelSize: 24
//                }
//            }
//            Item{
//                id:activityTab
//                Text {
//                    anchors.centerIn: parent
//                    color: "white"
//                    text: qsTr("text2")
//                    font.pixelSize: 24
//                }
//            }

//        }

//    TabBar{
//        id:bar
//        width:parent.width
//        Repeater{
//            model:["First","Second","Third","Fourth","Fifth","Fifth","Fifth","Fifth","Fifth"]
//            TabButton{
//                text:modelData;
//                width:Math.max(100,bar.width/5);
//            }
//        }
//    }

//    Datepicker{
//        width: 150
//               dateValue: (new Date()).toLocaleString(Qt.locale(), "yyyy-MM-dd")
//               onDateValueChanged: text = dateValue
//    }

//    Rectangle{
//        id:rect1
//        color: "blue"
//        width: 500
//        height: 500
//        TextField {
////                anchors.top: parent.top
////                anchors.horizontalCenter: parent.horizontalCenter
//                onPressed: {
//                    vkb.visible = true; //当选择输入框的时候才显示键盘
//                }
//         }
//    }
//    Rectangle{
//        color: "red"
//        width: 500
//        height: 500
//        anchors.left:rect1.right
//        TextField {
////                anchors.top: parent.top
////                anchors.horizontalCenter: parent.horizontalCenter
//                onPressed: {
//                    vkb.visible = true; //当选择输入框的时候才显示键盘
//                }
//         }
//    }

//    Font{
//        id:font1
//        pixelSize: 25
//    }

//    property int index: 0
//    Row{
//        TextField{
//            onPressed: index = 1

//        }
//        Label{
//            text: ":"
//            font.pixelSize: 32

//        }
//        TextField{
//            onPressed: index = 2

//        }
//        Label{
//            text: ":"
//            font.pixelSize: 32
//        }
//        TextField{
//            onPressed: index = 3

//        }

//    }

//    TimeInput{
//        id:time

//    }
//    TextField{
//        focus: true
//        text: "2029/01/01"
////        inputMask: "0000/00/00"
//        validator: RegExpValidator{regExp:/(\d\d\d\d\/[01]?[0-2]\/[03]?[0-1])/}


//        color: focus?"blue":"red"
//    }

    Datepicker{
        id:date
        anchors.top:mycombobox.bottom
        width: 150
               dateValue: (new Date()).toLocaleString(Qt.locale(), "yyyy-MM-dd")
               onDateValueChanged: text = dateValue
    }

    Button{
        anchors.top: date.bottom
    }

    TextField {
        id: textfield_operation_ip1
        x: 492
        y: 120
        width: 269
        height: 61
        text: ""
        visible: true
        color: focus?"blue":"red"
//        background: {

//        }



        placeholderText: qsTr("0.0.0.0")
        font.pixelSize: 25
        focus:true
        onPressed: {
            vkb.visible = true; //当选择输入框的时候才显示键盘
            console.log("curpos "+cursorPosition)
        }

        validator: RegExpValidator{regExp:/(?=(\b|\D))(((\d{1,2})|(1\d{1,2})|(2[0-4]\d)|(25[0-5]))\.){3}((\d{1,2})|(1\d{1,2})|(2[0-4]\d)|(25[0-5]))(?=(\b|\D))/}
    }

    Mydate{
        id:datedis
        anchors.top: textfield_operation_ip1.top
        anchors.right: textfield_operation_ip1.left

//        onPressed: vkb.visible = true;

        onValueChanged: console.log("test value:"+value)
    }

    Mydate{
        type: "time"
        anchors.top: datedis.bottom
        anchors.topMargin:10
        anchors.left: datedis.left
    }
    Button{
        anchors.left: textfield_operation_ip1.right
        anchors.top: textfield_operation_ip1.top
//        onClicked: console.log("index "+time.index)
    }
        InputPanel {
            id: vkb
            visible: false
//            anchors.right: parent.right
//            anchors.left: parent.left
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            width: parent.width /2
            height: parent.height/4
            //这种集成方式下点击隐藏键盘的按钮是没有效果的，
            //只会改变active，因此我们自己处理一下
            onActiveChanged: {
                if(!active) { visible = false; }
            }
        }

}




