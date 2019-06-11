import QtQuick 2.0
import QtQuick.Controls 1.2 as Control1
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.2

FocusScope {
    id:root

    width: 150
    height: 40

    property string type: "date"
    property int curindex: 0
    property string value: ""
    property int minyear: 1700
    property alias text: textfield.text
//    property var datelist
//    property alias cursorPosition: textfield.cursorPosition
    property variant datan : [31,28,31,30,31,30,31,31,30,31,30,31]
    signal pressed()

    function calcindex(value){
        if(type == "date"){
            if(value <= 4 ){
                curindex = 0
            }else if(value <=7){
                curindex = 1
            }else{
                curindex = 2
            }
        }else{
            if(value <= 2 ){
                curindex = 0
            }else if(value <=5){
                curindex = 1
            }else{
                curindex = 2
            }

        }

    }

    function checkrunnian(year){
        if(((year%4 == 0)&&(year%100 != 0)) || (year%400 == 0)){
//            console.log("run")
            return true
        }else{
//            console.log("norun")
            return false
        }
    }

    function checkvalue(){
//        var datan = [31,28,31,30,31,30,31,31,30,31,30,31]
        var strlist
        var num
        if(type == "date"){
            strlist = value.split("/");
            if(strlist[0] === "" || Number(strlist[0] < minyear)){
                strlist[0] = minyear.toString();
            }
            if(strlist[1] === "" || Number(strlist[1]) > 12){
                strlist[1] = String("12")
            }
            if(checkrunnian(Number(strlist[0]))){
                    console.log("yes run")
            }
            console.log("dat:"+Number(strlist[2]))
            if(strlist[2] === "" || Number(strlist[1])!==2 && Number(strlist[2]) > datan[Number(strlist[1])-1]){
//                num = datan[Number(strlist[1])-1]
                strlist[2] = datan[Number(strlist[1])-1].toString()
            }else if(Number(strlist[1])===2){
                if((checkrunnian(Number(strlist[0])) && Number(strlist[2]) > datan[Number(strlist[1])-1] + 1)||
                        (!checkrunnian(Number(strlist[0])) && Number(strlist[2]) > datan[Number(strlist[1])-1])){
                    strlist[2] = String("01")
                }

            }

            value = strlist[0]+"/"+strlist[1]+"/"+strlist[2];
        }else{
            strlist = value.split(":");
            if(strlist[0] === "" || Number(strlist[0] > 23)){
                strlist[0] = String("23")
            }
            if(strlist[1] === "" || Number(strlist[1] > 59)){
                strlist[1] = String("59")
            }
            if(strlist[2] === "" || Number(strlist[2] > 59)){
                strlist[2] = String("59")
            }
            value = strlist[0]+":"+strlist[1]+":"+strlist[2];

        }
    }
    function inc(str){
        var intnum;
        var strlist;
        var retstr;

        if(type == "date"){
            strlist = str.split("/");
            console.log(strlist[0] + strlist[1] + strlist[2]);

//            switch(curindex){
//            case 0:
//                intnum = Number(strlist[0]);
//                intnum++;
//                strlist[0] = intnum.toString();
//                break;
//            case 1:
//                intnum = Number(strlist[1]);
//                intnum++;

//                strlist[1] = (intnum<10)?"0"+intnum.toString():intnum.toString();
//                break;
//            case 2:
//                intnum = Number(strlist[2]);
//                intnum++;
//                strlist[2] = (intnum<10)?"0"+intnum.toString():intnum.toString();
//                break;

//            default:
//                return;
//            }
//            retstr = strlist[0]+"/"+strlist[1]+"/"+strlist[2];
//            console.log(retstr)

        }else{
            strlist = str.split(":");
            console.log(strlist[0] + strlist[1] + strlist[2]);
        }

        switch(curindex){
        case 0:
            intnum = Number(strlist[0]);
            intnum++;
            strlist[0] = intnum.toString();
            break;
        case 1:
            intnum = Number(strlist[1]);
            intnum++;

            strlist[1] = (intnum<10)?"0"+intnum.toString():intnum.toString();
            break;
        case 2:
            intnum = Number(strlist[2]);
            intnum++;
            strlist[2] = (intnum<10)?"0"+intnum.toString():intnum.toString();
            break;

        default:
            return;
        }

        if(type == "date"){
            retstr = strlist[0]+"/"+strlist[1]+"/"+strlist[2];
        }else{
            retstr = strlist[0]+":"+strlist[1]+":"+strlist[2];
        }

        console.log(retstr)
        return retstr;
    }
    function dec(str){
        var intnum;
        var strlist;
        var retstr;

        if(type == "date"){
            strlist = str.split("/");
            console.log(strlist[0] + strlist[1] + strlist[2]);

            switch(curindex){
            case 0:
                intnum = Number(strlist[0]);
                intnum--;
                if(intnum < minyear)
                    intnum = minyear
                strlist[0] = intnum.toString();
                break;
            case 1:
                intnum = Number(strlist[1]);
                intnum--;
                if(intnum < 1)
                    intnum = 1;
                strlist[1] = (intnum<10)?"0"+intnum.toString():intnum.toString();
                break;
            case 2:
                intnum = Number(strlist[2]);
                intnum--;
                if(intnum < 1)
                    intnum = 1
                strlist[2] = (intnum<10)?"0"+intnum.toString():intnum.toString();
                break;

            default:
                return;
            }
            retstr = strlist[0]+"/"+strlist[1]+"/"+strlist[2];

        }else{
            strlist = str.split(":");
            console.log(strlist[0] + strlist[1] + strlist[2]);

            switch(curindex){
            case 0:
                intnum = Number(strlist[0]);
                intnum--;
                if(intnum < 0)
                    intnum = 0
                strlist[0] = intnum.toString();
                break;
            case 1:
                intnum = Number(strlist[1]);
                intnum--;
                if(intnum < 0)
                    intnum = 0;
                strlist[1] = (intnum<10)?"0"+intnum.toString():intnum.toString();
                break;
            case 2:
                intnum = Number(strlist[2]);
                intnum--;
                if(intnum < 0)
                    intnum = 0
                strlist[2] = (intnum<10)?"0"+intnum.toString():intnum.toString();
                break;

            default:
                return;
            }
            retstr = strlist[0]+":"+strlist[1]+":"+strlist[2];


        }

        console.log(retstr)
        return retstr;
    }

    function selecttext(index){
        var strlist;
        var start;
        var end;
        if(type == "date"){
            strlist = textfield.text.split("/");
//            switch(index){
//            case 0:start = 0;break
//            case 1:start = String(strlist[0]).length + 1;break
//            case 2:start = String(strlist[0]).length + String(strlist[1]).length + 2;break
//            }

//            end = start+String(strlist[index]).length

        }else{

            strlist = textfield.text.split(":");
        }
        switch(index){
            case 0:start = 0;break
            case 1:start = String(strlist[0]).length + 1;break
            case 2:start = String(strlist[0]).length + String(strlist[1]).length + 2;break
        }

        end = start+String(strlist[index]).length

        textfield.select(start,end)

    }


    TextField{
        id:textfield
        anchors.fill: parent
        text:type=="date"?(new Date()).toLocaleString(Qt.locale(), "yyyy/MM/dd"):(new Date()).toLocaleString(Qt.locale(), "hh:mm:ss")

        inputMask: type=="date"?"0000/00/00":"00:00:00"

        Control1.Button{
            height:parent.height /2
            width: 25
            anchors.top: parent.top
            anchors.right: parent.right

            iconSource: "images/arrow.png"
            onClicked: {
//                if(type == "date"){
                    value = inc(parent.text)
//                }

//                console.log(datelist.count())

            }

        }

        Control1.Button{
            height:parent.height /2
            width: 25
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            iconSource: "images/arrow_bc.png"

//            style:ButtonStyle{

//            }


            onClicked: {
//                if(type == "date"){
                    value = dec(parent.text)
//                }
            }
        }

        onPressed: parent.pressed()

        onCursorPositionChanged: {
            calcindex(cursorPosition)
            console.log("curindex "+curindex)
            console.log("curposition change "+cursorPosition)
        }


    }

    onValueChanged:{
//        if(type == "date"){
            var preindex = curindex
            checkvalue()
            textfield.text = value
            selecttext(preindex)
//        }
        console.log("value: "+value)
//        console.log(((new Date()).toLocaleString(Qt.locale(), "hh:mm:ss")))
    }

//    Component{
//        id:buttstyle
//        ButtonStyle{

//        }
//    }

//    Rectangle{

//    }

}
