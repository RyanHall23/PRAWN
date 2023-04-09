#include "VESCheck.h"

CVESCheck::CVESCheck()
{

}

CVESCheck::~CVESCheck()
{

}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int CVESCheck::CheckTaxMOT(std::string str_Reg)
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, m_strAPIKey.c_str());
        headers = curl_slist_append(headers, m_strContent.c_str());

        const char* data = "{\"registrationNumber\": \"TE57VRN\"}";

        curl_easy_setopt(curl, CURLOPT_URL, "https://driver-vehicle-licensing.api.gov.uk/vehicle-enquiry/v1/vehicles");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(data));
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        OutputDebugString((LPCSTR)readBuffer.c_str());
    }

    return 0;
}
