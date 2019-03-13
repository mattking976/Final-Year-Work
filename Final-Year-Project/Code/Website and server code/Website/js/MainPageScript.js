const checkboxLight = document.getElementById("Lights");
const checkboxHeat = document.getElementById("Heater");

function getData()
{
	var xhttp = new XMLHttpRequest();
	alert("button is clicked");
	xhttp.open("GET", "mockdata.txt", true);
	xhttp.send();
	xhttp.onreadystatechange = function(){
		if (this.readyState == 4 && (this.status == 200 || this.status == 304))
		{
			alert("there was a response");
			document.getElementById("celData").innerHTML = xhttp.responseText;
		}
	};
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
