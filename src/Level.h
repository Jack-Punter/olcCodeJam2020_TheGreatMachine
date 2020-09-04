#ifndef LEVEL_H
#define LEVEL_H
#include "CircuitNode.h"
#include "CircuitEditor.h"
#include <math.h>
#include <algorithm>

struct Level {
    virtual ~Level() = default;
    virtual void OnUserCreate(olc::PixelGameEngine *_pge) {
        pge = _pge;
    }
    
    uint64_t AysncSimulationCounter = 0;
    virtual bool AsyncSimulateGen()
    {
        for(uint64_t i = 0; i < Inputs.size(); ++i) {
            bool ThisInput = AysncSimulationCounter & (1ll << i);
            Inputs[i].first->value = ThisInput;
            Inputs[i].second->value = ThisInput;
        }
        
        AysncSimulationCounter = (AysncSimulationCounter + 1) % (uint64_t)exp2(Inputs.size());
        return AysncSimulationCounter == 0;
    }
    
    uint64_t AysncValidateCounter = 0;
    virtual std::pair<bool, bool> AsyncValidateGen()
    {
        for(uint64_t i = 0; i < Inputs.size(); ++i) {
            bool ThisInput = AysncValidateCounter & (1ll << i);
            Inputs[i].first->value = ThisInput;
            Inputs[i].second->value = ThisInput;
        }
        
        bool result = std::all_of(std::begin(Outputs), std::end(Outputs),
                                  [] (std::pair<CircuitNode *, CircuitNode *> pair) {
                                      return pair.first->Evaluate(0) == pair.second->Evaluate(0);
                                  });
        if (result) {
            AysncValidateCounter = (AysncValidateCounter + 1) % (uint64_t)exp2(Inputs.size());
        } else {
            AysncValidateCounter = 0;
        }
        return std::make_pair(result, AysncValidateCounter == 0);
    };
    
    virtual void OnUserUpdate(float fElapsedTime)
    {
        if (!TruthTable) {
            TruthTableWidth = (Inputs.size() + Outputs.size());
            TruthTableHeight = (size_t)exp2(Inputs.size());
            TruthTable = (bool*)malloc(TruthTableWidth * TruthTableHeight * sizeof(bool));
            
            for (int y = 0; y < TruthTableHeight; ++y) {
                int x = 0;
                for (int i = 0; i < Inputs.size(); ++i) {
                    bool Value = (bool)(y & ((1 << (Inputs.size() - 1 - i))));
                    TruthTable[y * TruthTableWidth + x] = Value;
                    Inputs[i].first->value = Value;
                    ++x;
                }
                for (int i = 0; i <  Outputs.size(); ++i) {
                    bool Value = Outputs[i].first->Evaluate(0);
                    TruthTable[y * TruthTableWidth + x] = Value;
                    ++x;
                }
            }
            for(auto i : Inputs) {
                i.first->value = false;
            }
        }
        
        editor.OnUserUpdate(fElapsedTime);
        
        if (IsEvaluating) {
            StartTimeBetweenSimulationStates = std::min(30.0f / (float)exp2(Inputs.size()), 0.5f);
            TimeBetweenSimulationStates -= fElapsedTime;
            if(TimeBetweenSimulationStates <= 0.0f) {
                std::pair<bool, bool> R = AsyncValidateGen();
                IsEvaluating = !R.second;
                if(!R.first) {
                    IsEvaluating = false;
                }
                LevelCompleted = (R.second && R.first);
                TimeBetweenSimulationStates = StartTimeBetweenSimulationStates;
            }
        }
        
        if (IsSimulating) {
            StartTimeBetweenSimulationStates = std::min(30.0f / (float)exp2(Inputs.size()), 0.5f);
            TimeBetweenSimulationStates -= fElapsedTime;
            if(TimeBetweenSimulationStates <= 0.0f) {
                if (AsyncSimulateGen()) {
                    IsSimulating = false;
                }
                TimeBetweenSimulationStates = StartTimeBetweenSimulationStates;
            }
        }
    }
    
