//   ESP32 RGB Control - RGB LED control with ESP32 using WebServer.
//   Copyright (C) 2024  Kerem Biçen

//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.

//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.

//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <WiFi.h>
#include <WebServer.h>

#define RED 15
#define GREEN 4
#define BLUE 5

int r = 0, g = 0, b = 0;

const char* ssid = "ssid";
const char* password = "password";

WebServer server(80);

void setup()
{
    Serial.begin(115200);
    
    ledcAttachPin(RED, 1);
    ledcAttachPin(GREEN, 2);
    ledcAttachPin(BLUE, 3);

    ledcSetup(1, 5000, 8);
    ledcSetup(2, 5000, 8);
    ledcSetup(3, 5000, 8);

    ledcWrite(1, 255);
    ledcWrite(2, 255);
    ledcWrite(3, 255);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
        delay(1000);
    
    Serial.println(WiFi.localIP());

    server.on("/", handle_index);

    server.begin();
}

void loop()
{
    server.handleClient();
}

void handle_index()
{
    if (server.args() == 3)
    {
        r = server.arg(0).toInt();
        g = server.arg(1).toInt();
        b = server.arg(2).toInt();

        ledcWrite(1, 255 - r);
        ledcWrite(2, 255 - g);
        ledcWrite(3, 255 - b);
    }

    String document = "<html>\n<head>\n<title>RGB Control</title>\n<script>\nfunction change_color() {\nwindow.location.href = `${window.location.href.split(\"?\")[0]}?r=${document.getElementsByTagName(\"input\")[0].value}&g=${document.getElementsByTagName(\"input\")[1].value}&b=${document.getElementsByTagName(\"input\")[2].value}`;\n}\n</script>\n</head>\n<body>\n";
    document += "<input type=\"number\" min=\"0\" max=\"255\" value=\"" + String(r) + "\">\n";
    document += "<input type=\"number\" min=\"0\" max=\"255\" value=\"" + String(g) + "\">\n";
    document += "<input type=\"number\" min=\"0\" max=\"255\" value=\"" + String(b) + "\">\n";
    document += "<button onclick=\"change_color()\">Change Color</button>\n</body>\n</html>";

    server.send(200, "text/html", document);
}