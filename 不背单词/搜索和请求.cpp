#include "初始化.h"
#include "主题管理.h"
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL.h>
#include <curl/curl.h>
#include <json/json.h>
#include <stdio.h>
#include <stdlib.h>



char search_textS[1024];

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    ((std::string*)userp)->append((char*)contents, realsize);
    return realsize;
}

void 下载音频(const char* word) {
    CURL* curl;
    CURLcode res;
    FILE* fp;
    char url[256];

    // 腾讯云语音合成 API 请求地址
    snprintf(url, sizeof(url), "https://tts.tencentcloudapi.com/");

    // 设置请求参数（需要使用腾讯云控制台提供的 SecretId 和 SecretKey）
    char data[512];
    snprintf(data, sizeof(data), R"({
        "Action": "TextToVoice",
        "Text": "%s",
        "VoiceType": 1,
        "SessionId": "123456",
        "ModelType": 1,
        "SecretId": "YOUR_SECRET_ID",
        "SecretKey": "YOUR_SECRET_KEY"
    })", word);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        fp = fopen("audio.mp3", "wb");
        if (fp) {
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);  // 设置请求体
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

            res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }

            fclose(fp);
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}
// 获取词根词缀信息
void 获取词根(const char* word) {
    CURL* curl;
    CURLcode res;
    std::string response;
    char url[256];
    snprintf(url, sizeof(url), "https://dictionaryapi.com/api/v3/references/collegiate/json/%s?key=YOUR_API_KEY", word);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else {
            Json::Reader reader;
            Json::Value root;
            if (reader.parse(response, root)) {
                const std::string etymology = root[0]["meta"]["syns"][0].asString();  // 示例获取词源信息
                printf("词源信息: %s\n", etymology.c_str());
            }
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

// 获取图片
void 获取图片(const char* query) {
    CURL* curl;
    CURLcode res;
    std::string response;
    char url[256];
    snprintf(url, sizeof(url), "https://api.unsplash.com/photos/random?query=%s&client_id=YOUR_API_KEY", query);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else {
            Json::Reader reader;
            Json::Value root;
            if (reader.parse(response, root)) {
                const std::string image_url = root[0]["urls"]["regular"].asString();
                printf("获取到的图片URL: %s\n", image_url.c_str());
                // 在 UI 上显示该图片
            }
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

// 搜索查询
void 搜索查询(const char* query) {
    // 获取音频、词根、图片等信息
    下载音频(query);      // 获取音频
    获取词根(query);      // 获取词根词缀
    获取图片(query);     // 获取图片
}


 
   
  