    virtual void DrawUI()
    {
        if (ImGui::Begin("Window", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
        {
            olc::vi2d WindowSize = pge->GetWindowSize();
            SideBarSize = {(float)WindowSize.x / 4, (float)WindowSize.y};
            SideBarPos = {WindowSize.x - SideBarSize.x, 0.0f};
            
            editor.SideBarSize = SideBarSize;
            editor.SideBarPos = SideBarPos;
            
            ImGui::SetWindowPos("Window", {SideBarPos.x, SideBarPos.y});
            ImGui::SetWindowSize("Window", {SideBarSize.x, SideBarSize.y});
            ImGuiID dockspace_id = ImGui::GetID("Window");
            ImGui::DockSpace(dockspace_id);
            
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Windows")) {
                    
                    ImGui::MenuItem("Show Editor Controls", NULL, &ShowEditorControls);
                    ImGui::MenuItem("Show Level Controls", NULL, &ShowLevelControls);
                    ImGui::MenuItem("Show Black Box Truth Table", NULL, &ShowTruthTable);
                    
                    ImGui::EndMenu();
                }
                
                ImGui::EndMenuBar();
            }
        }
        ImGui::End();
        
        if (ShowEditorControls) {
            editor.DrawUI();
        }
        
        if (ShowLevelControls) {
            if (ImGui::Begin("Simulation")){
                ImGui::Text(LevelCompleted ? "Congratulations: Level Complete!" : "This level has not yet been completed");
                
                if (!IsEvaluating && !IsSimulating) {
                    if (ImGui::Button("Evaluate Circuit")) {
                        IsEvaluating = true;
                        TimeBetweenSimulationStates = 0.0f;
                    }
                    if(ImGui::Button("Simulate Circuit")) {
                        IsSimulating = true;
                        TimeBetweenSimulationStates = 0.0f;
                    }
                }
            }
            ImGui::End();
        }
        
        if(ShowTruthTable) {
            if(ImGui::Begin("Truth Table")) {
                std::string str = "";
                for (int y = 0; y < TruthTableHeight; ++y) {
                    for (int x = 0; x < TruthTableWidth; ++x) {
                        if (x == Inputs.size()) {
                            str += "| ";
                        }
                        str += TruthTable[y * TruthTableWidth + x] ? "1 " : "0 ";
                    }
                    str+= "\n";
                }
                ImGui::Text(str.c_str());
            }
            ImGui::End();
        }
    }
    
    size_t TruthTableWidth;
    size_t TruthTableHeight;
    bool *TruthTable = 0;
    std::vector<std::pair<CircuitNodeBIT *, CircuitNodeBIT *>> Inputs;
    std::vector<std::pair<CircuitNode *, CircuitNode *>> Outputs;
    
    char *LevelName;
    char *LevelCompleteName;
    
    olc::PixelGameEngine *pge;
    CircuitEditor editor;
    
    float StartTimeBetweenSimulationStates = 0.5f;
    float TimeBetweenSimulationStates = StartTimeBetweenSimulationStates;
    bool IsEvaluating = false;
    bool IsSimulating = false;
    bool LevelCompleted = false;
    
    bool ShowEditorControls = true;
    bool ShowLevelControls = true;
    bool ShowTruthTable = false;
    olc::vf2d SideBarSize;
    olc::vf2d SideBarPos;
};

struct SandboxLevel : public Level
{
    void OnUserCreate(olc::PixelGameEngine *_pge) override
    {
        Level::OnUserCreate(_pge);
        editor.OnUserCreate(_pge);
        LevelName = "Sandbox Level";
        LevelCompleteName = "Sandbox Level";
    }
    void OnUserUpdate(float fElapsedTime) override
    {
        editor.OnUserUpdate(fElapsedTime);
    }
    
    void DrawUI() override
    {
        ShowLevelControls = false;
        ShowTruthTable = false;
        Level::DrawUI();
    }
};

#endif //LEVEL_H
