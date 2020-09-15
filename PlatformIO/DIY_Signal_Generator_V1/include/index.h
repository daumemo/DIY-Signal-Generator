const char MAIN_page[] PROGMEM = R"=====(
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
@media (max-width:500px) {
	
}
@media (min-width:501px) {

    .key:hover {
        background: whitesmoke;
    }
}
.device-control-main{
    width: 100%;
    height: 150px;
    display: inline-block;
    position: relative;
    border-radius: 8px;
    box-shadow: 0 1px 3px 0 rgba(0, 0, 0, 0.4);
    background-color: white;
}

body{
    background-color: whitesmoke;
}

.keyboard{
    width: 100%;
    height: auto;
    display: inline-block;
    position: relative;
    text-align: center;
}

.key{
    width: 100%;
    float: left;
    height: 100%;
    box-shadow: 0 0px 3px 0 rgba(0, 0, 0, 0.6);
    border-radius: 5px;
    background-color: white;
    color: grey;
}
.key-place-20{
    width: 20%;
    height: 40px;
    float: left;
    margin-bottom: 3px;
    margin-top: 3px;
    box-sizing: border-box;
    padding-left: 3px;
    padding-right: 3px;
}
.key div {
    position: relative;
    left: 50%;
    top: 50%;
    transform: translate(-50%,-50%);
    font-family: Roboto, sans-serif;
    font-size: 20px;
    font-weight:normal;
    color:inherit;
    -webkit-user-select: none; /* Safari */        
    -moz-user-select: none; /* Firefox */
    -ms-user-select: none; /* IE10+/Edge */
}

.pwm {
    position: relative;
    left: 50%;
    top: 50%;
    transform: translate(-50%, 30%);
    font-family: Roboto, sans-serif;
    font-size: 16px;
    font-weight:normal;
    color:inherit;
    -webkit-user-select: none; /* Safari */        
    -moz-user-select: none; /* Firefox */
    -ms-user-select: none; /* IE10+/Edge */
}

.key.num-active {
    color:rgb(210, 150, 0);
    /*background-color: rgb(255, 210, 126);*/
}

.key-place-40{
    width: 40%;
    height: 40px;
    float: left;
    margin-bottom: 3px;
    margin-top: 3px;
    box-sizing: border-box;
    padding-left: 3px;
    padding-right: 3px;
}

.measurement{
    position: absolute;
    font-family: Roboto, sans-serif;
    font-size: 30px;
    font-weight:400;
    color: #a2a2a2;
    -webkit-user-select: none; /* Safari */        
    -moz-user-select: none; /* Firefox */
    -ms-user-select: none; /* IE10+/Edge */
    user-select: none;
}

.text-active {
    color:rgb(210, 150, 0);
}

#frequency {
    top:20%;
    left:70%;
    transform: translate(-100%, -50%);
}

#amplitude {
    top:50%;
    left:70%;
    transform: translate(-100%, -50%);
}

#offset {
    top:80%;
    left:70%;
    transform: translate(-100%, -50%);
}

#hz {
    top:20%;
    left:73%;
    transform: translate(0, -50%);
}

#vamp {
    top:52%;
    left:73%;
    transform: translate(0, -50%);
}

#voff {
    top:82%;
    left:73%;
    transform: translate(0, -50%);
}

.a {
    fill:none;
    stroke:#808080;
    stroke-linecap:round;
    stroke-linejoin:round;
    stroke-width:2.5px;
}

#output-led {
    position:absolute;
    height: 10px;
    width: 10px;
    border-radius: 5px;
    box-shadow: 0 0px 3px 0 rgba(0, 0, 0, 0.6);
    background-color: whitesmoke;
    left: 20px;
    bottom: 20px;
    transform: translate(-50%, 50%);
}
#output-text {
    position:absolute;
    font-size: 15px;
    font-family: Roboto, sans-serif;
    left: 29px;
    bottom: 20px;
    transform: translate(0, 50%);
    color: lightgrey;
}
#output-led.active {
    background-color: rgb(0, 200, 0); 
}
#output-text.active {
    color: rgb(0, 200, 0); 
}
#output-button.active {
    color: rgb(0, 200, 0);
}
.b{
    stroke:rgb(225,225,225);
    stroke-linecap:round;
    stroke-linejoin:round;
    stroke-width:2.5px;
}
.b100{
    fill: #f0f0f0;
}

