void setRGBLED(int R, int G, int B) {
  //preventing overflow
  R = (R > 1023) ? (1023) : (R);
  R = (R < 0) ? (0) : (R);
  G = (G > 1023) ? (1023) : (G);
  G = (G < 0) ? (0) : (G);
  B = (B > 1023) ? (1023) : (B);
  B = (B < 0) ? (0) : (B);
  glo_R = R;
  glo_G = G;
  glo_B = B;
  Serial.print("H = ");
  Serial.println(getH(R, G, B));
}

/* Converts a color from HSV to RGB.
 * h is hue, as a number between 0 and 360.
 * s is the saturation, as a number between 0 and 255.
 * v is the value, as a number between 0 and 255. */
void setHSV(uint16_t h, uint8_t s, uint8_t v){
    uint8_t f = (h % 60) * 255 / 60;
    uint8_t p = (255 - s) * (uint16_t)v / 255;
    uint8_t q = (255 - f * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t t = (255 - (255 - f) * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t r = 0, g = 0, b = 0;
    switch((h / 60) % 6){
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }
    glo_R = r*4;
    glo_G = g*4;
    glo_B = b*4;
    return;
}

int getH(int r, int g, int b) {
    double rd = (double) r/1023;
    double gd = (double) g/1023;
    double bd = (double) b/1023;
    double maxd = max(rd, max(gd, bd)), mind = min(rd, min(gd, bd));
    double h, s, v = maxd;

    double d = maxd - mind;
    s = (maxd==0) ? 0 : (d/maxd);

    if (maxd == mind) { 
        h = 0; // achromatic
    } else {
        if (maxd == rd) {
            h = (gd - bd) / d + (gd < bd ? 6 : 0);
        } else if (maxd == gd) {
            h = (bd - rd) / d + 2;
        } else if (maxd == bd) {
            h = (rd - gd) / d + 4;
        }
        h *= 60;
    }

    return h;
}

void MQTT_reconnect() {
  char temp[128];
  char _mac[32];

  const char* online = "Device online";
  // Loop until we're reconnected
  while (!MQTT_client.connected()) {
    Serial.print("Attempting MQTT connection...ID=");
    Serial.println(randomID);
    // Attempt to connect
    if (MQTT_client.connect(randomID)) {
      Serial.println("connected");
      
      // Once connected, publish an announcement...
      ((String)randomID + "/log").toCharArray(temp, 128);
      MQTT_client.publish(temp, online);
      MQTT_client.publish(temp, _mac);
      // ... and resubscribe
      for (int i = 0; i < numOfChannels; i++) {
        ((String)randomID + (String)channel[i]).toCharArray(temp, 128);
        MQTT_client.subscribe(temp);
      }
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(MQTT_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
int hexString2int(String obj) {
  int num = 0;
  for (int i = 0; i < obj.length(); i++) {
    num *= 16;
    if (((obj[i] - '0') >= 0) && ((obj[i] - '0') < 10))
      num += obj[i] - '0';
    else if (((obj[i] - 'A') >= 0) && ((obj[i] - 'A') < 6))
      num += obj[i] - 'A' + 10;
    else
      num += 0;
  }
  return num;
}

