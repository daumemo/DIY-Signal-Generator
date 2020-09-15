const char RESET_page[] PROGMEM = R"=====(
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
    font-size: 17px;
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
        <div class="name">Choose Action:</div>
    </div>
    <div class="submits" style="padding-top:13px; margin-top:15px;" onclick="location.href='/reboot';">Reboot Device</div>
    <div class="submits" style="padding-top:13px; margin-top:15px;" onclick="location.href='/clearmem';">Forget WiFi and Reboot</div>
</div>
</body>
</html>
)=====";