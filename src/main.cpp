#include <ImGuiInstance/ImGuiInstance.hpp>
#include <nlohmann/json.hpp>
#include <imgui_stdlib.h>
#include <fstream>
#include <iostream>


nlohmann::json load_json(const std::string path){
    std::ifstream json_file(path);
    nlohmann::json return_json;
    json_file>>return_json;
    return return_json; 

}
template<typename T>
T load_config(const std::string key){
    nlohmann::json settings = load_json("../../config/config.json");
    return settings[key].get<T>();
}
template<typename T>
T load_config(const std::string firstkey, const std::string lastkey){
    nlohmann::json settings = load_json("../../config/config.json");
    return settings[firstkey][lastkey].get<T>();
}


auto load_lang_string(const std::string& lang_key) {
    std::ifstream lang_file("../../config/" + load_config<std::string>("language")+ ".json");
    nlohmann::json lang_settings;
    lang_file >> lang_settings;
    std::string returnstring = lang_settings[lang_key].get<std::string>();
    std::cout << returnstring << std::endl;
    return returnstring;
}


struct label{
    std::string input_text_fin_label = load_lang_string("input_text_fin_label");
    std::string input_text_fin_fail = load_lang_string("input_text_fin_fail");
    std::string input_text_fin_complete = load_lang_string("input_text_fin_complete");
    std::string button_save_label = load_lang_string("button_save_label");
};



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
    label mylabel;

    //std::string input_text_fin_label = {load_lang_string("input_text_fin_label")};

    auto render = [&]() {
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("foobar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);
        ImGui::SetWindowFontScale(3);
        
        
        static char fahrzeugidentnummer[18]="";
        
        //ImGui::InputText("input_text_fin_label", fahrzeugidentnummer,18);
        ImGui::InputText(mylabel.input_text_fin_label.c_str(), fahrzeugidentnummer,18);
        if(check_fin_valid(fahrzeugidentnummer)){
            ImGui::Text(mylabel.input_text_fin_complete.c_str());
        } else {
            ImGui::Text(mylabel.input_text_fin_fail.c_str());
        }
                
        ImGui::Button(mylabel.button_save_label.c_str(), ImVec2(load_config<int>("button","sizex"), load_config<int>("button","sizey")));

        ImGui::End();
    };
    ImGuiInstance window{load_config<size_t>("window","sizex"), load_config<size_t>("window","sizex"), "Test"};
    while(window.run(render)) {
    }
    return 0;
}
