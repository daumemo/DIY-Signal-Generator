const char SETUP_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
.device-card{
    width: 300px;
    height: auto;
    display: inline-block;
    left: 50%;
    top: 50%;
    position: absolute;
    transform: translate(-50%, -50%);
    margin-bottom: 5px;
}

::placeholder {
  color: lightgray;
  opacity: 1; /* Firefox */
}

:-ms-input-placeholder { /* Internet Explorer 10-11 */
 color: lightgray;
}

::-ms-input-placeholder { /* Microsoft Edge */
 color: lightgray;
}

.device-control-extra{
    width: 100%;
    height: 48px;
    display: inline-block;
    position: relative;
    border-radius: 8px;
    box-shadow: 0 1px 3px 0 rgba(0, 0, 0, 0.4);
    text-align: center;
    background-color: white;
    box-sizing: border-box;
}
.inputs{
    width: 100%;
    height: 48px;
    display: inline-block;
    position: relative;
    border-radius: 8px;
    padding-left: 20px;
    background-color: white;
    box-sizing: border-box;
    margin-top:4px;
    border: 1px solid #ccc;
    font-size: 15px;
    color: grey;
}
.submits{
    width: 100%;
    height: 48px;
    display: inline-block;
    position: relative;
    border-radius: 8px;
    text-align: center;
    background-color: rgb(255, 210, 126);
    border: 1px solid #ccc;
    box-shadow: 0 1px 3px 0 rgba(0, 0, 0, 0.4);
    color: white;
    box-sizing: border-box;
    margin-top:4px;
    font-size: 15px;
    font-weight: bold;
    cursor: pointer;
}

.name{
    position: absolute;
    top:50%;
    left:50%;
    transform: translate(-50%, -50%);
    font-family: Roboto, sans-serif;
    font-size: 20px;
    font-weight:normal;
    color: grey;
    -webkit-user-select: none; /* Safari */        
    -moz-user-select: none; /* Firefox */
    -ms-user-select: none; /* IE10+/Edge */
    user-select: none;
}

body{
    background-color: whitesmoke;
    font-family: Roboto, sans-serif;
}

.hidden{
    display:none;
}

</style>
</head>
<body>

<div class="device-card">
    <div class="device-control-extra">
        <div class="name">Device Setup</div>
    </div>
    <form action='setting' method='POST'>
        <select  id="ssid_select" name="ssid_select" class="inputs" required="required" onchange="checkSSID(this.value)">
            <option value selected disabled>Select SSID</option>
            <option value="insert_custom_ssid" style="color: rgb(255, 210, 126);">Custom SSID</option>
          </select>
        <input id="ssid" name='ssid' placeholder='SSID' length=32 class="inputs hidden">
        <input name='pass' placeholder='Password' length=64 class="inputs">
        <input type='submit' value="SAVE" class=submits>
    </form>
</div>


<script>
    function getDataFromServer()
    {
        var xmlhttp = new XMLHttpRequest();
        xmlhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
            var myObj = JSON.parse(this.responseText);
            for(var i = 0; i< myObj.SSID.length; i++){
                var option = document.createElement("option");
		        option.text = myObj.SSID[i] +"(" + myObj.RSSI[i] + ")" + ((myObj.Protected[i] == 0)?" ":"*");
		        option.value = myObj.SSID[i];
    	        document.getElementById("ssid_select").appendChild(option);
                }
            }
    };
    
    xmlhttp.open("GET", "/wifissids");
    xmlhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    xmlhttp.send();
    }

    function checkSSID(name){
        if(name == "insert_custom_ssid"){
            document.getElementById("ssid").classList.remove("hidden");
            document.getElementById("ssid").value = "";
        }
        else {
            document.getElementById("ssid").classList.add("hidden");
            document.getElementById("ssid").value = name;
        }
    }

    getDataFromServer();
</script>
</body>
</html>
)=====";