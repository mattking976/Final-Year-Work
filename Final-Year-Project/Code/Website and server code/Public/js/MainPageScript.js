const checkboxLight = document.getElementById("Lights");
const checkboxHeat = document.getElementById("Heater");
const heatOff = 'off';
const heatOn = 'on';
const lightOff = 'off';
const lightOn = 'on';
var currentHeat;
var currentLight;

$(document).ready(function readyFn(){
        update();
        getSwitchState();
        setSwitchState();
});

checkboxHeat.addEventListener('change', (event) => {
        if(event.target.checked)
        {
                currentHeat = heatOn;
                setSwitchStateP(currentLight, heatOn);
                //send heater on
        }else
        {
                currentHeat = heatOff;
                setSwitchStateP(currentLight, heatOff);
                //send heater off
        }
});

checkboxLight.addEventListener('change', (event) => {
        if(event.target.checked)
        {
                currentLight = lightOn;
                setSwitchStateP(lightOn, currentHeat);
                //send lights on
        }else
        {
                currentLight = lightOff;
                setSwitchStateP(lightOff, currentHeat);
                //send lights off
        }
});

function update()
{
        $.ajax({
        url: 'http://10.108.200.109:8124/',
        dataType: "jsonp",
        jsonpCallback: "_brewdata",
        cache: false,
        timeout: 5000,
        success: function(data) {
            $("#LiveData").html(data);
            window.setTimeout(update, 10000);
        },
        error: function(jqXHR, textStatus, errorThrown) {
            alert('error ' + textStatus + " " + errorThrown);
        }
    });

}

function getData()
{
    $.ajax({
        url: 'http://10.108.200.109:8124/',
        dataType: "jsonp",
        jsonpCallback: "_brewdata",
        cache: false,
        timeout: 5000,
        success: function(data) {
            $("#LiveData").html(data);
        },
        error: function(jqXHR, textStatus, errorThrown) {
            alert('error ' + textStatus + " " + errorThrown);
        }
    });
}

function getSwitchState()
{
    $.ajax({
        url: 'http://10.108.200.109:8124/?light=off&heat=off',
        dataType: "jsonp",
        jsonpCallback: "_getSwitchState",
        cache: false,
        timeout: 5000,
        success: function(data) {
            $("#test").html(data);
        },
        error: function(jqXHR, textStatus, errorThrown) {
            alert('error ' + textStatus + " " + errorThrown);
        }
    });
};

function setSwitchState()
{
    $.ajax({
        url: 'http://10.108.200.109:8124/?light=off&heat=off',
        dataType: "jsonp",
        jsonpCallback: "_setSwitchState",
        cache: false,
        timeout: 5000,
        success: function(data) {
            $("#test").html(data);
        },
        error: function(jqXHR, textStatus, errorThrown) {
            alert('error ' + textStatus + " " + errorThrown);
        }
    });
};

function setSwitchStateP(lightState, heatState)
{
    $.ajax({
        url: 'http://10.108.200.109:8124/?light='+ lightState +'&heat=' + heatState,
        dataType: "jsonp",
        jsonpCallback: "_setSwitchState",
        cache: false,
        timeout: 5000,
        success: function(data) {
            $("#test").html(data);
        },
        error: function(jqXHR, textStatus, errorThrown) {
            alert('error ' + textStatus + " " + errorThrown);
        }
    });
};
