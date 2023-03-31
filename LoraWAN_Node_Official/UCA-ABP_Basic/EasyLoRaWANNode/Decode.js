// The function must return an object, e.g. {"temperature": 22.5}
function hex_to_ascii(str1)
 {
	var hex  = str1.toString();
	var str = '';
	for (var n = 0; n < hex.length; n += 2) {
		str += String.fromCharCode(parseInt(hex.substr(n, 2), 16));
	}
	return str;
 }

function toHexString(bytes) {
    return bytes.map(function(byte) {
        return ("00" + (byte & 0xFF).toString(16)).slice(-2)
      }).join('')
}
function Decode(fPort, bytes) {

    var tohex = toHexString(bytes);
	var toascii = hex_to_ascii(tohex);
  
    return {"mydata": toascii};

}
function bin2String(array)

{

    return String.fromCharCode.apply(String, array).substring(0,10);

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