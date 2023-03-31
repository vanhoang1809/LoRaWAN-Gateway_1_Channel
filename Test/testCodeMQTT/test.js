function Decode(fport,bytes)
{
    
    var myObj = {
        field1: "",
        field2: ""
    };

    myObj.field1 = String.fromCharCode.apply(null, bytes).substring(0,5)
	myObj.field2 = String.fromCharCode.apply(null, bytes).substring(6,12)
    return myObj;

}