//requires for the webserver
const path = require('path');//require file system module
const express = require('express');//require express module
const SerialPort = require('serialport');//require serialport module
const onoff = require('onoff');//require onoff gpio module
const nodemailer = require('nodemailer');
var http = require('http');//require http module
var url = require('url');

//inital setup of resources
const port = new SerialPort('/dev/ttyS0',
        { baudRate: 9600 });
var Readline = SerialPort.parsers.Readline;
var isdbOpen = false;
var parser = new Readline();
port.pipe(parser);
const sqlite3 = require('sqlite3').verbose();
var app = express();
app.use(express.static('Public'));
var GPIO = onoff.Gpio;
var heatled = new GPIO(17, 'out');
var lightled = new GPIO(18, 'out');
var datagbl;
var transporter = nodemailer.createTransport({
  service: 'gmail',
  auth: {
    user: 'mattxyz25@gmail.com',
    pass: 'trimatt007'
  }
});

var mailOptions = {
  from: 'mattxyz25@gmail.com',
  to: 'matthew_d_king@outlook.com',
  subject: 'Sending Email using Node.js',
  text: 'Were an online server and were sending data to places !!!'
};

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
port.on('open', function(){
        console.log("port is open");
});

//getting data from the uart connection and storing it in the database
parser.on("data", function(data){
        data = data.trim();
        data = data.split(',');
        let now = new Date();
        let date = require('date-and-time');
        date.locale('en');
        date.format(now, 'YYYY/MM/DD HH:mm:ss');
        datagbl = data
        //writeData(date.format(now, 'YYYY/MM/DD HH:mm:ss'), data[0], data[1]);
});

transporter.sendMail(mailOptions, function(error, info){
  if (error) {
    console.log(error);
  } else {
    console.log('Email sent: ' + info.response);
  }
});

http.createServer(function (req, res) {
try
        {
                console.log('request received');

                var parsedUrl = url.parse(req.url, true); // true to get query as object
                var queryAsObject = parsedUrl.query;

                console.log(JSON.stringify(queryAsObject));
                console.log(JSON.stringify(req.url.substring(2)));

                // pass in location.search to drop the leading ?
                var callingFunc = new URLSearchParams(req.url.substring(2)).get('callback');
                console.log('Calling function: '+ callingFunc);
                var paramA = new URLSearchParams(req.url.substring(2)).get('light'); // this is prefixed with /? so it needs stripping off
                console.log('Parameter light: '+ paramA);
                var paramB = new URLSearchParams(req.url.substring(2)).get('heat');
                console.log('Parameter heat: '+paramB);

                if(callingFunc)
                {
                        // determine which ajax method has called use
                        switch(callingFunc) {
                          case "_brewdata":
                                console.log('request received');
                                var d = new Date();
                                var time = d.toLocaleTimeString();
                                res.writeHead(200, {'Content-Type': 'text/plain'});
                                res.end('_brewdata(\'<h2>Current Brew Status<\h2><p>Time:' + time + '</p><p>Brew Temp:' + datagbl[0] +'C</p><p>Brew Temp: ' + datagbl[1] +'F</p>\')');
                                console.log("response sent");
                          case "_getSwitchState":
                                console.log('called by _getSwitchState');
                                res.writeHead(200, {'Content-Type': 'text/plain'});
                                res.end('_getSwitchState(\'0\')');

                                break;
                          case "_setSwitchState":
                                console.log('called by _setSwitchState');
                                // get the state of paramA and paramB - call to set them on the pipe
                                // query the state and return that state to the calling func
                                if(paramA == 'off')
                                {
                                        lightled.writeSync(0);
                                }else if(paramA == 'on')
                                {
                                        lightled.writeSync(1);
                                }

                                if(paramB == 'off')
                                {
                                        heatled.writeSync(0);
                                }else if(paramB == 'on')
                                {
                                        heatled.writeSync(1);
                                }
                                res.writeHead(200, {'Content-Type': 'text/plain'});
                                res.end('_setSwitchState(\'0\')');

                                break;
                          default:
                                console.log('unexpected method called me');
                        }
                }
        }
        catch(e)
        {
                console.log('failed to write data back to the caller' + e.stack);
        }
}).listen(8124);

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

function LEDoff(LED)
{
        //pass in an led object and then switch the led off
        //and free up GPIO resources
        LED.writeSync(0);
        LED.unexport();
}
