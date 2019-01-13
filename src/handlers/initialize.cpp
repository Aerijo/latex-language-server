#include <Handler.h>

std::optional<Value> getObject (const std::string &key, Value &params) {
    auto itr = params.FindMember(key.c_str());
    if (itr == params.MemberEnd() || itr->value.IsNull()) {
        return {};
    }
    return { itr->value.GetObject() };
}

std::optional<Value> getArray (const std::string &key, Value &params) {
    auto itr = params.FindMember(key.c_str());
    if (itr == params.MemberEnd() || itr->value.IsNull()) {
        return {};
    }
    return { itr->value.GetArray() };
}

std::optional<int_fast64_t> getIntFast64 (const std::string &key, Value &params) {
    auto itr = params.FindMember(key.c_str());
    if (itr == params.MemberEnd() || itr->value.IsNull()) {
        return {};
    }
    return { itr->value.GetInt64() };
}

std::optional<std::string> getString (const std::string &key, Value &params) {
    auto itr = params.FindMember(key.c_str());
    if (itr == params.MemberEnd() || itr->value.IsNull()) {
        return {};
    }
    return { itr->value.GetString() };
}

struct IntitializationParams {

};

class InitializeHandler : RequestHandler {

    InitializeHandler () { method = "initialize"; }

    void run (Id id, Value &params) override {
        auto parentProcessId = getIntFast64("processId", params);

        auto rootUri = getString("rootUri", params);

        auto capabilities = getObject("capabilities", params);

        auto workspaceFolders = getArray("workspaceFolders", params);
    }
};
