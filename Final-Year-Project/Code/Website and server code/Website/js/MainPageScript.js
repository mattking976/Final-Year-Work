//the code that the button runs when clicked
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

// This function is pointing to localhost for debug. Needs to go to the pi for live
function getData2()
{
    $.ajax({
        url: 'http://127.0.0.1:8124/',
        dataType: "jsonp",
        jsonpCallback: "_brewdata",
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



