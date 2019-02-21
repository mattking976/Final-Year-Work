//requires for the webserver
var path = require('path');//require file system module
var express = require('express');
var app = express();
const SerialPort = require('serialport');
const port = new SerialPort('/dev/ttyS0',
	{ baudRate: 9600 });
const sqlite3 = require('sqlite3').verbose();
app.use(express.static('Public'));

app.get('Public/', function(req, res){
	res.sendFile(path.join(__dirname + 'index.html'));
});

//allowing the use of static features
app.use('/css', express.static(__dirname + 'Public/css'))
app.use('/js', express.static(__dirname + 'Public/js'))

var server = app.listen(8082, function(){
	var port = server.address().port;
	console.log("server has started at http://locahost:%s", port);
});

let db = new sqlite3.Database('sensordata.db', sqlite3.OPEN_READWRITE, (err) => {
	if(err)
	{
		console.log(err.message);
	}
	else{
		console.log('connected to local database');
	}
});

db.close();

port.on('open', function(){
	console.log("port is open");
});

port.on("data", function(data){
	port.on("readable", function(data){
		if(port.read() != null)
		{
			console.log(port.read());
		}
	});
});
