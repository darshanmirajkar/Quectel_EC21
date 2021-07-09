#include "Quectel_EC21.h"

QuectelEC21module::QuectelEC21module()
{
	_Serial = NULL;
}

void QuectelEC21module::SelectSerial(HardwareSerial *theSerial)
{
	_Serial = theSerial;
}

bool QuectelEC21module::hardRestart()
{
	digitalWrite(EC21_RESET, LOW);
	delay(1000);
	digitalWrite(EC21_RESET, HIGH);
}

void QuectelEC21module::begin()
{
	pinMode(EC21_RESET, OUTPUT);
	digitalWrite(EC21_RESET, HIGH);
	_baud = DEFAULT_BAUD_RATE; // Default baud rate 115200
	_Serial->begin(_baud);
	_buffer.reserve(BUFFER_RESERVE_MEMORY);
}

void QuectelEC21module::Retry(uint16_t NumofRetrys, uint16_t RetryDelays)
{
	NumofRetry = NumofRetrys;
	RetryDelay = RetryDelays;
}

void QuectelEC21module::begin(uint32_t baud)
{
	pinMode(EC21_RESET, OUTPUT);
	digitalWrite(EC21_RESET, HIGH);
	_baud = baud;
	_Serial->begin(_baud);
	_buffer.reserve(BUFFER_RESERVE_MEMORY);
}

void QuectelEC21module::begin(uint32_t baud, uint32_t config, int8_t rxPin, int8_t txPin)
{
	pinMode(EC21_RESET, OUTPUT);
	digitalWrite(EC21_RESET, HIGH);
	_baud = baud;
	_Serial->begin(_baud, config, rxPin, txPin);
	_buffer.reserve(BUFFER_RESERVE_MEMORY);
}


void QuectelEC21module::setup()
{
	SelectSerial(LTE_SERIAL_PORT); //Select the serial port
	begin(115200, SERIAL_8N1, LTE_RX_PIN, LTE_TX_PIN);
	initilizeModule();
	Serial.println("\nSearching For network..");
	if (checkforNetwork())
	{
		Serial.println("Network Found");
	}
	else
	{
		Serial.print(".");
	}

	if (enableECHO())
	{
		#if ENABLE_DEBUG
		Serial.println("Echo Enabled");
		#endif
	}

	if (disConnectNetwork())
	{
		#if ENABLE_DEBUG
		Serial.println("Disconnected to internet");
		#endif
	}

	if (setAPN(operaterAPN)
	{
		#if ENABLE_DEBUG
		Serial.println("Set Operater APN");
		#endif
	}

	if (connectNetwork())
	{
		Serial.println("Connected to Internet");
	}else{
		Serial.println("Failed to connect Internet");
	}
}

void QuectelEC21module::basicSetup()
{
	SelectSerial(LTE_SERIAL_PORT); //Select the serial port
	begin(115200, SERIAL_8N1, LTE_RX_PIN, LTE_TX_PIN);
	initilizeModule();
	Serial.println("\nSearching For network..");
	if (checkforNetwork())
	{
		Serial.println("Network Found");
	}
	else
	{
		Serial.print(".");
	}

	if (enableECHO())
	{
		#if ENABLE_DEBUG
		Serial.println("Echo Enabled");
		#endif
	}

}

bool QuectelEC21module::SetAT()
{
	int count = 0;
	do
	{
		_Serial->print(F("at\r\n"));

		_buffer = _readSerial(200);
		count++;
		delay(RetryDelay);
	} while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if ((_buffer.indexOf("OK")) == -1)
		{
			return false;
		}
		else
		{
			Serial.println("AT command Success");
			initilize = true;
			return true;
		}
	}
}

