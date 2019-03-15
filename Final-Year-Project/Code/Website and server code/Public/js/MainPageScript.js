const checkboxLight = document.getElementById("Lights");
const checkboxHeat = document.getElementById("Heater");

// This function is pointing to localhost for debug. Needs to go to the pi for live
function getData2()
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


checkboxHeat.addEventListener('change', (event) => {
	if(event.target.checked)
	{
		alert("heater is on");
		//send heater on
	}else
	{
		alert("heater is off");
		//send heater off
	}
});

checkboxLight.addEventListener('change', (event) => {
	if(event.target.checked)
	{
		alert("Light is on");
		//send lights on
	}else
	{
		alert("Light is off");
		//send lights off
	}
});
