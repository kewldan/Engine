#include "Settings.h"

Engine::Settings::Settings(const char *name) {
    path = new char[128];
    static auto *b = new wchar_t[128];
    SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &b);
    wsprintfA(path, "%ls", b);
    strcat_s(path, 128, "\\");
    strcat_s(path, 128, name);
    std::filesystem::create_directories(path);
    strcat_s(path, 128, "\\settings.bson");
    json = nlohmann::json::object();
}

void Engine::Settings::save() {
    std::vector<uint8_t> data = nlohmann::json::to_bson(json);
    Engine::Filesystem::writeFile(path, reinterpret_cast<const char *>(data.data()), data.size());
}

void Engine::Settings::load() {
    int size = 0;
    const char *data = Engine::Filesystem::readFile(path, &size);
    std::vector<uint8_t> v;
    for (int i = 0; i < size; i++) {
        v.push_back(data[i]);
    }
    json = nlohmann::json::from_bson(v);
}

bool Engine::Settings::exists() {
    return Engine::Filesystem::exists(path);
}

template<class T>
T Engine::Settings::get(const nlohmann::json::json_pointer &ptr, T defaultValue) {
    if (json.contains(ptr)) {
        return json[ptr];
    } else {
        return defaultValue;
    }
}
