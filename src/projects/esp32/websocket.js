let webSocketServer = require('websocket').server; 
//npm install websocket
//node websocket.js
let http = require('http');

const port = 3000;
const hostname = '192.168.1.9';

let server = http.createServer((req, res) => {
    res.statusCode = 200;
    res.setHeader('Content-type', 'text/plain');
    res.end('Hello world from node server@tads');
});

server.listen(port, hostname, () => {
    console.log(`Server ready at port: ${port}`);
});

let wsServer = new webSocketServer({
    httpServer: server
});

//callback to process client requests
wsServer.on('request', (request) => {
    let state = false;
    let client = request.accept(null, request.origin);

    client.on('message', (message) => {
        if (message.type == 'utf8') {
            console.log(message.utf8Data);
        }
    });

    let interval = setInterval(() => {
        client.sendUTF(state? "on": "off");
        state = !state;
    }, 1000);

    client.on('close', () => {
        console.log('connection closed');
        clearInterval(interval);
    });
});
