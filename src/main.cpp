#include <ImGuiInstance/ImGuiInstance.hpp>
#include <nlohmann/json.hpp>
#include <imgui_stdlib.h>
#include <fstream>
#include <iostream>

auto load_lang_string(const std::string& lang_key) {
    std::ifstream lang_file("config/de.json");
    nlohmann::json lang_settings;
    lang_file >> lang_settings;
    std::string returnstring = lang_settings[lang_key].get<std::string>();
    std::cout << returnstring << std::endl;
    return returnstring;
}

struct label{
    std::string input_text_fin_label = load_lang_string("input_text_fin_label");
    std::string input_text_fin_fail = load_lang_string("input_text_fin_fail");
    std::string input_text_fin_success = load_lang_string("input_text_fin_success");
};



template <typename T>
bool check_fin_valid (T fin_to_check)
{   
    
    if (strlen(fin_to_check)!=17)
    {
        return true;
   
    }  
    else 
    {
        return false;
    }
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
            ImGui::Text(mylabel.input_text_fin_fail.c_str());
        } else {
            ImGui::Text(mylabel.input_text_fin_success.c_str());
        }
        

        
        ImGui::Button("speichern", ImVec2(300, 200));



        ImGui::End();
    };

    ImGuiInstance window{1280, 760, "Test"};
    while(window.run(render)) {
    }
    return 0;
}
