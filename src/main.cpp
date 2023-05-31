#include <ImGuiInstance/ImGuiInstance.hpp>
#include <nlohmann/json.hpp>
#include <imgui_stdlib.h>
#include <fstream>
#include <iostream>
#include <filesystem>





nlohmann::json load_json_file(const std::string path){
    std::ifstream json_file(path);
    nlohmann::json return_json;
    json_file>>return_json;
    return return_json; 
}
void write_json_file(const std::string path, const nlohmann::json& json_data) {
    std::ofstream json_file(path);
    json_file << json_data.dump(4);
    json_file.close();
}
template<typename T>
T load_json(nlohmann::json configjson, std::string key){
    return configjson[key].get<T>();
}
template<typename T>
T load_json(nlohmann::json configjson, std::string key1, std::string key2){
    return configjson[key1][key2].get<T>();
}
template<typename T>
T load_json(nlohmann::json configjson, std::string key1, std::string key2, std::string key3){
    return configjson[key1][key2][key3].get<T>();
}
template<typename T>
T load_json(nlohmann::json configjson, std::string key1, std::string key2, std::string key3,std::string key4){
    return configjson[key1][key2][key3][key4].get<T>();
}

std::vector<std::string> getAvailableLanguages(const std::string& languageFolder) {
    std::vector<std::string> languages;
    for (const auto& entry : std::filesystem::directory_iterator(languageFolder)) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            if (extension == ".json") {
                std::string filename = entry.path().filename().stem().string();
                languages.push_back(filename);
            }
        }
    }
    return languages;
}


template <typename T>
bool check_fin_valid (T fin_to_check)
{    
    if (strlen(fin_to_check)!=17)
    {
        return false;
    }  
    else 
    {
        return true;
    }
}

void save_data(const std::string& fin) {
    nlohmann::json data;
    data["timestamp"] = std::chrono::system_clock::now().time_since_epoch().count();
    data["fin"] = fin;

    std::ofstream file("data.json");
    file << data.dump(4); // Hier wird der Inhalt der JSON-Datei gespeichert, mit einer Einrückung von 4 Leerzeichen
    file.close();
}

int main() {
    nlohmann::json config = load_json_file("../../config/config.json");
    nlohmann::json language = load_json_file("../../config/" + load_json<std::string>(config,"language") + ".json");
    
    auto render = [&]() {


        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("foobar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);
        ImGui::SetWindowFontScale(3);
        
        if (ImGui::BeginMenu(load_json<std::string>(language,"menu","label").c_str())) {
            // Optionen hinzufügen
            if (ImGui::BeginMenu(load_json<std::string>(language,"menu","option").c_str())) {
                ImGui::SetWindowFontScale(3);
                if (ImGui::MenuItem("Deutsch")) {
                    config["language"]="de";
                    write_json_file("../../config/config.json",config);
                }
                if (ImGui::MenuItem("Englisch")) {
                    config["language"]="en";
                    write_json_file("../../config/config.json",config);
                }
                
                ImGui::EndMenu();    
            }
            
            if (ImGui::MenuItem("nope")) {
           
            }
            if (ImGui::MenuItem("no overtime")) {
              
            }
            
            ImGui::EndMenu();
        }
        
        static char fahrzeugidentnummer[18]="";
        
        //ImGui::InputText("input_text_fin_label", fahrzeugidentnummer,18);
        ImGui::InputText(load_json<std::string>(language,"inputfin","label").c_str(), fahrzeugidentnummer,18);
        if(check_fin_valid(fahrzeugidentnummer)){
            ImGui::Text(load_json<std::string>(language,"inputfin","fail").c_str());
        } else {
            ImGui::Text(load_json<std::string>(language,"inputfin","complete").c_str());
        }
             
        ImGui::Button(load_json<std::string>(language,"button","save","label").c_str(), ImVec2(load_json<int>(config,"button","sizex"), load_json<int>(config,"button","sizey")));

        
        

        ImGui::End();
    };
   
    ImGuiInstance window{1920,1080, "Test"};

    while(window.run(render)) {
        
    }
    return 0;
}