bool QuectelEC21module::enableECHO()
{
	int count = 0;
	do
	{
		_Serial->print(F("ate1\r\n"));

		_buffer = _readSerial(10);
		count++;
		delay(RetryDelay2);
	} while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if ((_buffer.indexOf("OK")) == -1)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

void QuectelEC21module::initilizeModule(){
	if (!SPIFFS.begin(true))
	{
		Serial.println("SPIFFS Mount Failed");
	}
	uint64_t timeOld = millis();
	while (SetAT())
	{
		if(!(millis() > timeOld + SET_AT_TIMEOUT)){
			Serial.print(".");
		}else{
			return;
		}
		
	}
	if(configureModule()){
		Serial.println("\nConfiguration Successfull");
	}else{
		Serial.println("\nConfiguration Failed");
	}
}

bool QuectelEC21module::configureModule()
{
	uint8_t flag1;
	uint8_t flag2;
	uint8_t flag3;
	uint8_t flag4;
	int count = 0;
	do
	{
		_Serial->print(F("AT+CMEE=2\r\n"));
		_buffer = _readSerial(200);
		count++;
		delay(RetryDelay);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			flag1 = 0;
		}
		else
		{
			flag1 = 1;
		}
	}
	count = 0;
	do
	{
		_Serial->print(F("AT+CREG=2\r\n"));
		_buffer = _readSerial(200);
		count++;
		delay(RetryDelay);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			flag2 = 0;
		}
		else
		{
			flag2 = 1;
		}
	}
	count = 0;
	do
	{
		_Serial->print(F("AT+CFUN=0\r\n"));
		_buffer = _readSerial(200);
		count++;
		delay(RetryDelay);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			flag3 = 0;
		}
		else
		{
			flag3 = 1;
		}
	}
	// delay(2000);
	count = 0;
	do
	{
		_Serial->print(F("AT+CFUN=1\r\n"));
		_buffer = _readSerialUntill("+CPIN", 5000);
		count++;
		delay(RetryDelay);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("+CPIN") == -1);
	{
		if (_buffer.indexOf("+CPIN") == -1)
		{
			flag4 = 0;
		}
		else
		{
			flag4 = 1;
		}
		if (flag1 == 0 && flag2 == 0 && flag3 == 0 && flag4 == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

bool QuectelEC21module::checkforNetwork()
{
	int count = 0;
	do
	{
		_Serial->print(F("AT+COPS?\r\n"));
		_buffer = _readSerial(200);
		count++;
		delay(1000);
		if (count > 45)
		{
			Serial.println("\nNetwork Not Found");
			return false;
			// setLTE();
			// hardRestart();
			// delay(10000);
			// ESP.restart();
		}
	}

	while ((count < 50) && _buffer.indexOf("\"") == -1);
	{
		if (_buffer.indexOf("\"") == -1)
		{
			return false;
			;
		}
		else
		{
			return true;
		}
	}
}

bool QuectelEC21module::resetSettings()
{
	int count = 0;
	do
	{
		_Serial->print(F("AT&F\r\n"));
		_buffer = _readSerial(100);
		count++;
		delay(RetryDelay);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			return false;
		}
		else
			return true;
	}
}

int QuectelEC21module::getRssi()
{
	int count = 0;
	String Rssi;
	int rssiDbm;
	do
	{
		_Serial->print(F("AT+CSQ\r\n"));
		_buffer = _readSerial(100);
		count++;
		delay(RetryDelay);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			return false;
		}
		else
			Rssi = midString(_buffer, "+CSQ: ", ",");
		if (int(Rssi.toInt()) <= 30 || int(Rssi.toInt()) >= 2)
		{
			rssiDbm = -(109 - ((int(Rssi.toInt()) - 2) * 2));
		}
		else
		{
			rssiDbm = -51;
		}
		return rssiDbm;
	}
}

String QuectelEC21module::getOperater()
{
	int count = 0;
	String operater;
	do
	{
		_Serial->print(F("AT+COPS?\r\n"));
		_buffer = _readSerial(100);
		count++;
		delay(RetryDelay);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			return "ERROR";
		}
		else
			operater = midString(_buffer, "\"", "\"");
		return operater;
	}
}

String QuectelEC21module::getNetworkType()
{
	int count = 0;
	String operater;
	do
	{
		_Serial->print(F("AT+QNWINFO\r\n"));
		_buffer = _readSerial(100);
		count++;
		delay(RetryDelay);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			return "ERROR";
		}
		else
			operater = midString(_buffer, "\"", "\"");
		return operater;
	}
}

String QuectelEC21module::getBandInfo()
{
	int count = 0;
	String band;
	do
	{
		_Serial->print(F("AT+QNWINFO\r\n"));
		_buffer = _readSerial(100);
		count++;
		delay(RetryDelay);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			return "ERROR";
		}
		else
		{
			band = midStringSecond(_buffer, "\",\"", "\",");
			return band;
		}
	}
}

bool QuectelEC21module::setAPN(const char *apn)
{
	int count = 0;
	do
	{
		_Serial->print(F("AT+QICSGP=1,1,\""));
		_Serial->print(apn);
		_Serial->print(F("\",\"\",\"\",1\r\n"));
		_buffer = _readSerial(10);
		count++;
		delay(RetryDelay2);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

String QuectelEC21module::getIMEI()
{
	String response = "";
	int count = 0;
	do
	{
		_Serial->print(F("AT+CGSN\r\n"));
		_buffer = _readSerial(10);
		count++;
		delay(RetryDelay2);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			return "ERROR";
		}
		else
		{
			response = midString(_buffer, "AT+CGSN\r\r\n", "\r\n\r\nOK");
			return response;
		}
	}
}

String QuectelEC21module::getModelNumber()
{
	String response = "";
	int count = 0;
	do
	{
		_Serial->print(F("AT+CGMM\r\n"));
		_buffer = _readSerial(10);
		count++;
		delay(RetryDelay2);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			return "ERROR";
		}
		else
		{
			response = midString(_buffer, "AT+CGMM\r\r\n", "\r\n\r\nOK");
			return response;
		}
	}
}

