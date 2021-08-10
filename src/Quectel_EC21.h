
#ifndef QUECTEL_EC21_h
#define QUECTEL_EC21_h
#include "Arduino.h"
#include "SPIFFS.h"
#include "MD5Builder.h"

#include <functional>
#include "Update.h"

#define BUFFER_RESERVE_MEMORY 1024 * 10
#define DEFAULT_BAUD_RATE 115200
#define TIME_OUT_READ_SERIAL 8000
#define DOWNLOAD_CHUNK_SIZE 10240
#define SET_AT_TIMEOUT 20000 //in ms
#define ENABLE_DEBUG 0  // 0 - Disable, 1 - level1 , 2 - level2

#define EC21_RESET 12

#define LTE_RX_PIN 5
#define LTE_TX_PIN 2
#define LTE_SERIAL_PORT &Serial1

//#define Macro(x)  (((x) > (10)) ? (abc()) : 2)
class QuectelEC21module
{
private:
  int NumofRetry = 3;
  // int NumofCustonRetry[5] = {1,2,3,4,5};
  int RetryDelay = 50; //in ms
  int RetryDelay2 = 1; //in ms
  int MAX_Count = 10;
  uint32_t _baud;
  String _buffer;
  bool _sleepMode;
  String _readSerial();
  String _readSerial(uint32_t timeout);
  String _readSerialUntill(String buff, uint32_t timeout);
  String storeFile(uint16_t rxBuffersize, uint32_t timeout);
  String storeFile(File f, uint16_t rxBuffersize, uint32_t timeout);
  String midString(String str, String start, String finish);
  String midStringSecond(String str, String start, String finish);
  String file_md5(File &f);
  String convertToString(char *a, int size);
  int numberOfDigits(uint16_t n);
  bool deleteFile(String filename);
  HardwareSerial *_Serial;

public:
  QuectelEC21module();
  const char* operaterAPN = "airtelgprs.com";
  const char* operaterUsername = "";
  const char* operaterPassword = "";
  uint8_t operaterAuth = 3;
  const char* operaterPUK = "";
  int _timeout;
  bool initilize = false;
  void SelectSerial(HardwareSerial *theSerial);
  void begin(); //Default baud 115200
  void begin(uint32_t baud);
  void begin(uint32_t baud, uint32_t config, int8_t rxPin, int8_t txPin);
  void setup();
  void basicSetup();
  bool SetAT();
  bool enableECHO();
  void enable();
  void disable();
  void initilizeModule();
  bool configureModule();
  bool checkforNetwork();
  bool initateHTTP();
  bool Restart();
  bool hardRestart();
  bool httpContentType(uint8_t type);
  uint16_t PostHTTP(String URL, String message, int type);
  uint16_t GetHTTP(String URL);
  uint16_t PutHTTP(String URL, String message, int type);
  bool addHeader(String name, String value);
  String HTTPread();
  bool setAPN(const char *apn, const char* username, const char *password, int auth);
  bool connectNetwork();
  bool disConnectNetwork();
  String getIPAddress();
  bool getCurrentTime(String url);
  bool ping(String url);
  bool downloadFile(const char *URL, const char *filename);
  int downloadUpdate(const char *URL, char *md5Checksum);
  String SerialRead();
  void Retry(uint16_t NumofRetrys, uint16_t RetryDelays);

  size_t getFileSize(char *filename);
  int readFile(const char *filename);
  int updateESP(char *md5Checksum);
  bool resetSettings();
  /**Module Info**/
  String getIMEI();
  String getSerialNumber();
  String getManufacturer();
  String getModelNumber();
  void getModuleInfo();

  String manufacturarer;
	String model;
	String imei;
	String serialNumber;

  /**SIM Info**/
  String getSimNumber();
  bool simPresence();
  int getRssi();
  String getOperater();
  String getNetworkType();
  String getBandInfo();
  void getSimInfo();

  String operater;
  String techonology;
  String band;
  int rssi;
  String simNum;

  String getMD5checksum(const char *path);
};
extern QuectelEC21module EC21module;
#endif






