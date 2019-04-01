#pragma once
#include <iostream>
#include <windows.h>
#include <WinUser.h>
#include <WinBase.h>
#include <tchar.h>
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib,"wininet.lib")
#pragma comment(lib, "Advapi32.lib")
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <WinInet.h>
#include <iomanip>
#include <cstdio>  


std::string GetHwUID()
{
	HW_PROFILE_INFO hwProfileInfo;
	std::string szHwProfileGuid = "";

	if (GetCurrentHwProfileA(&hwProfileInfo) != NULL)
		szHwProfileGuid = hwProfileInfo.szHwProfileGuid;

	return szHwProfileGuid;
}
std::string GetCompUserName(bool User)
{
	std::string CompUserName = "";

	char szCompName[MAX_COMPUTERNAME_LENGTH + 1];
	char szUserName[MAX_COMPUTERNAME_LENGTH + 1];

	DWORD dwCompSize = sizeof(szCompName);
	DWORD dwUserSize = sizeof(szUserName);

	if (GetComputerNameA(szCompName, &dwCompSize))
	{
		CompUserName = szCompName;

		if (User && GetUserNameA(szUserName, &dwUserSize))
		{
			CompUserName = szUserName;
		}
	}

	return CompUserName;
}
std::string StringToHex(const std::string input)
{
	const char* lut = "0123456789ABCDEF";
	size_t len = input.length();
	std::string output = "";

	output.reserve(2 * len);

	for (size_t i = 0; i < len; i++)
	{
		const unsigned char c = input[i];
		output.push_back(lut[c >> 4]);
		output.push_back(lut[c & 15]);
	}

	return output;
}
DWORD GetVolumeID()
{
	DWORD VolumeSerialNumber;

	BOOL GetVolumeInformationFlag = GetVolumeInformationA(
		"c:\\",
		0,
		0,
		&VolumeSerialNumber,
		0,
		0,
		0,
		0
	);

	if (GetVolumeInformationFlag)
		return VolumeSerialNumber;

	return 0;
}

std::string GetSerialKey()
{
	std::string SerialKey = "61A345B5496B2";
	std::string CompName = GetCompUserName(false);
	std::string UserName = GetCompUserName(true);

	SerialKey.append(StringToHex(GetHwUID()));
	SerialKey.append("-");
	SerialKey.append(StringToHex(std::to_string(GetVolumeID())));
	SerialKey.append("-");
	SerialKey.append(StringToHex(CompName));
	SerialKey.append("-");
	SerialKey.append(StringToHex(UserName));

	return SerialKey;
}
bool SpectroFobia = true;
bool Revenger = false;
bool Diss = false;
std::string GetHashText(const void * data, const size_t data_size)
{
	HCRYPTPROV hProv = NULL;

	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
	{
		return "";
	}

	BOOL hash_ok = FALSE;
	HCRYPTPROV hHash = NULL;

	hash_ok = CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash);

	if (!hash_ok)
	{
		CryptReleaseContext(hProv, 0);
		return "";
	}

	if (!CryptHashData(hHash, static_cast<const BYTE *>(data), data_size, 0))
	{
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		return "";
	}

	DWORD cbHashSize = 0, dwCount = sizeof(DWORD);
	if (!CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE *)&cbHashSize, &dwCount, 0))
	{
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		return "";
	}

	std::vector<BYTE> buffer(cbHashSize);

	if (!CryptGetHashParam(hHash, HP_HASHVAL, reinterpret_cast<BYTE*>(&buffer[0]), &cbHashSize, 0))
	{
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		return "";
	}

	std::ostringstream oss;

	for (std::vector<BYTE>::const_iterator iter = buffer.begin(); iter != buffer.end(); ++iter)
	{
		oss.fill('0');
		oss.width(2);
		oss << std::hex << static_cast<const int>(*iter);
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);
	return oss.str();
}

