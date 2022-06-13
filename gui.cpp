#include "toad.h"
#include "imgui.h"
#include <imgui_internal.h>

ImColor mainColor = ImColor(0, 82, 22);
ImDrawList* draw;

float x = -34, y = -25;
float a = 484, b = 461;

int tab = 0;
bool binding = false;
int k = 0;

void decorations() {
    ImGuiStyle* style = &ImGui::GetStyle();

    style->FramePadding.y = 2.5f;
    style->TabRounding = 0;

    style->Colors[ImGuiCol_Text] = ImColor(188, 188, 188);

    style->Colors[ImGuiCol_ChildBg] = ImColor(20, 20, 20);
    style->Colors[ImGuiCol_Border] = ImColor(36, 36, 36);

    style->Colors[ImGuiCol_CheckMark] = mainColor;
    style->Colors[ImGuiCol_SliderGrab] = mainColor;
    style->Colors[ImGuiCol_SliderGrabActive] = ImColor(0, 60, 12);
    
    style->Colors[ImGuiCol_Button] = mainColor;
    style->Colors[ImGuiCol_ButtonHovered] = ImColor(0, 70, 17);
    style->Colors[ImGuiCol_ButtonActive] = ImColor(0, 60, 12);

    style->Colors[ImGuiCol_FrameBg] = ImColor(16, 16, 16);
    style->Colors[ImGuiCol_FrameBgActive] = ImColor(16, 16, 16);
    style->Colors[ImGuiCol_FrameBgHovered] = ImColor(16, 16, 16);

    style->Colors[ImGuiCol_PopupBg] = ImColor(36, 36, 36);
    style->Colors[ImGuiCol_HeaderHovered] = ImColor(0, 0, 0, 0);

    style->Colors[ImGuiCol_Tab] = ImColor(36, 36, 36);
    style->Colors[ImGuiCol_TabHovered] = ImColor(15, 97, 37);
    style->Colors[ImGuiCol_TabActive] = mainColor;
}

void toad::hotkey_handler(HWND window) {
    if (!binding) {
        if ((GetAsyncKeyState(toad::misc::keycode) & 1) && (k == 0))
        {
            if (toad::misc::window_hidden) { toad::misc::show(window); toad::misc::window_hidden = false; }
            else if (!toad::misc::window_hidden) { toad::misc::hide(window); toad::misc::window_hidden = true; }
            k = 1;
        }
        else if (GetAsyncKeyState(toad::misc::keycode) == 0) k = 0;

        if (GetAsyncKeyState(toad::clicker::keycode) & 1) { if (toad::misc::beep_on_toggle) Beep(350, 100); toad::clicker::enabled = !toad::clicker::enabled; }
        if (GetAsyncKeyState(toad::clicker::r::right_keycode) & 1) { if (toad::misc::beep_on_toggle) Beep(350, 100);  toad::clicker::r::right_enabled = !toad::clicker::r::right_enabled; }
    }
    else if (binding) {
        //(1)lmb - (123)f12
        for (int i = 3; i < 123; i++) {
            if (toad::clicker::key == "..") {
                if (GetAsyncKeyState(i)) {
                    if (i == VK_ESCAPE) { toad::clicker::key = "none"; binding = false; toad::clicker::keycode = 0; }
                    else { toad::clicker::key = toad::keys[i - 1]; toad::clicker::keycode = i; }
                }
                if (toad::clicker::key != "..") binding = false;
            }
            else if (toad::clicker::r::right_key == "..") {
                if (GetAsyncKeyState(i)) {
                    if (i == VK_ESCAPE) { toad::clicker::r::right_key = "none"; binding = false; toad::clicker::r::right_keycode = 0; }
                    else { toad::clicker::r::right_key = toad::keys[i - 1]; toad::clicker::r::right_keycode = i; }
                }
                if (toad::clicker::r::right_key != "..") binding = false;
            }
            else if (toad::misc::hide_key == "..") {
                if (GetAsyncKeyState(i)) {
                    if (i == VK_ESCAPE) { toad::misc::hide_key = "none"; binding = false; toad::misc::keycode = 0; }
                    else { toad::misc::hide_key = toad::keys[i - 1]; toad::misc::keycode = i; }
                }
                if (toad::misc::hide_key != "..") binding = false;
            }
        }
    }
}

