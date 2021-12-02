/* arduino.library manager
Installed library - ArduinoWebsockets
https://github.com/gilmaimon/ArduinoWebsockets/blob/master/README.md
*/

/* O cliente estabelece uma conexão WebSocket por meio de um 
processo conhecido como handshake do WebSocket. 
Esse processo começa com o cliente enviando uma solicitação HTTP
 regular para o servidor. Um cabeçalho de Upgrade é incluído neste pedido que 
 informa ao servidor que o cliente deseja estabelecer uma conexão WebSocket.
*/

/*
Agora que o handshake está concluído, a conexão HTTP inicial é substituída por uma conexão WebSocket que usa a
mesma conexão TCP/IP subjacente. Neste momento, qualquer uma das partes pode começar a enviar dados.
Com WebSockets, você pode transferir quantos dados quiser sem incorrer na sobrecarga associada às 
solicitações HTTP tradicionais. Os dados são transferidos por meio de um WebSocket como mensagens , 
cada uma das quais consiste em um ou mais frames (quadros) contendo os dados que você está enviando (a carga útil).
 Para garantir que a mensagem possa ser adequadamente reconstruída ao atingir o cliente, 
 cada quadro é prefixado com 4 a 12 bytes de dados sobre a carga útil. 
 O uso desse sistema de mensagens baseado em quadros ajuda a reduzir a quantidade de dados 
 que são transferidos, levando a reduções significativas na latência.
*/

//https://jimb-cc.medium.com/esp-and-mongodb-sitting-in-a-tree-7d043fb1a4d
//https://github.com/jimb-cc/houseplant/blob/master/src/main.cpp
//https://gitter.im/espressif/arduino-esp32?at=5bf2bc631ad4155d3aeb8c10

#include <ArduinoWebsockets.h>
#include <WiFi.h>
//#include <SocketIOclient.h>

const char* ssid = "servos_superior";
const char* password = "discipulo";

using namespace websockets; //to use WebsocketsClient

WebsocketsClient client;
const int ledPin = 2;

//server - client (esp32)

void onMessageCallback(WebsocketsMessage message) {
    Serial.println(message.data());
    if (message.data().equalsIgnoreCase("on")) digitalWrite(ledPin, HIGH);
    else if (message.data().equalsIgnoreCase("off")) digitalWrite(ledPin, LOW);
}

void setup() {
    Serial.begin(115200);
    //delay(5000);
    pinMode(ledPin, OUTPUT);
    WiFi.begin(ssid, password);
    //WiFi.mode(WIFI_MODE_STA);
    Serial.print("IP address: ");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("Wifi is now connected with PI");
    Serial.println(WiFi.localIP());
    
    client.setCACert(ssl_ca_cert);
    //bool connected = client.connect(websockets_server_host, websockets_server_port, "/");
    bool connected = client.connect("http://192.168.1.9:3000/");
    Serial.println(connected);
    if (connected) {
        Serial.println("Connected");
        client.send("Hello server");
    } else {
        Serial.println("Not connected");
        return;
    }
    client.send("Hello server from client ESP32 reading sensor readings...");
    client.onMessage(onMessageCallback);
}

void loop() {
    if (client.available()) {
        client.poll();
    }
    delay(500);
}