std::string GetHashSerialKey()
{
	std::string SerialKey = GetSerialKey();
	const void* pData = SerialKey.c_str();
	size_t Size = SerialKey.size();
	std::string Hash = GetHashText(pData, Size);

	for (auto& c : Hash)
	{
		if (c >= 'a' && c <= 'f')
		{
			c = '4';
		}
		else if (c == 'b')
		{
			c = '5';
		}
		else if (c == 'c')
		{
			c = '6';
		}
		else if (c == 'd')
		{
			c = '7';
		}
		else if (c == 'e')
		{
			c = '8';
		}
		else if (c == 'f')
		{
			c = '9';
		}

		c = toupper(c);
	}

	return Hash;
}
std::string GetSerial()
{
	std::string Serial = "";
	std::string HashSerialKey = GetHashSerialKey();

	std::string Serial1 = HashSerialKey.substr(0, 4);
	std::string Serial2 = HashSerialKey.substr(4, 4);
	std::string Serial3 = HashSerialKey.substr(8, 4);
	std::string Serial4 = HashSerialKey.substr(12, 4);

	Serial += Serial1;
	Serial += '-';
	Serial += Serial2;
	Serial += '-';
	Serial += Serial3;
	Serial += '-';
	Serial += Serial4;

	return Serial;
}
std::string valid[] =
{
	("2844-6146-3444-4455"),//QUADRO
	("0514-4544-5494-4744"),//KENT
	("1964-4447-9574-4446"),
	("7149-4444-4548-4404"),
	("7247-3998-3665-4446"),
	//("0226-4663-6044-8114"),//("6878-0406-0447-2948"), Baka 
	("4456-7444-7471-6144"),
	("3462-4244-4364-3964"),
	("7417-5634-4437-3147"),
	("9744-6470-5482-5644"),//("4744-4004-5444-8446"),//LEHA SELLER
	("0649-3483-1441-8404"),
	("3464-8814-7469-4182"),
	("2847-4146-0694-4397"),
	("7444-4501-6444-0427"),//	("1442-4984-4242-2449"), // Alexander Fatianov
	("0024-4184-9444-2415"),
	("3214-4240-1444-4714"),
	("3744-4446-1444-4644"),
	("0344-4487-2743-3410"),
	("2449-9748-4144-4400"),
	("1414-8414-5541-4443"),//KIZARU
	("4240-4464-4444-4984"),//ASDASD
	("6418-1234-4247-4462"),//Миша Гутняк
	("4404-4943-8434-4642"),// EU USER XENA
	("4444-7843-9430-8747"),//Second EU USER Dodu110
	("5474-6864-4454-7342"),
	("0984-4844-4372-1420"),//maxim
	("4780-4494-4844-9334"),
	("4194-9340-2406-2617"),
};
char* user_names[] =
{
	"6348-1414-9448-6644",
	"0514-4544-5494-4744",
	"1964-4447-9574-4446",
	"4240-4464-4444-4984",
	"7149-4444-4548-4404",
	"7247-3998-3665-4446",
	"6878-0406-0447-2948",
	"4456-7444-7471-6144",
	"3462-4244-4364-3964",
	"7417-5634-4437-3147",
};
/*
bool CheckLicense()
{
for (int i = 0; i < sizeof(valid); i++)
{
if (valid[i] == GetSerial())
{
return true;
}
}
return false;
}*/
#define PATH "/WebPanel/"
#define HOST ("webpanel.ru")
#define HOST2 ("webpanel.ru")
static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";
std::string base64_encode(char const* bytes_to_encode, unsigned int in_len)
{
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--)
	{
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i <4); i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while ((i++ < 3))
			ret += '=';

	}

	return ret;
}

std::string GetSerial64()
{
	std::string Serial = GetSerial();
	Serial = base64_encode(Serial.c_str(), Serial.size());
	return Serial;
}

