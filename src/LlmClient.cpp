#include "LlmClient.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <stdexcept>

using json = nlohmann::json;

namespace {
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}
}

LlmClient::LlmClient(std::string baseUrl, std::string apiKey, std::string model)
    : baseUrl_(std::move(baseUrl)), apiKey_(std::move(apiKey)), model_(std::move(model)) {}

std::string LlmClient::chat(const std::string& systemPrompt, const std::string& userPrompt) {
    CURL* curl = curl_easy_init();
    if (!curl) throw std::runtime_error("Failed to initialize CURL");

    json payload = {
        {"model", model_},
        {"messages", {
            {{"role", "system"}, {"content", systemPrompt}},
            {{"role", "user"}, {"content", userPrompt}}
        }},
        {"temperature", 0.2}
    };

    std::string payloadStr = payload.dump();
    std::string responseStr;
    std::string endpoint = baseUrl_ + "/chat/completions";

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey_).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payloadStr.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        throw std::runtime_error(std::string("CURL request failed: ") + curl_easy_strerror(res));
    }

    auto parsed = json::parse(responseStr);
    return parsed["choices"].at(0)["message"]["content"].get<std::string>();
}

