//requires for the webserver
const path = require('path');//require file system module
const express = require('express');//require express module
//const SerialPort = require('serialport');//require serialport module

//inital setup of resources
//const port = new SerialPort('/dev/ttyS0',{ baudRate: 9600 });
//var Readline = SerialPort.parsers.Readline;
var isdbOpen = false;
//var parser = new Readline();
//port.pipe(parser);
const sqlite3 = require('sqlite3').verbose();
var app = express();
app.use(express.static('Public'));
//onoff test code

//when a connection is established with the server send the index page
app.get('Public/', function(req, res){
	res.sendFile(path.join(__dirname + 'index.html'));
});

//allowing the use of static features
app.use('/css', express.static(__dirname + 'Public/css'))
app.use('/js', express.static(__dirname + 'Public/js'))

//opening the webserver
var server = app.listen(8082, function(){
	var port = server.address().port;
	console.log("server has started at http://locahost:%s", port);
});

//opening the uart connection
//port.on('open', function(){	console.log("port is open");});

//getting data from the uart connection and storing it in the database
//parser.on("data", function(data){
	//data = data.trim();
	//data = data.split(',');
	//let now = new Date();
	//let date = require('date-and-time');
	//date.locale('en');
	//date.format(now, 'YYYY/MM/DD HH:mm:ss');
	//writeData(date.format(now, 'YYYY/MM/DD HH:mm:ss'), data[0], data[1]);
//});

//function to write data to the database
function writeData(dateTime, tempC, tempF)
{
	if (isdbOpen == false)
	{
		isdbOpen = true;
		conn = new sqlite3.Database('sensorData.db', sqlite3.OPENREADWRITE);
        	conn.run("INSERT INTO Treadings values((?), (?), (?))", [dateTime, tempC, tempF], function(err, result){
			if(err) throw err;
		});
		conn.close(function()
		{
			isdbOpen = false;
		});
	}else
	{
		console.log("error database is in use");
	}
}

var http = require('http');

http.createServer(function (req, res) {
try
	{
		console.log('request received');
		var d = new Date();
		var time = d.toLocaleTimeString();    
		res.writeHead(200, {'Content-Type': 'text/plain'});
		res.end('_brewdata(\'<h2>Current Brew Status<\h2><p>Time:' + time + '</p><p>Brew Temp: 27C</p>\')');
		console.log('response written' + '<h2>Current Brew Status<\h2><p>Time:' + time + '</p><p>Brew Temp: 27C</p>');
	}
	catch
	{
		console.log('failed to write data back to the caller');
	}
}).listen(8124);