String QuectelEC21module::getSerialNumber()
{
	String response = "";
	int count = 0;
	do
	{
		_Serial->print(F("AT+EGMR=0,5\r\n"));
		_buffer = _readSerial(10);
		count++;
		delay(RetryDelay2);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			return "ERROR";
		}
		else
		{
			response = midString(_buffer, "+EGMR: \"", "\"\r\n\r\nOK");
			return response;
		}
	}
}

String QuectelEC21module::getManufacturer()
{
	String response = "";
	int count = 0;
	do
	{
		_Serial->print(F("AT+GMI\r\n"));
		_buffer = _readSerial(10);
		count++;
		delay(RetryDelay2);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			return "ERROR";
		}
		else
		{
			response = midString(_buffer, "AT+GMI\r\r\n", "\r\n\r\nOK");
			return response;
		}
	}
}

String QuectelEC21module::getSimNumber()
{
	String response = "";
	int count = 0;
	do
	{
		_Serial->print(F("AT+CCID\r\n"));
		_buffer = _readSerial(10);
		count++;
		delay(RetryDelay2);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			return "ERROR";
		}
		else
		{
			response = midString(_buffer, "+CCID:", "\r\n\r\nOK");
			return response;
		}
	}
}

bool QuectelEC21module::simPresence()
{
	String response = "";
	int count = 0;
	do
	{
		_Serial->print(F("AT+QSIMSTAT?\r\n"));
		_buffer = _readSerial(10);
		count++;
		delay(RetryDelay2);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			return false;
		}
		else
		{
			response = midString(_buffer, "+QSIMSTAT: 0,", "\r\n\r\nOK");
			if (response.indexOf("0") == -1)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}

void QuectelEC21module::getModuleInfo()
{
	String manufacturarer = getManufacturer();
	String model = getModelNumber();
	String imei = getIMEI();
	String serialNumber = getSerialNumber();
	Serial.println("Manufacturer : " + manufacturarer);
	Serial.println("Model        : " + model);
	Serial.println("IMEI         : " + imei);
	Serial.println("S/N          : " + serialNumber);
}

void QuectelEC21module::getSimInfo()
{
	if (simPresence())
	{
		Serial.println("SIM CARD FOUND");
		String operater = getOperater();
		String techonology = getNetworkType();
		String band = getBandInfo();
		int rssi = getRssi();
		String simNum = getSimNumber();
		
		Serial.println("Operater   : " + operater);
		Serial.println("Technology : " + techonology);
		Serial.println("Band       : " + band);
		Serial.println("RSSI       : " + String(rssi) + " dBm");
		Serial.println("SIM NO     : " + simNum);
	}
	else
	{
		Serial.println("SIM CARD NOT FOUND");
	}
}

bool QuectelEC21module::connectNetwork()
{
	int count = 0;

	do
	{
		_Serial->print(F("AT+QIACT=1\r\n"));
		_buffer = _readSerial(10);
		count++;
		delay(RetryDelay2);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

bool QuectelEC21module::disConnectNetwork()
{
	int count = 0;
	do
	{
		_Serial->print(F("AT+QIDEACT=1\r\n"));
		_buffer = _readSerial(10);
		count++;
		delay(RetryDelay2);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

bool QuectelEC21module::terminateHTTP()
{
	int count = 0;
	do
	{
		_Serial->print(F("AT+QHTTPSTOP\r\n"));

		_buffer = _readSerial(200);
		count++;
		delay(RetryDelay);
	} while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if ((_buffer.indexOf("OK")) == -1)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

bool QuectelEC21module::ping(String url)
{
	int count = 0;
	do
	{
		_Serial->print(F("AT+QPING=1,\""));
		_Serial->print(url);
		_Serial->print(F("\",5,2\r\n"));
		_buffer = _readSerial(2000);
		count++;
		delay(RetryDelay);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("+QPING:") == -1);
	{
		if (_buffer.length() > 100)
		{
			return true;
		}
		else if (_buffer.indexOf("+QPING:") == -1)
		{
			return false;
		}
		else
		{
			return false;
		}
	}
}

size_t QuectelEC21module::getFileSize(char *filename)
{
	int count = 0;
	do
	{
		_Serial->print(F("AT+QFLST\r\n"));
		_buffer = _readSerial(2000);
		count++;
		delay(RetryDelay);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("temp") == -1);
	{
		if (_buffer.indexOf("temp") == -1)
		{
			return false;
		}
		else
		{
			return midString(_buffer, "temp\",", "\r\nOK").toInt();
		}
	}
}

bool QuectelEC21module::deleteFile(String filename)
{
	int count = 0;
	do
	{
		_Serial->print(F("AT+QFDEL=\""));
		_Serial->print(filename);
		_Serial->print(F("\"\r\n"));
		_buffer = _readSerial(1000);
		count++;
		delay(RetryDelay);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

int QuectelEC21module::updateESP(char *md5Checksum)
{
	uint8_t fileHandler = 0;
	uint16_t chunkSize = DOWNLOAD_CHUNK_SIZE;
	uint32_t fileSize = getFileSize("temp");
	uint16_t downloadParts = fileSize / chunkSize;
	uint16_t lastPacket = fileSize % chunkSize;
	int seeksize = 0;
	int count = 0;
	do
	{
		_Serial->print(F("AT+QFOPEN=\"temp\"\r\n"));
		_buffer = _readSerial(100);
		count++;
		delay(RetryDelay);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("+QFOPEN:") == -1);
	{
		if (_buffer.indexOf("+QFOPEN:") == -1)
		{
			Serial.println("File Open Failed!");
			return false;
		}
		else
		{
			fileHandler = midString(_buffer, "+QFOPEN:", "\r\nOK").toInt();
		}
	}
	if (Update.begin(fileSize, U_FLASH))
	{
		Update.setMD5(md5Checksum);
		for (int i = 0; i <= downloadParts; i++)
		{

			count = 0;
			if (i == 0)
			{
				seeksize = 0;
			}
			else
			{
				seeksize = seeksize + chunkSize;
				Serial.print("Download Percentage : ");
				Serial.print((seeksize * 100) / fileSize);
				Serial.println("%");
			}

			if (i == downloadParts)
			{
				chunkSize = lastPacket;
			}
			do
			{
				_Serial->print(F("AT+QFSEEK="));
				_Serial->print(fileHandler);
				_Serial->print(F(","));
				_Serial->print(seeksize);
				_Serial->print(F(","));
				if (i == 0)
				{
					_Serial->print(F("0\r\n"));
				}
				else
				{
					_Serial->print(F("0\r\n"));
				}
				_buffer = _readSerial(100);
				count++;
				delay(RetryDelay);
			}

			while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
			{
				if (_buffer.indexOf("OK") == -1)
				{
					Serial.println("Seek Failed");
				}
			}

			count = 0;
			do
			{
				_Serial->print(F("AT+QFREAD="));
				_Serial->print(fileHandler);
				_Serial->print(F(","));
				_Serial->print(chunkSize);
				_Serial->print(F("\r\n"));
				_buffer = storeFile(chunkSize, 100);
				count++;
				delay(50);
			}

			while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
			{
				if (_buffer.indexOf("OK") == -1)
				{
					Serial.println("Read Failed");
				}
			}
		}
	}
	count = 0;
	do
	{
		_Serial->print(F("AT+QFCLOSE="));
		_Serial->print(fileHandler);
		_Serial->print(F("\r\n"));
		_buffer = _readSerial(500);
		count++;
		delay(RetryDelay);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			Serial.println("File Close Failed");
		}
		else
		{
			deleteFile("temp");
			if (Update.end())
			{
				Serial.println("Flashing ... done!");
				delay(100);
				Serial.println("Rebooting ESP");
				ESP.restart();
				return true;
			}
			else
			{
				Serial.println("Flashing Failed");
			}
		}
	}
}

int QuectelEC21module::readFile(const char *filename)
{
	uint8_t fileHandler = 0;
	uint16_t chunkSize = DOWNLOAD_CHUNK_SIZE;
	uint32_t fileSize = getFileSize("temp");
	uint16_t downloadParts = fileSize / chunkSize;
	uint16_t lastPacket = fileSize % chunkSize;
	int seeksize = 0;
	int count = 0;
	do
	{
		_Serial->print(F("AT+QFOPEN=\"temp\"\r\n"));
		_buffer = _readSerial(100);
		count++;
		delay(RetryDelay);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("+QFOPEN:") == -1);
	{
		if (_buffer.indexOf("+QFOPEN:") == -1)
		{
			Serial.println("File Open Failed!");
			return false;
		}
		else
		{
			fileHandler = midString(_buffer, "+QFOPEN:", "\r\nOK").toInt();
		}
	}
	File f = SPIFFS.open(filename, "w");
	for (int i = 0; i <= downloadParts; i++)
	{

		count = 0;
		if (i == 0)
		{
			seeksize = 0;
		}
		else
		{
			seeksize = seeksize + chunkSize;
			Serial.print("Download Percentage : ");
			Serial.print((seeksize * 100) / fileSize);
			Serial.println("%");
		}

		if (i == downloadParts)
		{
			chunkSize = lastPacket;
		}
		do
		{
			_Serial->print(F("AT+QFSEEK="));
			_Serial->print(fileHandler);
			_Serial->print(F(","));
			_Serial->print(seeksize);
			_Serial->print(F(","));
			if (i == 0)
			{
				_Serial->print(F("0\r\n"));
			}
			else
			{
				_Serial->print(F("0\r\n"));
			}
			_buffer = _readSerial(100);
			count++;
			delay(RetryDelay);
		}

		while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
		{
			if (_buffer.indexOf("OK") == -1)
			{
				Serial.println("Seek Failed");
			}
		}

		count = 0;
		do
		{
			_Serial->print(F("AT+QFREAD="));
			_Serial->print(fileHandler);
			_Serial->print(F(","));
			_Serial->print(chunkSize);
			_Serial->print(F("\r\n"));
			_buffer = storeFile(f, chunkSize, 100);
			count++;
			delay(RetryDelay);
		}

		while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
		{
			if (_buffer.indexOf("OK") == -1)
			{
				Serial.println("Read Failed");
			}
		}
	}
	f.close();
	count = 0;
	do
	{
		_Serial->print(F("AT+QFCLOSE="));
		_Serial->print(fileHandler);
		_Serial->print(F("\r\n"));
		_buffer = _readSerial(500);
		count++;
		delay(RetryDelay);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			Serial.println("File Close Failed");
		}
		else
		{
			deleteFile("temp");
			Serial.println("Download Successfull");
			Serial.println(filename);
			Serial.println("Downaload CheckSum : " + getMD5checksum(filename));
			return true;
		}
	}
}

bool QuectelEC21module::PostHTTP(const char *URL, const char *message)
{
	uint8_t flag1;
	uint8_t flag2;
	uint8_t flag3;
	uint8_t flag4;
	int count = 0;
	do
	{
		_Serial->print(F("AT+QHTTPCFG=\"contenttype\",1\r\n"));
		_buffer = _readSerial(100);
		count++;
		delay(RetryDelay);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			flag1 = 0;
		}
		else
		{
			flag1 = 1;
		}
	}
	// delay(100);

	count = 0;
	do
	{
		delay(RetryDelay);
		_Serial->print(F("AT+QHTTPURL=\""));
		_Serial->print(URL);
		_Serial->print(F("\"\r\n"));
		_buffer = _readSerial(100);
		count++;
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			flag2 = 0;
		}
		else
		{
			flag2 = 1;
		}
	}
	// delay(100);
	count = 0;

	do
	{
		_Serial->print(F("AT+QHTTPPOST="));
		_Serial->print(strlen(message));
		_Serial->print(F(",15,15"));
		_Serial->print(F("\r\n"));
		_buffer = _readSerial(1000);
		count++;
	} while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("CONNECT") == -1);
	{
		if (_buffer.indexOf("CONNECT") == -1)
		{
			flag3 = 0;
		}
		else
		{
			flag3 = 1;
		}
	}
	// delay(100);
	count = 0;

	do
	{
		delay(RetryDelay);
		_Serial->print(message + String("\r\n"));
		_buffer = _readSerial(100);
		count++;

	} while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("200") == -1);
	{
		if (_buffer.indexOf("200") == -1)
		{
			flag4 = 0;
		}
		else
		{
			flag4 = 1;
		}
		if (flag1 == 1 && flag2 == 1 && flag3 == 1 && flag4 == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	// delay(100);
}

bool QuectelEC21module::GetHTTP(const char *URL, const char *message)
{
	uint8_t flag1;
	uint8_t flag2;
	uint8_t flag3;
	uint8_t flag4;
	int count = 0;
	do
	{
		_Serial->print(F("AT+QHTTPCFG=\"contenttype\",1\r\n"));
		_buffer = _readSerial(100);
		count++;
		delay(RetryDelay);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			flag1 = 0;
		}
		else
		{
			flag1 = 1;
		}
	}
	// delay(100);

	count = 0;
	do
	{
		delay(RetryDelay);
		_Serial->print(F("AT+QHTTPURL=\""));
		_Serial->print(URL);
		_Serial->print(F("\"\r\n"));
		_buffer = _readSerial(100);
		count++;
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			flag2 = 0;
		}
		else
		{
			flag2 = 1;
		}
	}
	// delay(100);
	count = 0;

	do
	{
		_Serial->print(F("AT+QHTTPGET="));
		// _Serial->print(strlen(message));
		_Serial->print(F("80"));
		_Serial->print(F("\r\n"));
		_buffer = _readSerial(1000);
		count++;
	} while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("CONNECT") == -1);
	{
		if (_buffer.indexOf("CONNECT") == -1)
		{
			flag3 = 0;
		}
		else
		{
			flag3 = 1;
		}
	}
	// delay(100);
	count = 0;

	do
	{
		delay(RetryDelay);
		_Serial->print(message + String("\r\n"));
		_buffer = _readSerial(100);
		count++;

	} while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("200") == -1);
	{
		if (_buffer.indexOf("200") == -1)
		{
			flag4 = 0;
		}
		else
		{
			flag4 = 1;
		}
		if (flag1 == 1 && flag2 == 1 && flag3 == 1 && flag4 == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	// delay(100);
}

bool QuectelEC21module::downloadUpdate(const char *URL, char *md5Checksum)
{
	uint8_t flag1;
	uint8_t flag2;
	uint8_t flag3;
	uint8_t flag4;
	size_t size = 0;
	int count = 0;
	do
	{
		_Serial->print(F("AT+QHTTPCFG=\"contenttype\",2\r\n"));
		_buffer = _readSerial(100);
		count++;
		delay(RetryDelay);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			flag1 = 0;
		}
		else
		{
			flag1 = 1;
		}
	}
	// delay(100);

	count = 0;
	do
	{
		delay(RetryDelay);
		_Serial->print(F("AT+QHTTPURL=\""));
		_Serial->print(URL);
		_Serial->print(F("\"\r\n"));
		_buffer = _readSerial(100);
		count++;
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			flag2 = 0;
		}
		else
		{
			flag2 = 1;
		}
	}
	// delay(100);
	count = 0;

	do
	{
		_Serial->print(F("AT+QHTTPGET="));
		// _Serial->print(strlen(message));
		_Serial->print(F("100"));
		_Serial->print(F("\r\n"));
		// _buffer = _readSerial(2000);
		_buffer = _readSerialUntill("200", 3000);
		count++;
	} while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("200") == -1);
	{
		if (_buffer.indexOf("200") == -1)
		{
			flag3 = 0;
		}
		else
		{
			flag3 = 1;
		}
	}
	do
	{
		_Serial->print(F("AT+QHTTPREADFILE=\"temp\",300\r\n"));
		_buffer = _readSerialUntill("+QHTTPREADFILE: 0\r\n", 120 * 1000);
		count++;
	} while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("+QHTTPREADFILE: 0\r\n") == -1);
	{
		if (_buffer.indexOf("+QHTTPREADFILE: 0\r\n") == -1)
		{
			flag4 = 0;
		}
		else
		{
			flag4 = 1;
			updateESP(md5Checksum);
		}
	}
	// delay(100);

	if (flag1 == 1 && flag2 == 1 && flag3 == 1 && flag4 == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool QuectelEC21module::downloadFile(const char *URL, const char *filename)
{
	uint8_t flag1;
	uint8_t flag2;
	uint8_t flag3;
	uint8_t flag4;
	size_t size = 0;
	int count = 0;
	do
	{
		_Serial->print(F("AT+QHTTPCFG=\"contenttype\",2\r\n"));
		_buffer = _readSerial(100);
		count++;
		delay(RetryDelay);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			flag1 = 0;
		}
		else
		{
			flag1 = 1;
		}
	}
	// delay(100);

	count = 0;
	do
	{
		delay(RetryDelay);
		_Serial->print(F("AT+QHTTPURL=\""));
		_Serial->print(URL);
		_Serial->print(F("\"\r\n"));
		_buffer = _readSerial(100);
		count++;
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			flag2 = 0;
		}
		else
		{
			flag2 = 1;
		}
	}
	// delay(100);
	count = 0;

	do
	{
		_Serial->print(F("AT+QHTTPGET="));
		// _Serial->print(strlen(message));
		_Serial->print(F("100"));
		_Serial->print(F("\r\n"));
		// _buffer = _readSerial(2000);
		_buffer = _readSerialUntill("200", 3000);
		count++;
	} while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("200") == -1);
	{
		if (_buffer.indexOf("200") == -1)
		{
			flag3 = 0;
		}
		else
		{
			flag3 = 1;
		}
	}
	do
	{
		_Serial->print(F("AT+QHTTPREADFILE=\"temp\",300\r\n"));
		_buffer = _readSerialUntill("+QHTTPREADFILE: 0\r\n", 120 * 1000);
		count++;
	} while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("+QHTTPREADFILE: 0\r\n") == -1);
	{
		if (_buffer.indexOf("+QHTTPREADFILE: 0\r\n") == -1)
		{
			flag4 = 0;
		}
		else
		{
			flag4 = 1;
			readFile(filename);
		}
	}
	// delay(100);

	if (flag1 == 1 && flag2 == 1 && flag3 == 1 && flag4 == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool QuectelEC21module::PutHTTP(const char *URL, const char *message)
{
	uint8_t flag1;
	uint8_t flag2;
	uint8_t flag3;
	uint8_t flag4;
	int count = 0;
	do
	{
		_Serial->print(F("AT+QHTTPCFG=\"contenttype\",1\r\n"));
		_buffer = _readSerial(100);
		count++;
		delay(RetryDelay);
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			flag1 = 0;
		}
		else
		{
			flag1 = 1;
		}
	}
	// delay(100);

	count = 0;
	do
	{
		delay(RetryDelay);
		_Serial->print(F("AT+QHTTPURL=\""));
		_Serial->print(URL);
		_Serial->print(F("\"\r\n"));
		_buffer = _readSerial(100);
		count++;
	}

	while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			flag2 = 0;
		}
		else
		{
			flag2 = 1;
		}
	}
	// delay(100);
	count = 0;

	do
	{
		_Serial->print(F("AT+QHTTPPUT="));
		_Serial->print(strlen(message));
		_Serial->print(F(",15,15"));
		_Serial->print(F("\r\n"));
		_buffer = _readSerial(1000);
		count++;
	} while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("CONNECT") == -1);
	{
		if (_buffer.indexOf("CONNECT") == -1)
		{
			flag3 = 0;
		}
		else
		{
			flag3 = 1;
		}
	}
	// delay(100);
	count = 0;

	do
	{
		delay(RetryDelay);
		_Serial->print(message + String("\r\n"));
		_buffer = _readSerial(100);
		count++;

	} while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("200") == -1);
	{
		if (_buffer.indexOf("200") == -1)
		{
			flag4 = 0;
		}
		else
		{
			flag4 = 1;
		}
		if (flag1 == 1 && flag2 == 1 && flag3 == 1 && flag4 == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	// delay(100);
}

String QuectelEC21module::HTTPread()
{
	int count = 0;
	String response = "";
	do
	{
		_Serial->print(F("AT+QHTTPREAD"));
		_Serial->print(F("\r\n"));
		_buffer = _readSerial(100);
		count++;
		delay(RetryDelay);
	} while ((count < NumofRetry && count < MAX_Count) && _buffer.indexOf("OK") == -1);
	{
		if (_buffer.indexOf("OK") == -1)
		{
			response = "";
		}
		else
		{
			response = midString(_buffer, "CONNECT\r\n", "\r\nOK");
		}
	}
	return response;
}

String QuectelEC21module::SerialRead()
{
	_timeout = 0;
	while (!_Serial->available() && _timeout < 12000)
	{
		delay(13);
		_timeout++;
	}
	if (_Serial->available())
	{
		return _Serial->readString();
	}
}

//
////
//PRIVATE METHODS
//
String QuectelEC21module::_readSerial()
{
	uint64_t timeOld = millis();
	while (!_Serial->available() || !(millis() > timeOld + TIME_OUT_READ_SERIAL))
	{
	}
	String str = "";
	if (_Serial->available() > 0)
	{
		str += (char)_Serial->read();
	}
	#if ENABLE_DEBUG
		Serial.println(str);
	#endif
	return str;
}

String QuectelEC21module::_readSerial(uint32_t timeout)
{
	uint64_t timeOld = millis();
	while (!_Serial->available() && !(millis() > timeOld + timeout))
	{
		
	}
	String str = "";

	if (_Serial->available() > 0)
	{
		str = _Serial->readString();
	}
	#if ENABLE_DEBUG
		Serial.println(str);
	#endif
	return str;
}

String QuectelEC21module::_readSerialUntill(String buff, uint32_t timeout)
{
	uint64_t timeOld = millis();
	String str = "";
	while (_Serial->available() > 0 || !(millis() > timeOld + timeout))
	{
		Serial.print(".");
		str = _Serial->readString();
		if (str.indexOf(buff) == -1)
		{
		}
		else
		{
			break;
		}
	}
	#if ENABLE_DEBUG
		Serial.println(str);
	#endif
	return str;
}

String QuectelEC21module::midString(String str, String start, String finish)
{
	int locStart = str.indexOf(start);
	if (locStart == -1)
		return "";
	locStart += start.length();
	int locFinish = str.indexOf(finish, locStart);
	if (locFinish == -1)
		return "";
	return str.substring(locStart, locFinish);
}

String QuectelEC21module::midStringSecond(String str, String start, String finish)
{
	int locStart = str.indexOf(start);
	if (locStart == -1)
		return "";
	locStart = locStart + 1;
	if (locStart == -1)
		return "";
	int temp = str.indexOf(start, locStart);
	locStart = temp + start.length();
	if (locStart == -1)
		return "";
	int locFinish = str.indexOf(finish, locStart);
	if (locFinish == -1)
		return "";
	return (str.substring(locStart, locFinish));
}

String QuectelEC21module::file_md5(File &f)
{
	if (!f)
	{
		return String();
	}

	if (f.seek(0, SeekSet))
	{

		MD5Builder md5;
		md5.begin();
		md5.addStream(f, f.size());
		md5.calculate();
		return md5.toString();
	}
	return String();
}

String QuectelEC21module::getMD5checksum(const char *path)
{
	File c = SPIFFS.open(path);
	String md5str = file_md5(c);
	return md5str;
}

String QuectelEC21module::storeFile(uint16_t rxBuffersize, uint32_t timeout)
{
	uint64_t timeOld = millis();
	String str = "";
	uint8_t disB = 0;
	uint8_t disL = 6;
	uint8_t addB = 0;
	if (numberOfDigits(rxBuffersize) == 5)
	{
		disB = 34;
	}
	else if (numberOfDigits(rxBuffersize) == 4)
	{
		disB = 32;
	}
	else if (numberOfDigits(rxBuffersize) == 3)
	{
		disB = 30;
	}
	else if (numberOfDigits(rxBuffersize) == 2)
	{
		disB = 28;
	}
	else
	{
		disB = 26;
	}
	addB = disB + disL + 1;
	_Serial->setRxBufferSize(rxBuffersize + addB);
	delay(1000);
	size_t size;

	int count = 0;
	size = _Serial->available();

	#if ENABLE_DEBUG > 1
		Serial.println("Size : "+String(size));
	#endif
	

	while (_Serial->available() || !(millis() > timeOld + timeout))
	{
		if (count > disB && count < (size - disL))
		{
			uint8_t c = _Serial->read();
			Update.write(&c, 1);
		}
		else
		{
			str += (char)_Serial->read();
		}
		if (count > size)
		{
			break;
		}
		count++;
	}
	#if ENABLE_DEBUG > 1
		Serial.println(str);
	#endif
	return str;
}

String QuectelEC21module::storeFile(File f, uint16_t rxBuffersize, uint32_t timeout)
{
	uint64_t timeOld = millis();
	String str = "";
	uint8_t disB = 0;
	uint8_t disL = 6;
	uint8_t addB = 0;
	if (numberOfDigits(rxBuffersize) == 5)
	{
		disB = 34;
	}
	else if (numberOfDigits(rxBuffersize) == 4)
	{
		disB = 32;
	}
	else if (numberOfDigits(rxBuffersize) == 3)
	{
		disB = 30;
	}
	else if (numberOfDigits(rxBuffersize) == 2)
	{
		disB = 28;
	}
	else
	{
		disB = 26;
	}
	addB = disB + disL + 1;
	_Serial->setRxBufferSize(rxBuffersize + addB);
	delay(1000);
	size_t size;

	int count = 0;
	size = _Serial->available();

	#if ENABLE_DEBUG > 1
		Serial.println("Size : "+String(size));
	#endif

	while (_Serial->available() || !(millis() > timeOld + timeout))
	{
		if (count > disB && count < (size - disL))
		{
			f.write(_Serial->read());
		}
		else
		{
			str += (char)_Serial->read();
		}
		if (count > size)
		{
			break;
		}
		count++;
	}
	#if ENABLE_DEBUG > 1
		Serial.println(str);
	#endif
	return str;
}

String QuectelEC21module::convertToString(char *a, int size)
{
	int i;
	String s = "";
	for (i = 0; i < size; i++)
	{
		s = s + a[i];
	}
	return s;
}

int QuectelEC21module::numberOfDigits(uint16_t n)
{
	int count = 0;
	while (n != 0)
	{
		n /= 10; // n = n/10
		++count;
	}
	return count;
}
