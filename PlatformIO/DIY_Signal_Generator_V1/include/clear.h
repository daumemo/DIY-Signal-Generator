const char CLEAR_page[] PROGMEM = R"=====(
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
.device-control-extra{
    width: 100%;
    height: 200px;
    display: inline-block;
    position: relative;
    border-radius: 8px;
    box-shadow: 0 1px 3px 0 rgba(0, 0, 0, 0.4);
    background-color: white;
    box-sizing: border-box;
    padding-top: 15px;
}
.name{
    font-family: Roboto, sans-serif;
    font-size: 14px;
    font-weight:normal;
    color: grey;
    -webkit-user-select: none; /* Safari */        
    -moz-user-select: none; /* Firefox */
    -ms-user-select: none; /* IE10+/Edge */
    user-select: none;
    margin-left: 10px;
    margin-right: 10px;
}
body{
    background-color: whitesmoke;
    font-family: Roboto, sans-serif;
}
</style>
</head>
<body>
<div class="device-card">
    <div class="device-control-extra">
        <p class="name">Clearing WiFi SSID and Password...</p>
        <p class="name">Disconecting WiFi...</p>
        <p class="name">Rebooting Device...</p>
        <p class="name" style="color:green;">After reboot the Device will create its own WiFi Access Point. Controls will be available through created WiFi on 192.168.4.1 IP.</p>
    </div>
</div>
</body>
</html>
)=====";