.b75{
    fill: url(#grad75);
}
.b50{
    fill: url(#grad50);
}
.b25{
    fill: url(#grad25);
}
.b15{
    fill: url(#grad15);
    stroke:#e00000 !important;
}

.c{
    fill:none;
    stroke:rgb(220,220,220);
    stroke-linecap:round;
    stroke-linejoin:round;
    stroke-width:2.5px;
}
#battery-icon {
    position:absolute;
    top: 10px;
    left: 12px;
}
#battery-text{
    position:absolute;
    top: 10px;
    left: 32px;
    font-size: 13px;
    font-family: Roboto, sans-serif;
    color: lightgrey;
}

.battery-low{
    color: #e00000 !important;
}

#output-form-icon {
    position:absolute;
    top: 60px;
    left: 12px;
    color: lightgrey;
}

</style>
</head>
<body>

<div class="device-card">
    
    <div class="device-control-main">
        <div class="measurement" id="frequency">1.000</div> <div class="measurement" id="hz">kHz</div>
        <div class="measurement" id="amplitude">5.000</div> <div class="measurement" id="vamp">V<sub>pp</sub></div>
        <div class="measurement" id="offset">-2.560</div> <div class="measurement"id="voff">V<sub>off</sub></div>
        <div id="output-led"></div> <div id="output-text">OUT</div>
        <div id="battery-icon"></div>
        <div id="battery-text">100%</div>
        <div id="output-form-icon"></div>
    </div>
    <div class="keyboard">
        <div class="key-place-20"><div class="key" onclick="doKeyPress('sine',0)"></div></div>
        <div class="key-place-20"><div class="key" onclick="doKeyPress('triangle',1)"></div></div>
        <div class="key-place-20"><div class="key" onclick="doKeyPress('square',2)"></div></div>
        <div class="key-place-20"><div class="key" onclick="doKeyPress('pwm',3)"><div>PWM</div></div></div>
        <div class="key-place-20"><div class="key"><div>...</div></div></div>

        <div class="key-place-20"><div class="key" onclick="doKeyPress(1, 5)"><div>1</div></div></div>
        <div class="key-place-20"><div class="key" onclick="doKeyPress(2, 6)"><div>2</div></div></div>
        <div class="key-place-20"><div class="key" onclick="doKeyPress(3, 7)"><div>3</div></div></div>
        <div class="key-place-20"><div class="key" onclick="doKeyPress('Hz', 8)"></div></div>
        <div class="key-place-20"><div class="key" onclick="typeInFreq()"><div>Freq</div></div></div>

        <div class="key-place-20"><div class="key" onclick="doKeyPress(4, 10)"><div>4</div></div></div>
        <div class="key-place-20"><div class="key" onclick="doKeyPress(5, 11)"><div>5</div></div></div>
        <div class="key-place-20"><div class="key" onclick="doKeyPress(6, 12)"><div>6</div></div></div>
        <div class="key-place-20"><div class="key" onclick="doKeyPress('kHz', 13)"></div></div>
        <div class="key-place-20"><div class="key" id="amp-duty" onclick="typeInAmplitude()"><div>Amp</div></div></div>

        <div class="key-place-20"><div class="key" onclick="doKeyPress(7, 15)"><div>7</div></div></div>
        <div class="key-place-20"><div class="key" onclick="doKeyPress(8, 16)"><div>8</div></div></div>
        <div class="key-place-20"><div class="key" onclick="doKeyPress(9, 17)"><div>9</div></div></div>
        <div class="key-place-20"><div class="key" onclick="doKeyPress('MHz', 18)"></div></div>
        <div class="key-place-20"><div class="key" id="key-offset" onclick="typeInOffset()"><div>Offs</div></div></div>

        <div class="key-place-20"><div class="key" onclick="doKeyPress('.', 20)"><div>.</div></div></div>
        <div class="key-place-20"><div class="key" onclick="doKeyPress(0, 21)"><div>0</div></div></div>
        <div class="key-place-20"><div class="key" onclick="doKeyPress('-', 22)"><div>&#177;</div></div></div>
        <div class="key-place-40"><div class="key" id="output-button" onclick="toggleOutput()"><div>Output</div></div></div>
        
    </div>
    
</div>


<script>
    var tempFrequency = "";
    var trueFrequency = 10000; //always in 0.1 Herz?
    var shownFrequency = "1.000"; // as typed in by the user;
    var shownFrequencyUnit = "kHz"; // as selected by the user;

    var shownAmplitude = "5.000";
    var shownAmplitudeUnit = "V<sub>pp</sub>";

    var shownDuty = "50.0";
    var shownDutyUnit = "%";

    var shownOffset = "-2.560";
    var shownOffsetUnit = "V<sub>off</sub>";
    var offsetNegative = false;

    var waveForm = "sine";

    var batteryValue = 100;

    var freqIsBeingTypedIn = false;
    var amplitudeIsBeingTypedIn = false;
    var offsetIsBeingTypedIn = false;
    var dutyIsBeingTypedIn = false;
    var freqSetKeyMap = [5,6,7,8,10,11,12,13,15,16,17,18,20,21];
    var lockDot = false; 
    var output = false;

    var updateIsRunning = false;

    var batterySvgText = '<svg width="18px" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 29.59 21"> <defs><linearGradient id="grad15"><stop offset="15%" stop-color=#f00000 /><stop offset="15%" stop-opacity="0" /></linearGradient> <linearGradient id="grad25"><stop offset="25%" stop-color=#f0f0f0 /><stop offset="25%" stop-opacity="0" /></linearGradient> <linearGradient id="grad50"><stop offset="50%" stop-color=#f0f0f0 /><stop offset="50%" stop-opacity="0" /></linearGradient> <linearGradient id="grad75"><stop offset="75%" stop-color=#f0f0f0 /><stop offset="75%" stop-opacity="0" /></linearGradient></defs> <polygon id="battery-poly" class="b" points="1.25 1.25 25.35 1.25 25.35 4.25 28.34 4.25 28.34 16.75 25.35 16.75 25.35 19.75 1.25 19.75 1.25 1.25"/></svg>';
    var sineSvgText = '<svg width="30px" height="20px" style="margin-top:5px;" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 29.5 29.44"><path class="a" d="M7186,7201.28c.59-2.35,2.92-15.24,7-15.24,3.49,0,5,7.45,6.49,13.44,1.64,6.72,2.68,13.49,6.4,13.49,4.53,0,6.61-12.69,7.15-15.42" transform="translate(-7184.77 -7184.79)"/></svg>';
    var triangleSvgText = '<svg width="30px" height="20px" style="margin-top:5px;" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 29.55 26.52"><polyline class="a" points="1.25 13.28 7.84 1.25 21.71 25.27 28.3 13.27"/></svg>';
    var squareSvgText = '<svg width="30px" height="20px" style="margin-top:5px;" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 29.44 26.55"><polyline class="a" points="1.25 13.27 1.25 1.25 14.76 1.25 14.76 25.3 28.19 25.3 28.19 13.27"/></svg>';

    function typeInFreq(){
        //enable numpad for frequency;
        var keys = document.getElementsByClassName("key");
        var freqText = document.getElementById("frequency");
        var freqUnitText = document.getElementById("hz");

        if(freqIsBeingTypedIn){
            //cancel
            for (var i = 0; i<freqSetKeyMap.length; i++) keys[freqSetKeyMap[i]].classList.remove("num-active");
            freqText.classList.remove("text-active");
            freqUnitText.classList.remove("text-active");
            keys[9].classList.remove("num-active");
            freqText.innerHTML = shownFrequency.toFixed(3);
            freqUnitText.innerHTML = shownFrequencyUnit;
            freqIsBeingTypedIn = false;
            emptyUnitKeys();
            startDataChecks();
        }
        else{
            stopDataChecks();
            for (var i = 0; i<freqSetKeyMap.length; i++) keys[freqSetKeyMap[i]].classList.add("num-active");
            freqText.classList.add("text-active");
            freqUnitText.classList.add("text-active");
            keys[9].classList.add("num-active");
            keys[8].innerHTML = "<div>Hz</div>";
            keys[13].innerHTML = "<div>kHz</div>";
            keys[18].innerHTML = "<div>MHz</div>";
            freqText.innerHTML = "-----";
            freqUnitText.innerHTML = "-Hz";
            freqIsBeingTypedIn = true;
        }
    }

    function typeInAmplitude(){
        var keys = document.getElementsByClassName("key");
        var freqText = document.getElementById("amplitude");
        var freqUnitText = document.getElementById("vamp");

        if(amplitudeIsBeingTypedIn){
            //cancel
            for (var i = 0; i<freqSetKeyMap.length; i++) keys[freqSetKeyMap[i]].classList.remove("num-active");
            freqText.classList.remove("text-active");
            freqUnitText.classList.remove("text-active");
            keys[14].classList.remove("num-active");
            freqText.innerHTML = shownAmplitude.toFixed(2);
            freqUnitText.innerHTML = shownAmplitudeUnit;
            amplitudeIsBeingTypedIn = false;
            emptyUnitKeys();
            startDataChecks();
        }
        else{
            stopDataChecks();
            for (var i = 0; i<freqSetKeyMap.length; i++) keys[freqSetKeyMap[i]].classList.add("num-active");
            freqText.classList.add("text-active");
            freqUnitText.classList.add("text-active");
            keys[14].classList.add("num-active");
            keys[8].innerHTML = "<div>V<sub>pp</sub></div>";
            keys[13].innerHTML = "<div></div>";
            keys[18].innerHTML = "<div></div>";
            freqText.innerHTML = "-----";
            freqUnitText.innerHTML = "-V<sub>pp</sub>";
            amplitudeIsBeingTypedIn = true;
        }
    }

    function typeInOffset(){
        var keys = document.getElementsByClassName("key");
        var freqText = document.getElementById("offset");
        var freqUnitText = document.getElementById("voff");

        if(offsetIsBeingTypedIn){
            //cancel
            for (var i = 0; i<freqSetKeyMap.length; i++) keys[freqSetKeyMap[i]].classList.remove("num-active");
            freqText.classList.remove("text-active");
            freqUnitText.classList.remove("text-active");
            keys[19].classList.remove("num-active");
            keys[22].classList.remove("num-active");
            freqText.innerHTML = shownOffset.toFixed(2);
            freqUnitText.innerHTML = shownOffsetUnit;
            offsetIsBeingTypedIn = false;
            emptyUnitKeys();
            startDataChecks();
        }
        else{
            stopDataChecks();
            for (var i = 0; i<freqSetKeyMap.length; i++) keys[freqSetKeyMap[i]].classList.add("num-active");
            freqText.classList.add("text-active");
            freqUnitText.classList.add("text-active");
            keys[19].classList.add("num-active");
            keys[22].classList.add("num-active");
            keys[8].innerHTML = "<div>V</div>";
            keys[13].innerHTML = "<div></div>";
            keys[18].innerHTML = "<div></div>";
            freqText.innerHTML = "-----";
            freqUnitText.innerHTML = "-V<sub>off</sub>";
            offsetIsBeingTypedIn = true;
        }
    }

    function typeInDuty(){
        var keys = document.getElementsByClassName("key");
        var freqText = document.getElementById("amplitude");
        var freqUnitText = document.getElementById("vamp");

        if(dutyIsBeingTypedIn){
            //cancel
            for (var i = 0; i<freqSetKeyMap.length; i++) keys[freqSetKeyMap[i]].classList.remove("num-active");
            freqText.classList.remove("text-active");
            freqUnitText.classList.remove("text-active");
            keys[19].classList.remove("num-active");
            keys[22].classList.remove("num-active");
            freqText.innerHTML = shownDuty.toFixed(1);
            freqUnitText.innerHTML = shownDutyUnit;
            dutyIsBeingTypedIn = false;
            emptyUnitKeys();
            startDataChecks();
        }
        else{
            stopDataChecks();
            for (var i = 0; i<freqSetKeyMap.length; i++) keys[freqSetKeyMap[i]].classList.add("num-active");
            freqText.classList.add("text-active");
            freqUnitText.classList.add("text-active");
            keys[19].classList.add("num-active");
            keys[22].classList.add("num-active");
            keys[8].innerHTML = "<div>%</div>";
            //keys[13].innerHTML = "<div>V</div>";
            //keys[18].innerHTML = "<div></div>";
            freqText.innerHTML = "-----";
            freqUnitText.innerHTML = "--";
            dutyIsBeingTypedIn = true;
        }
    }

    function doKeyPress(value, keynum){

        var freqText = document.getElementById("frequency");
        var freqUnitText = document.getElementById("hz");

        var ampText = document.getElementById("amplitude");
        var ampUnitText = document.getElementById("vamp");

        var offText = document.getElementById("offset");
        var offUnitText = document.getElementById("voff");

        if(freqIsBeingTypedIn){
            if(checkIfKeyIsNum(value)){
                if (freqText.innerHTML.length < 8) {
                    if (freqText.innerHTML == "-----")
                    freqText.innerHTML = value;
                    else freqText.innerHTML += value;
                }
            }
            else if(value == "Hz" || value == "kHz" || value == "MHz"){
                freqUnitText.innerHTML = value;
                finaliseFreqSelection();
            }
            else if (value == "." && !lockDot){
                if (freqText.innerHTML.length < 8) {
                    if (freqText.innerHTML == "-----")
                    freqText.innerHTML = "0" + value;
                    else freqText.innerHTML += value;

                    lockDot = true;
                }
            }
        }

        if(amplitudeIsBeingTypedIn){
            if(checkIfKeyIsNum(value)){
                if (ampText.innerHTML.length < 8) {
                    if (ampText.innerHTML == "-----")
                    ampText.innerHTML = value;
                    else ampText.innerHTML += value;
                }
            }
            else if(value == "Hz"){
                if(value =="Hz") ampUnitText.innerHTML = "V<sub>pp</sub>";
                else ampUnitText.innerHTML = "V<sub>pp</sub>";
                //finaliseFreqSelection();
                finaliseAmpSelection();
            }
            else if (value == "." && !lockDot){
                if (ampText.innerHTML.length < 8) {
                    if (ampText.innerHTML == "-----")
                    ampText.innerHTML = "0" + value;
                    else ampText.innerHTML += value;

                    lockDot = true;
                }
            }
        }

        if(offsetIsBeingTypedIn){
            if(checkIfKeyIsNum(value)){
                if (offText.innerHTML.length < 8) {
                    if (offText.innerHTML == "-----")
                    offText.innerHTML = value;
                    else offText.innerHTML += value;
                }
            }
            else if(value == "Hz" ){
                if(value =="Hz") offUnitText.innerHTML = "V<sub>off</sub>";
                else offUnitText.innerHTML = "V<sub>off</sub>";
                //finaliseFreqSelection();
                finaliseOffSelection();
            }
            else if (value == "." && !lockDot){
                if (offText.innerHTML.length < 8) {
                    if (offText.innerHTML == "-----")
                    offText.innerHTML = "0" + value;
                    else offText.innerHTML += value;

                    lockDot = true;
                }
            }
            else if (value == "-"){
                if (offText.innerHTML == "-----") {
                    offText.innerHTML = "-";
                }
                else {
                    if(offsetNegative){
                        offsetNegative = false;
                        offText.innerHTML = offText.innerHTML.replace("-", "");
                    }
                    else{
                        offsetNegative = true;
                        offText.innerHTML = "-" + offText.innerHTML;
                    }
                }
            }
        }

        if(dutyIsBeingTypedIn){
            if(checkIfKeyIsNum(value)){
                if (ampText.innerHTML.length < 8) {
                    if (ampText.innerHTML == "-----")
                    ampText.innerHTML = value;
                    else ampText.innerHTML += value;
                }
            }
            else if(value == "Hz" || value == "kHz" ){
                if(value =="Hz") ampUnitText.innerHTML = "%";
                else ;
                //finaliseFreqSelection();
                //finaliseAmpSelection();
                finaliseDutySelection();
            }
            else if (value == "." && !lockDot){
                if (ampText.innerHTML.length < 8) {
                    if (ampText.innerHTML == "-----")
                    ampText.innerHTML = "0" + value;
                    else ampText.innerHTML += value;

                    lockDot = true;
                }
            }
        }

        if (value == 'sine'){
            stopDataChecks();
            /*document.getElementById("output-form-icon").innerHTML = sineSvgText.replace('class="a"','class="c"');//need better solution
            document.getElementById("amp-duty").setAttribute("onCLick", "typeInAmplitude()");
            document.getElementById("amp-duty").innerHTML = "<div>Amp</div>";
            document.getElementById("key-offset").setAttribute("onCLick", "typeInOffset()");
            document.getElementById("key-offset").innerHTML = "<div>Offs</div>";
            document.getElementById("amplitude").innerHTML = shownAmplitude.toFixed(2);
            if (shownAmplitudeUnit == "Vpp"){
                document.getElementById("vamp").innerHTML = "V<sub>pp</sub>";
            }
            else {
                document.getElementById("vamp").innerHTML = "V<sub>amp</sub>";
            }
            document.getElementById("offset").innerHTML = shownOffset.toFixed(2);
            document.getElementById("voff").innerHTML = shownOffsetUnit;*/
            setWaveViewToSine();
            waveForm = value;
            sendWaveform();
            startDataChecks();
        }

        if (value == 'triangle'){
            stopDataChecks();
            /*document.getElementById("output-form-icon").innerHTML = triangleSvgText.replace('class="a"','class="c"');//need better solution
            document.getElementById("amp-duty").setAttribute("onCLick", "typeInAmplitude()");
            document.getElementById("amp-duty").innerHTML = "<div>Amp</div>";
            document.getElementById("key-offset").setAttribute("onCLick", "typeInOffset()");
            document.getElementById("key-offset").innerHTML = "<div>Offs</div>";
            document.getElementById("amplitude").innerHTML = shownAmplitude.toFixed(2);
            if (shownAmplitudeUnit == "Vpp"){
                document.getElementById("vamp").innerHTML = "V<sub>pp</sub>";
            }
            else {
                document.getElementById("vamp").innerHTML = "V<sub>amp</sub>";
            }
            document.getElementById("offset").innerHTML = shownOffset.toFixed(2);
            document.getElementById("voff").innerHTML = shownOffsetUnit;*/
            setWaveViewToTriangle();
            waveForm = value;
            sendWaveform();
            startDataChecks();
        }

        if (value == 'square'){
            stopDataChecks();
            /*document.getElementById("output-form-icon").innerHTML = squareSvgText.replace('class="a"','class="c"');//need better solution
            document.getElementById("amp-duty").setAttribute("onCLick", "typeInAmplitude()");
            document.getElementById("amp-duty").innerHTML = "<div>Amp</div>";
            document.getElementById("key-offset").setAttribute("onCLick", "typeInOffset()");
            document.getElementById("key-offset").innerHTML = "<div>Offs</div>";
            document.getElementById("amplitude").innerHTML = shownAmplitude.toFixed(2);
            if (shownAmplitudeUnit == "Vpp"){
                document.getElementById("vamp").innerHTML = "V<sub>pp</sub>";
            }
            else {
                document.getElementById("vamp").innerHTML = "V<sub>amp</sub>";
            }
            document.getElementById("offset").innerHTML = shownOffset.toFixed(2);
            document.getElementById("voff").innerHTML = shownOffsetUnit;*/
            setWaveViewToSquare();
            waveForm = value;
            sendWaveform();
            startDataChecks();
        }

        if (value == 'pwm'){
            stopDataChecks();
            /*document.getElementById("output-form-icon").innerHTML = '<div class="pwm">PWM<div>';
            document.getElementById("amp-duty").setAttribute("onCLick", "typeInDuty()");
            document.getElementById("amp-duty").innerHTML = "<div>Duty</div>";
            document.getElementById("key-offset").setAttribute("onCLick", " ");
            document.getElementById("key-offset").innerHTML = " ";
            document.getElementById("amplitude").innerHTML = shownDuty.toFixed(1);
            document.getElementById("vamp").innerHTML = shownDutyUnit;
            document.getElementById("offset").innerHTML = "";
            document.getElementById("voff").innerHTML = "---";*/
            setWaveViewToPWM();
            waveForm = value;
            sendWaveform();
            startDataChecks();
        }
        //to do: send data to the server;
    }

    function setWaveViewToSine(){
        document.getElementById("output-form-icon").innerHTML = sineSvgText.replace('class="a"','class="c"');//need better solution
        document.getElementById("amp-duty").setAttribute("onCLick", "typeInAmplitude()");
        document.getElementById("amp-duty").innerHTML = "<div>Amp</div>";
        document.getElementById("key-offset").setAttribute("onCLick", "typeInOffset()");
        document.getElementById("key-offset").innerHTML = "<div>Offs</div>";
        document.getElementById("amplitude").innerHTML = shownAmplitude.toFixed(2);
        if (shownAmplitudeUnit == "Vpp"){
            document.getElementById("vamp").innerHTML = "V<sub>pp</sub>";
        }
        else {
            document.getElementById("vamp").innerHTML = "V<sub>amp</sub>";
        }
        document.getElementById("offset").innerHTML = shownOffset.toFixed(2);
        document.getElementById("voff").innerHTML = shownOffsetUnit;
    }

    function setWaveViewToTriangle(){
        document.getElementById("output-form-icon").innerHTML = triangleSvgText.replace('class="a"','class="c"');//need better solution
        document.getElementById("amp-duty").setAttribute("onCLick", "typeInAmplitude()");
        document.getElementById("amp-duty").innerHTML = "<div>Amp</div>";
        document.getElementById("key-offset").setAttribute("onCLick", "typeInOffset()");
        document.getElementById("key-offset").innerHTML = "<div>Offs</div>";
        document.getElementById("amplitude").innerHTML = shownAmplitude.toFixed(2);
        if (shownAmplitudeUnit == "Vpp"){
            document.getElementById("vamp").innerHTML = "V<sub>pp</sub>";
        }
        else {
            document.getElementById("vamp").innerHTML = "V<sub>amp</sub>";
        }
        document.getElementById("offset").innerHTML = shownOffset.toFixed(2);
        document.getElementById("voff").innerHTML = shownOffsetUnit;
    }

    function setWaveViewToSquare(){
        document.getElementById("output-form-icon").innerHTML = squareSvgText.replace('class="a"','class="c"');//need better solution
        document.getElementById("amp-duty").setAttribute("onCLick", "typeInAmplitude()");
        document.getElementById("amp-duty").innerHTML = "<div>Amp</div>";
        document.getElementById("key-offset").setAttribute("onCLick", "typeInOffset()");
        document.getElementById("key-offset").innerHTML = "<div>Offs</div>";
        document.getElementById("amplitude").innerHTML = shownAmplitude.toFixed(2);
        if (shownAmplitudeUnit == "Vpp"){
            document.getElementById("vamp").innerHTML = "V<sub>pp</sub>";
        }
         else {
            document.getElementById("vamp").innerHTML = "V<sub>amp</sub>";
        }
        document.getElementById("offset").innerHTML = shownOffset.toFixed(2);
        document.getElementById("voff").innerHTML = shownOffsetUnit;
    }

    function setWaveViewToPWM(){
        document.getElementById("output-form-icon").innerHTML = '<div class="pwm">PWM<div>';
        document.getElementById("amp-duty").setAttribute("onCLick", "typeInDuty()");
        document.getElementById("amp-duty").innerHTML = "<div>Duty</div>";
        document.getElementById("key-offset").setAttribute("onCLick", " ");
        document.getElementById("key-offset").innerHTML = " ";
        document.getElementById("amplitude").innerHTML = shownDuty.toFixed(1);
        document.getElementById("vamp").innerHTML = shownDutyUnit;
        document.getElementById("offset").innerHTML = "";
        document.getElementById("voff").innerHTML = "---";
    }

    function checkIfKeyIsNum(key){
        return Number.isInteger(key);
    }

    function finaliseFreqSelection() {
        var freqText = document.getElementById("frequency");
        var freqUnitText = document.getElementById("hz");

        shownFrequency = parseFloat(freqText.innerHTML);
        shownFrequencyUnit = freqUnitText.innerHTML;
        freqText = shownFrequency.toFixed(3);
        //do some additional checks whether the value is in range and correct
        //calculate true frequency in hz;
        typeInFreq(); //disables freq selection;
        lockDot = false;
        emptyUnitKeys();
        sendFrequency();
    }

    function finaliseAmpSelection(){
        var freqText = document.getElementById("amplitude");
        var freqUnitText = document.getElementById("vamp");

        shownAmplitude = parseFloat(freqText.innerHTML);
        shownAmplitudeUnit = freqUnitText.innerHTML;
        freqText = shownAmplitude.toFixed(2);
        //do some additional checks whether the value is in range and correct
        //calculate true frequency in hz;
        typeInAmplitude(); //disables freq selection;
        lockDot = false;
        emptyUnitKeys();
        sendAmplitude();
    }

    function finaliseOffSelection(){
        var freqText = document.getElementById("offset");
        var freqUnitText = document.getElementById("voff");

        shownOffset = parseFloat(freqText.innerHTML);
        shownOffsetUnit = freqUnitText.innerHTML;
        freqText = shownOffset.toFixed(2);
        //do some additional checks whether the value is in range and correct
        //calculate true frequency in hz;
        typeInOffset(); //disables freq selection;
        lockDot = false;
        emptyUnitKeys();
        sendOffset();
    }

    function finaliseDutySelection(){
        var freqText = document.getElementById("amplitude");
        var freqUnitText = document.getElementById("vamp");
        shownDuty = parseFloat(freqText.innerHTML);
        shownDutyUnit = freqUnitText.innerHTML;
        freqText.innerHTML = shownDuty.toFixed(1);
        //do some additional checks whether the value is in range and correct
        //calculate true frequency in hz;
        typeInDuty(); //disables freq selection;
        lockDot = false;
        emptyUnitKeys();
        sendDuty();
    }


    function emptyUnitKeys() {
        var keys = document.getElementsByClassName("key");
        keys[8].innerHTML = "<div></div>";
        keys[13].innerHTML = "<div></div>";
        keys[18].innerHTML = "<div></div>";
    }

    function calculateTrueFreq(){
        if(shownFrequencyUnit == "Hz")
        trueFrequency = parseFloat(shownFrequency)*10;
        else if (shownFrequencyUnit == "kHz") trueFrequency = parseFloat(shownFrequency)*10000;
        else if (shownFrequencyUnit == "MHz") trueFrequency = parseFloat(shownFrequency)*10000000;
    }

    function toggleOutput() {
        stopDataChecks();
        var keys = document.getElementsByClassName("key");
        var led = document.getElementById("output-led");
        var outText = document.getElementById("output-text");

        keys[23].classList.toggle("active");
        led.classList.toggle("active");
        outText.classList.toggle("active");
        if(output){
            output = false;
        }
        else{
            output = true;
        }
        sendOutput();
        startDataChecks();
    }

    function setOutputView(){
        var keys = document.getElementsByClassName("key");
        var led = document.getElementById("output-led");
        var outText = document.getElementById("output-text");
        if(output == 0){
            keys[23].classList.remove("active");
            led.classList.remove("active");
            outText.classList.remove("active");
        }
        else {
            keys[23].classList.add("active");
            led.classList.add("active");
            outText.classList.add("active");
        }
    }

    function loadSVGs(){
        var keys = document.getElementsByClassName("key");
        keys[0].innerHTML = "<div>" + sineSvgText + "</div>";
        keys[1].innerHTML = "<div>" + triangleSvgText + "</div>";
        keys[2].innerHTML = "<div>" + squareSvgText + "</div>";
        document.getElementById("battery-icon").innerHTML = batterySvgText;
    }

    function sendWaveform(){
        var xhr = new XMLHttpRequest();
        xhr.open('post', '/waveform');
        xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
        if(waveForm == "sine") xhr.send('value=' + 0);
        else if (waveForm == "triangle") xhr.send('value=' + 1);
        else if (waveForm == "square") xhr.send('value=' + 2);
        else xhr.send('value=' + 4);
    }

    function sendFrequency(){
        var xhr = new XMLHttpRequest();
        xhr.open('post', '/freq');
        xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
        xhr.send('value=' + shownFrequency + '&unit=' + shownFrequencyUnit);
    }

    function sendAmplitude(){
        var xhr = new XMLHttpRequest();
        xhr.open('post', '/ampl');
        xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
        if(shownAmplitudeUnit == "Vpp" || shownAmplitudeUnit == "V<sub>pp</sub>")
        xhr.send('value=' + shownAmplitude + '&unit=' + "Vpp");
        else xhr.send('value=' + shownAmplitude + '&unit=' + "Vamp");
    }

    function sendOffset(){
        var xhr = new XMLHttpRequest();
        xhr.open('post', '/offs');
        xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
        //if(shownOffsetUnit == "Vpp" || shownOffsetUnit == "V<sub>pp</sub>")
        xhr.send('value=' + shownOffset + '&unit=' + "V");
    }

    function sendDuty(){
        var xhr = new XMLHttpRequest();
        xhr.open('post', '/duty');
        xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
        xhr.send('value=' + shownDuty + '&unit=' + shownDutyUnit);
    }

    function sendOutput(){
        var xhr = new XMLHttpRequest();
        xhr.open('post', '/output');
        xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
        if (output){
            xhr.send('value=' + 1);
        }
        else {
            xhr.send('value=' + 0);
        }
        
    }

    function getDataFromServer()
    {
        var xmlhttp = new XMLHttpRequest();
        xmlhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
                if (updateIsRunning) {
                    var myObj = JSON.parse(this.responseText);

                    waveForm = myObj.wform;
                    shownFrequencyUnit = myObj.freqU;
                    if(shownFrequencyUnit == "Hz"){
                        shownFrequency = parseFloat(myObj.freq);
                    }
                    else if (shownFrequencyUnit == "kHz"){
                        shownFrequency = parseFloat(myObj.freq)/1000;
                    }
                    else{
                        shownFrequency = parseFloat(myObj.freq)/1000000;
                    }
                    shownAmplitudeUnit = myObj.amplU;
                    if(shownAmplitudeUnit == "Vpp"){
                        shownAmplitude = parseFloat(myObj.ampl)*2;
                    }
                    else{
                        shownAmplitude = parseFloat(myObj.ampl);
                    }
                    shownOffset = parseFloat(myObj.offset);

                    shownDutyUnit = myObj.dutyU;
                    if(shownDutyUnit == "%"){
                        shownDuty = parseFloat(myObj.duty);
                    }
                    else {
                        shownDuty = parseFloat(myObj.duty)/100;
                    }

                    output = myObj.out;
                    batteryValue = parseInt(myObj.batt);
            
                    updateView();
                }
            
            }
        };
    
    xmlhttp.open("GET", "/data");
    xmlhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    xmlhttp.send();
    }

    function startDataChecks(){
        updateIsRunning = true;
        periodicCheck = setInterval(getDataFromServer, 2500);
    }

    function stopDataChecks(){
        updateIsRunning = false;
        clearInterval(periodicCheck);
    }

    function updateView(){
       document.getElementById("frequency").innerHTML = shownFrequency.toFixed(3);
       document.getElementById("hz").innerHTML = shownFrequencyUnit;
       //doKeyPress(waveForm, 10); //BAD
       if(waveForm == 'sine') setWaveViewToSine();
       else if (waveForm == 'triangle') setWaveViewToTriangle();
       else if (waveForm == 'square') setWaveViewToSquare();
       else if (waveForm == 'pwm') setWaveViewToPWM();
       updateBatteryView();
       setOutputView();
    }

    function updateBatteryView(){
        document.getElementById("battery-text").innerHTML = batteryValue + "%";

        if(batteryValue>89){
            document.getElementById("battery-poly").classList.add("b100");
            document.getElementById("battery-poly").classList.remove("b75");
            document.getElementById("battery-poly").classList.remove("b50");
            document.getElementById("battery-poly").classList.remove("b25");
            document.getElementById("battery-poly").classList.remove("b15");
            document.getElementById("battery-text").classList.remove("battery-low");
        }
        else if (batteryValue>59){
            document.getElementById("battery-poly").classList.remove("b100");
            document.getElementById("battery-poly").classList.add("b75");
            document.getElementById("battery-poly").classList.remove("b50");
            document.getElementById("battery-poly").classList.remove("b25");
            document.getElementById("battery-poly").classList.remove("b15");
            document.getElementById("battery-text").classList.remove("battery-low");
        }
        else if (batteryValue>39){
            document.getElementById("battery-poly").classList.remove("b100");
            document.getElementById("battery-poly").classList.remove("b75");
            document.getElementById("battery-poly").classList.add("b50");
            document.getElementById("battery-poly").classList.remove("b25");
            document.getElementById("battery-poly").classList.remove("b15");
            document.getElementById("battery-text").classList.remove("battery-low");
        }
        else if (batteryValue>15){
            document.getElementById("battery-poly").classList.remove("b100");
            document.getElementById("battery-poly").classList.remove("b75");
            document.getElementById("battery-poly").classList.remove("b50");
            document.getElementById("battery-poly").classList.add("b25");
            document.getElementById("battery-poly").classList.remove("b15");
            document.getElementById("battery-text").classList.remove("battery-low");
        }
        else{
            document.getElementById("battery-poly").classList.remove("b100");
            document.getElementById("battery-poly").classList.remove("b75");
            document.getElementById("battery-poly").classList.remove("b50");
            document.getElementById("battery-poly").classList.remove("b25");
            document.getElementById("battery-poly").classList.add("b15");
            document.getElementById("battery-text").classList.add("battery-low");
        }
        
    }


    loadSVGs();
    updateIsRunning = true;
    getDataFromServer();
    updateIsRunning = false;
    startDataChecks();

</script>

</body>
</html>
)=====";