std::string GetUrlData(std::string url)
{
	std::string request_data = "";

	HINTERNET hIntSession = InternetOpenA("", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	if (!hIntSession)
	{
		return request_data;
	}

	HINTERNET hHttpSession = InternetConnectA(hIntSession, HOST, 80, 0, 0, INTERNET_SERVICE_HTTP, 0, NULL);

	if (!hHttpSession)
	{
		return request_data;
	}

	HINTERNET hHttpRequest = HttpOpenRequestA(hHttpSession, "GET", url.c_str()
		, 0, 0, 0, INTERNET_FLAG_RELOAD, 0);

	if (!hHttpSession)
	{
		return request_data;
	}

	char* szHeaders = ("Content-Type: text/html\r\nUser-Agent: License");
	char szRequest[1024] = { 0 };

	if (!HttpSendRequestA(hHttpRequest, szHeaders, strlen(szHeaders), szRequest, strlen(szRequest)))
	{
		return request_data;
	}

	CHAR szBuffer[1024] = { 0 };
	DWORD dwRead = 0;

	while (InternetReadFile(hHttpRequest, szBuffer, sizeof(szBuffer) - 1, &dwRead) && dwRead)
	{
		request_data.append(szBuffer, dwRead);
	}

	InternetCloseHandle(hHttpRequest);
	InternetCloseHandle(hHttpSession);
	InternetCloseHandle(hIntSession);

	return request_data;
}

std::string GetUrlDataSpectro(std::string url)
{
	std::string request_data = "";

	HINTERNET hIntSession = InternetOpenA("", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	if (!hIntSession)
	{
		return request_data;
	}

	HINTERNET hHttpSession = InternetConnectA(hIntSession, HOST2, 80, 0, 0, INTERNET_SERVICE_HTTP, 0, NULL);

	if (!hHttpSession)
	{
		return request_data;
	}

	HINTERNET hHttpRequest = HttpOpenRequestA(hHttpSession, "GET", url.c_str()
		, 0, 0, 0, INTERNET_FLAG_RELOAD, 0);

	if (!hHttpSession)
	{
		return request_data;
	}

	char* szHeaders = ("Content-Type: text/html\r\nUser-Agent: License");
	char szRequest[1024] = { 0 };

	if (!HttpSendRequestA(hHttpRequest, szHeaders, strlen(szHeaders), szRequest, strlen(szRequest)))
	{
		return request_data;
	}

	CHAR szBuffer[1024] = { 0 };
	DWORD dwRead = 0;

	while (InternetReadFile(hHttpRequest, szBuffer, sizeof(szBuffer) - 1, &dwRead) && dwRead)
	{
		request_data.append(szBuffer, dwRead);
	}

	InternetCloseHandle(hHttpRequest);
	InternetCloseHandle(hHttpSession);
	InternetCloseHandle(hIntSession);

	return request_data;
}
bool Inject = false;
bool CheckLicense()
{
	std::string Serial = GetSerial64();

	std::string UrlRequest = PATH;
	UrlRequest.append(("gate.php?serial=") + Serial);

	std::string ReciveHash = GetUrlData(UrlRequest);

	if (ReciveHash.size())
	{
	std::string LicenseOK = ("license-success-ok-") + Serial + "-";

	for (int RandomMd5 = 1; RandomMd5 <= 10; RandomMd5++)
	{
	std::string LicenseCheck = LicenseOK + std::to_string(RandomMd5);
	std::string LicenseOKHash = GetHashText(LicenseCheck.c_str(), LicenseCheck.size());

	if (ReciveHash == LicenseOKHash)
	{
	Inject = true;
	return true;
	}
	}
	}
	Inject = false;
	return false;

	/*for (int i = 0; i < sizeof(valid); i++)
	{
		if (valid[i] == GetSerial())
		{

			return true;
		}
	}
	return false;
	*/
}

bool CheckLicenseSpectro()
{
	std::string Serial = GetSerial64();

	std::string UrlRequest = PATH;
	UrlRequest.append(("gate.php?serial=") + Serial);

	std::string ReciveHash = GetUrlDataSpectro(UrlRequest);





	if (ReciveHash.size())
	{
		std::string LicenseOK = ("license-success-ok-") + Serial + "-";

		for (int RandomMd5 = 1; RandomMd5 <= 10; RandomMd5++)
		{
			std::string LicenseCheck = LicenseOK + std::to_string(RandomMd5);
			std::string LicenseOKHash = GetHashText(LicenseCheck.c_str(), LicenseCheck.size());

			if (ReciveHash == LicenseOKHash)
			{

				
				Inject = true;
				return true;
			}
		}
	}
	Inject = false;
	return false;

	/*for (int i = 0; i < sizeof(valid); i++)
	{
	if (valid[i] == GetSerial())
	{

	return true;
	}
	}
	return false;
	*/
}

std::string GetUserDayCount()
{
	std::string Serial = GetSerial64();

	std::string UrlRequest = PATH;
	UrlRequest.append("gate.php?day=" + Serial);

	static std::string ReciveDay = GetUrlData(UrlRequest);
	return ReciveDay;
}

std::string GetVer()
{
	std::string web_req = PATH;
	web_req.append("gate.php?version=ok");
	return GetUrlData(web_req);
} //#define HOST должен быть настроен 

std::string Detect()
{

	std::string UrlRequest = PATH;
	UrlRequest.append("detect.txt");
	static std::string Status = GetUrlData(UrlRequest);
	return Status;
}