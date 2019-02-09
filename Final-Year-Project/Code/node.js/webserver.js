//requires for the webserver and serialport
var http = require('http').createServer(handler);//require http server and create a server with function handler
var fs = require('fs');//require file system module

http.listen(8082);//listen to port 8082
console.log("server is running");//writing to the conole that the server is running

//http error handler
function handler (req, res){ //create server
        fs.readFile(__dirname + '/Public/index.html', function(err, data){//open the web page index.html
                if(err) {//if there is an error
                                res.writeHead(404, {'Content-Type': 'text/html'});//write in the header
                                return res.end("404 Not Found");//return the error code
                }
                else{
                        res.writeHead(200, {'Content-Type': 'text/html'});//else open the web page
                        res.write(data);
                }
                return res.end();
        });
}