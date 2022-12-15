function bin2String(array)
{
    
    var myObj = {
        DecodeDataString: "",
        DecodeDataHex: ""
    };

    myObj.DecodeDataString = String.fromCharCode.apply(String, array).substring(0,10)

    return myObj;

}

function bin2HexStr(arr)

{

    var str = "";

    for (var i = 0; i < arr.length; i++)

    {

        if (i != 0)

        {

            str += ", ";

        }

        var tmp = arr[i].toString(16);

        if (tmp.length == 1)

        {

            tmp = "0" + tmp;

        }

        str += "0x";

        str += tmp;

    }

    return str;

}

function Decode(fPort, bytes)

{

    var myObj = {
        "DecodeDataString": "",
        "DecodeDataHex": ""
    };

    var tostring = bin2String(bytes);

    var tosHextring = bin2HexStr(bytes);

    myObj.DecodeDataString = tostring;

    myObj.DecodeDataHex = tosHextring;

    return myObj;

}