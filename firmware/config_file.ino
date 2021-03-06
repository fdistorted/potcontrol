bool loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);


  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(buf.get());

  if (!root.success()) {
    Serial.println("Failed to parse config file");
    return false;
  }

  const char *pointName = "";
  const char *pointPass = "";

  root.prettyPrintTo(Serial);
  wateringPeriodM = root["wateringPeriodM"];
  wateringLengthS = root["wateringLengthS"];
  startLightH = root["startLightH"];
  startLightM = root["startLightM"];
  endLightH = root["endLightH"];
  endLightM = root["endLightM"];
  SubstratCriticalHumidity = root["SubstratCriticalHumidity"];
  workingMode = root["workingMode"];
  pointName = root["pointName"];
  pointPass = root["pointPass"];
  pointEnc = root["pointEnc"];
  timeZone  = root["timeZone"];
  lastWateringTime  = root["lastWateringTime"];

  Serial.print("workingMode: ");
  Serial.println(workingMode);
  if (workingMode < 1) {
    Serial.println("workingMode bad");
    workingMode = 1;
  }

  ssid = String(pointName);
  pwd = String(pointPass);

  Serial.print("ssid: ");
  Serial.println(pointName);
  Serial.print("pass: ");
  Serial.println(pointPass);

  Serial.print("ssid: ");
  Serial.println(ssid);
  Serial.print("pass: ");
  Serial.println(pwd);
  // Real world application would store these values in some variables for
  // later use.
  return true;
}

bool saveConfig() {

  const char *pointName = ssid.c_str();
  const char *pointPass = pwd.c_str();

  Serial.print("ssid: ");
  Serial.println(pointName);
  Serial.print("pass: ");
  Serial.println(pointPass);



  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["wateringPeriodM"] = wateringPeriodM;
  root["wateringLengthS"] = wateringLengthS;
  root["startLightH"] = startLightH;
  root["startLightM"] = startLightM;
  root["endLightH"] = endLightH;
  root["endLightM"] = endLightM;
  root["SubstratCriticalHumidity"] = SubstratCriticalHumidity;
  root["workingMode"] = workingMode;
  root["pointName"] = pointName;
  root["pointPass"] = pointPass;
  root["pointEnc"] = pointEnc;
  root["timeZone"] = timeZone;
  root["lastWateringTime"] = lastWateringTime;
  root.prettyPrintTo(Serial);
  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  root.printTo(configFile);
  return true;
}
