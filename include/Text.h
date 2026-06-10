// #ifndef TEXT_H
// #define TEXT_H

// #include "LED/Colors.h"
// #include "LED/Matrix.h"
// #include "LED/Text.h"

// // Function to handle deferred text display from main loop
// void handleText() {
//   if (textRequested) {
//     if (strcmp(textAnimation, "static") == 0) {
//       staticText(textContent);
//     } else if (strcmp(textAnimation, "wave") == 0) {
//       waveText(textContent);
//     } else if (strcmp(textAnimation, "scroll") == 0) {
//       scrollText(textContent);
//     } else if (strcmp(textAnimation, "display") == 0) {
//       displayOrScrollText(textContent);
//     }
//     textRequested = false;
//   }
// }

// void startText(ESPWiFi *device) {
//   device->initWebServer();
//   device->webServer->addHandler(new AsyncCallbackJsonWebHandler(
//       "/text", [device](AsyncWebServerRequest *request, JsonVariant &json) {
//         if (json.isNull() || !json.is<JsonObject>()) {
//           AsyncWebServerResponse *response = request->beginResponse(
//               400, "application/json", "{\"error\":\"EmptyInput\"}");
//           device->addCORS(response);
//           request->send(response);
//           device->logError("/text Error parsing JSON: EmptyInput");
//           return;
//         }

//         JsonObject jsonObject = json.as<JsonObject>();
//         JsonObject textObj = jsonObject["text"];
//         if (textObj.isNull()) {
//           AsyncWebServerResponse *response = request->beginResponse(
//               400, "application/json", "{\"error\":\"NoTextObject\"}");
//           device->addCORS(response);
//           request->send(response);
//           device->logError("/text Error: No text object in JSON");
//           return;
//         }

//         String content = textObj["content"] | "";
//         content = "     " + content;
//         String animation = textObj["animation"] | "scroll";
//         int speed = textObj["speed"] | 75;

//         if (content.length() == 0) {
//           AsyncWebServerResponse *response = request->beginResponse(
//               400, "application/json", "{\"error\":\"NoContent\"}");
//           device->addCORS(response);
//           request->send(response);
//           device->logError("/text Error: No content provided");
//           return;
//         }

//         // Copy to char arrays
//         strncpy(textContent, content.c_str(), espTextMaxLen);
//         textContent[espTextMaxLen] = '\0';
//         strncpy(textAnimation, animation.c_str(), 15);
//         textAnimation[15] = '\0';
//         textSpeed = speed;

//         // Defer all text display to main loop to avoid conflicts
//         textRequested = true;

//         String responseStr;
//         serializeJson(textObj, responseStr);
//         AsyncWebServerResponse *response =
//             request->beginResponse(200, "application/json", responseStr);
//         device->addCORS(response);
//         request->send(response);
//       }));
// }

// #endif