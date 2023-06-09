#include <ImGuiInstance/ImGuiInstance.hpp>
#include <nlohmann/json.hpp>
#include <imgui_stdlib.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <type_traits>

const std::string configpath = "../../config/config.json";



nlohmann::json load_json_file( std::string const& path){
    std::ifstream json_file(path);
    nlohmann::json return_json;
    json_file>>return_json;
    return return_json; 
}

void write_json_file( std::string const& path,  nlohmann::json const& json_data) {
    std::ofstream json_file(path);
    json_file << json_data.dump(4);
    json_file.close();
}


template<typename T>
T load_json(const nlohmann::json& configjson, const std::string& key) {
    if (configjson.contains(key)) {
        return configjson[key].get<T>();
    }
    else {
        if constexpr(std::is_same_v<T,std::string>) {
            return T{key};
        }else{
            return T{};
        }
    }
}

template<typename T, typename... Args>
T load_json(const nlohmann::json& config, const std::string& key, Args... keylist) {
    nlohmann::json unpackme = config[key];
    T returnme = load_json<T>(unpackme, keylist...);
    
    return returnme;
    
}


std::vector<std::string> getAvailableLanguages( std::string const& languageFolder) {
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
bool check_fin_valid(T fin_to_check) {
    if (strlen(fin_to_check) != 17) {
        return false;
    } else {
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

    nlohmann::json config = load_json_file(configpath);

    std::vector<std::string> availableLanguages = getAvailableLanguages(load_json<std::string>(config,"languagepath"));
    
    nlohmann::json language = load_json_file(load_json<std::string>(config,"languagepath") + load_json<std::string>(config,"language") + ".json");
    
    auto render = [&]() {


        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("foobar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);
        ImGui::SetWindowFontScale(3);
    

        //ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.6f, 0.9f, 1.0f)); // Ändere die Hintergrundfarbe des Buttons bei Hover
        //ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.2f, 0.3f, 1.0f)); // Ändere die Hintergrundfarbe des Buttons bei Aktivierung
    

        
        if (ImGui::BeginMenu(load_json<std::string>(language,"menu","label").c_str())) {
            // Optionen hinzufügen
            if (ImGui::BeginMenu(load_json<std::string>(language,"menu","language_option").c_str())) {
                ImGui::SetWindowFontScale(3);
                for (const auto& lang : availableLanguages) {
                    if (ImGui::MenuItem(lang.c_str())) {
                        config["language"] = lang;
                        write_json_file("../../config/config.json", config);
                    }
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

         // Ändere die Hintergrundfarbe des Buttons

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(load_json<float>(config,"button","normal","red"),load_json<float>(config,"button","normal","blue"),load_json<float>(config,"button","normal","green"),load_json<float>(config,"button","normal","transparency")));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(load_json<float>(config,"button","hover","red"),load_json<float>(config,"button","hover","blue"),load_json<float>(config,"button","hover","green"),load_json<float>(config,"button","hover","transparency")));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(load_json<float>(config,"button","active","red"),load_json<float>(config,"button","active","blue"),load_json<float>(config,"button","active","green"),load_json<float>(config,"button","active","transparency")));

     
       

        if(ImGui::Button(load_json<std::string>(language,"button","measure","label").c_str(), ImVec2(load_json<int>(config,"button","sizex"), load_json<int>(config,"button","sizey"))))
        {

        };
        ImGui::SameLine();
        if(ImGui::Button(load_json<std::string>(language,"button","send","label").c_str(), ImVec2(load_json<int>(config,"button","sizex"), load_json<int>(config,"button","sizey"))))
        {

        };
        ImGui::SameLine();
        if(ImGui::Button(load_json<std::string>(language,"button","save","label").c_str(), ImVec2(load_json<int>(config,"button","sizex"), load_json<int>(config,"button","sizey"))))
        {

        };
        ImGui::PopStyleColor(3);
        
        ImGui::End();
    };
   
    ImGuiInstance window{load_json<size_t>(config,"window","sizex"),load_json<size_t>(config,"window","sizey"),load_json<std::string>(config,"title")};

    while(window.run(render)) {
        
    }
    return 0;
}
