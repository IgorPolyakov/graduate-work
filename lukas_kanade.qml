import QtQuick 2.4

Item {
    width: 300
    height: 120

    Label {
        id: label1
        x: 21
        y: 20
        height: 20
        text: qsTr("Averaging:")
        verticalAlignment: Text.AlignVCenter
    }

    SpinBox {
        id: spinBox1
        x: 112
        y: 20
        width: 65
        height: 20
        minimumValue: 1
        maximumValue: 4096
    }

    Label {
        id: label9
        x: 21
        y: 60
        height: 20
        text: qsTr("Output Directory:")
        verticalAlignment: Text.AlignVCenter
    }

    TextField {
        id: outDirField
        objectName: "outDirField"
        x: 112
        y: 60
        width: 168
        height: 20
        placeholderText: qsTr("Text Field")
    }

    function getcmd()
    {
      return "lukas_kanade_qt";
    }

    function getarg()
    {
      var a = 
      [
        "-l", //load left image
        "-r", //load right image
        "-i", //count iteration (1 by default)
        "-w", //size window search (3px by default)
        "-g", //step for grid (5px by default)
        "-v", //show version
        "-h", //show help
        "-d", //debug mod on
        "-o", outDirField.text,//output directory
      ];

      return a;
    }

    function outdir() 
    {
      return outDirField.text;
    }

}