void toad::renderUI(HWND hwnd, bool& done) {

    toad::hotkey_handler(hwnd);

    ImGui::Begin("Toad", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    decorations();

    ImGui::TextColored(mainColor, "toad");
    ImGui::SameLine();
    ImGui::TextColored(ImColor(51, 51, 51), "minecraft");

    if (ImGui::BeginTabBar("##tabbar"))
    {
        if (ImGui::BeginTabItem("   clicker   ", false))
        {
            tab = 0;
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("    misc     ", false))
        {
            tab = 1;
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

//    if (ImGui::Button("##clicker", ImVec2(70, 5))) tab = 0;
//    ImGui::SameLine();
//    if (ImGui::Button("##misc", ImVec2(70, 5))) tab = 1;

    ImGui::SetCursorPosX(20);
    ImGui::SetCursorPosY(60);

    if (tab == 0) {
        ImGui::BeginChild("left", ImVec2(ImGui::GetWindowSize().x / 2 - 30, 270), true);

        ImGui::SetCursorPosX(80);
        ImGui::TextColored(ImColor(122, 122, 122), "left");

        ImGui::Separator();

        ImGui::Checkbox("enable", &toad::clicker::enabled); ImGui::SameLine(); ImGui::TextColored(ImColor(51, 51, 51), "[%s]", &toad::clicker::key);
        if (ImGui::IsItemClicked()) { toad::clicker::key = ".."; binding = true; }

        ImGui::Text("min");
        ImGui::SliderInt("##Min", &toad::clicker::mincps, 5, 20, "%dcps");

        ImGui::Spacing();

        ImGui::Text("max");
        ImGui::SliderInt("##Max", &toad::clicker::maxcps, 5, 20, "%dcps");

        ImGui::Checkbox("rmb lock", &toad::clicker::rmb_lock);
        ImGui::Checkbox("blatant mode", &toad::clicker::blatant_mode);
        ImGui::Checkbox("mouse click sounds", &toad::clicker::click_sounds);
        ImGui::Checkbox("inventory", &toad::clicker::inventory);
        ImGui::Checkbox("prioritize higher cps", &toad::clicker::higher_cps);

        ImGui::Checkbox("slot whitelist", &toad::clicker::slot_whitelist);
        if (toad::clicker::slot_whitelist) {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.7f, 1));
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.2f, 3.f));
            ImGui::SetCursorPosX(10);
            ImGui::Checkbox("##slot1", &toad::clicker::whitelisted_slots[0]); ImGui::SameLine();
            ImGui::Checkbox("##slot2", &toad::clicker::whitelisted_slots[1]); ImGui::SameLine();
            ImGui::Checkbox("##slot3", &toad::clicker::whitelisted_slots[2]); ImGui::SameLine();
            ImGui::Checkbox("##slot4", &toad::clicker::whitelisted_slots[3]); ImGui::SameLine();
            ImGui::Checkbox("##slot5", &toad::clicker::whitelisted_slots[4]); ImGui::SameLine();
            ImGui::Checkbox("##slot6", &toad::clicker::whitelisted_slots[5]); ImGui::SameLine();
            ImGui::Checkbox("##slot7", &toad::clicker::whitelisted_slots[6]); ImGui::SameLine();
            ImGui::Checkbox("##slot8", &toad::clicker::whitelisted_slots[7]); ImGui::SameLine();
            ImGui::Checkbox("##slot9", &toad::clicker::whitelisted_slots[8]);
            ImGui::PopStyleVar();
            ImGui::PopStyleVar();
        }

        if (toad::clicker::mincps > toad::clicker::maxcps) toad::clicker::mincps = toad::clicker::maxcps;

        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::SetCursorPosX(ImGui::GetWindowSize().x - ImVec2(ImGui::GetWindowSize().x / 2 - 30, 200).x - 20);
        ImGui::BeginChild("right", ImVec2(ImGui::GetWindowSize().x / 2 - 30, 200), true);

        ImGui::SetCursorPosX(80);
        ImGui::TextColored(ImColor(122, 122, 122), "right");

        ImGui::Separator();
        
        ImGui::Checkbox("##Enable right", &toad::clicker::r::right_enabled); ImGui::SameLine(); ImGui::Text("enable"); ImGui::SameLine(); ImGui::TextColored(ImColor(51, 51, 51), "[%s]", &toad::clicker::r::right_key);
        if (ImGui::IsItemClicked()) { toad::clicker::r::right_key = ".."; binding = true; }
        ImGui::Text("min");
        ImGui::SliderInt("##Min right", &toad::clicker::r::right_mincps, 5, 30, "%dcps");
        ImGui::Spacing();
        ImGui::Text("max");
        ImGui::SliderInt("##Max right", &toad::clicker::r::right_maxcps, 5, 30, "%dcps");
        ImGui::Checkbox("##Inventory right", &toad::clicker::r::right_inventory); ImGui::SameLine(); ImGui::Text("inventory");

        ImGui::EndChild();       
    }
    else if (tab == 1) {
        ImGui::BeginChild("misc", ImVec2(ImGui::GetWindowSize().x / 2 - 30, 200), true);
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x / 2) - ImGui::CalcTextSize("misc").x + 10);
        ImGui::TextColored(ImColor(122, 122, 122),"misc");

        ImGui::Separator();
       
        ImGui::Checkbox("beep on toggle", &toad::misc::beep_on_toggle);
         
        ImGui::Text("hide bind"); ImGui::SameLine(); ImGui::TextColored(ImColor(51, 51, 51), "[%s]", &toad::misc::hide_key);
        if (ImGui::IsItemClicked()) { toad::misc::hide_key = ".."; binding = true; }

        ImGui::SetCursorPosY(160);
        if (ImGui::Button("exit"))
        {
            done = true;
        }

        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::SetCursorPosX(ImGui::GetWindowSize().x - ImVec2(ImGui::GetWindowSize().x / 2 - 30, 200).x - 20);
        ImGui::BeginChild("jitter", ImVec2(ImGui::GetWindowSize().x / 2 - 25, 200), true);
        ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - ImGui::CalcTextSize("jitter").x + 20);
        ImGui::TextColored(ImColor(122, 122, 122),"jitter");
        ImGui::Separator();

        ImGui::Checkbox("enable", &toad::jitter::enable);
        ImGui::Text("intensity x");
        ImGui::SliderInt("##intensity x", &toad::jitter::intensity_X, 1, 10);
        ImGui::Spacing();
        ImGui::Text("intensity y");
        ImGui::SliderInt("##Intensity y", &toad::jitter::intensity_Y, 1, 10);
        ImGui::Spacing();
        ImGui::Text("chance");
        ImGui::SliderInt("##Chance", &toad::jitter::chance, 1, 100, "%d%%");

        ImGui::EndChild();
    }

    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 20);
    ImGui::TextColored(ImColor(51, 51, 51), "vierkant");
    ImGui::End();
}
