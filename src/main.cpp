#include "LlmClient.h"
#include <cstdlib>
#include <iostream>

int main() {
    const char* envUrl = std::getenv("LLM_BASE_URL");
    const char* envKey = std::getenv("OPENAI_API_KEY");
    const char* envModel = std::getenv("LLM_MODEL");

    std::string baseUrl = envUrl ? envUrl : "http://localhost:11434/v1";
    std::string apiKey  = envKey ? envKey : "ollama";
    std::string model   = envModel ? envModel : "qwen2.5-coder:7b";

    try {
        LlmClient client(baseUrl, apiKey, model);
        std::string reply = client.chat(
            "You are an embedded orchestration engine.",
            "Confirm connection and state your readiness in one short sentence."
        );
        std::cout << "Agent Response: " << reply << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

