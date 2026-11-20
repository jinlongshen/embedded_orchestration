#pragma once
#include <string>

class LlmClient {
public:
    LlmClient(std::string baseUrl, std::string apiKey, std::string model);
    std::string chat(const std::string& systemPrompt, const std::string& userPrompt);

private:
    std::string baseUrl_;
    std::string apiKey_;
    std::string model_;
};

