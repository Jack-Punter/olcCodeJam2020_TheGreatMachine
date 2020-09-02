#include "olcPixelGameEngine.h"
#include "pge\imgui_impl_pge.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#define Assert(x) {if(!(x)) *(int*)nullptr = 0;}
#define ArrayCount(x) (sizeof(x)/sizeof(x[0]))

template<typename Container, typename T>
inline void RemoveErase(Container* container, T value) {
    auto NewEnd = std::remove(std::begin(*container), std::end(*container), value);
    container->erase(NewEnd, std::end(*container));
}

#include "generated\generated.h"
#include "CircuitEditor.h"
#include "CircuitNode.h"
#include "Level.h"
#include "generated\LevelFullAdder.h"
#include <algorithm>

class TheGreatMachine : public olc::PixelGameEngine
{
    using olc::PixelGameEngine::FillRect;
    public:
	TheGreatMachine()
    {
		sAppName = "TheGreatMachine";
	}
    public:
	bool OnUserCreate() override
	{
        // ==================== IMGUI INIT ====================
        //One time initialization of the Dear ImGui library
		ImGui::CreateContext();
		//Create an instance of the Dear ImGui PGE Integration
		pge_imgui = olc::imgui::PGE_ImGUI();
		
		//The vi2d for pixel size must match the values given in Construct()
		//Otherwise the mouse will not work correctly
		pge_imgui.ImGui_ImplPGE_Init(this);
		//Initialize the OpenGL2 rendering system
		ImGui_ImplOpenGL2_Init();
        
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		
        //Set a custom render function on layer 0.  Since DrawUI is a member of
		//our class, we need to use std::bind
		SetLayerCustomRenderFunction(0, std::bind(&TheGreatMachine::DrawUI, this));
        // ====================================================
        
        // Because we are using the ImGui PGEX we need to render our game
        // to a layer that is NOT 0
        GameLayer = CreateLayer();
        EnableLayer(GameLayer, true);
        
        TheGreatMachineImage = new olc::Renderable;
        TheGreatMachineImage->Load("..\\misc\\TheGreatMachine.png");
        
        BGLayer = CreateLayer();
        EnableLayer(BGLayer, true);
        SetDrawTarget(BGLayer);
        Clear(olc::Pixel(0x4c, 0x4c, 0x4c));
        
        Levels.push_back(new ExampleLevel);
        Levels.back()->OnUserCreate(this);
        
        Levels.push_back(new FullAdderLevel);
        Levels.back()->OnUserCreate(this);
        
        return true;
	}
    
    float TitleTimer = 2.0f;
	bool OnUserUpdate(float fElapsedTime) override
	{
        SetDrawTarget(GameLayer);
        Clear(olc::BLANK);
        // Draw the rendered UI Pannels to the screen
        
        switch (GameState) {
            case GAME_STATE_TITLE: {
                float ScaleX = (float)ScreenWidth() / (float)TheGreatMachineImage->Sprite()->width;
                float ScaleY = (float)ScreenHeight() / (float)TheGreatMachineImage->Sprite()->height;
                DrawDecal({0.0f, 0.0f}, TheGreatMachineImage->Decal(), {ScaleX, ScaleY});
                
                TitleTimer -= fElapsedTime;
                if (TitleTimer < 0.0f || GetMouse(0).bPressed) {
                    GameState = GAME_STATE_LEVEL_SELECT;
                }
            } break;
            
            case GAME_STATE_LEVEL_SELECT: {
                SetDrawTarget(BGLayer);
                float ScaleX = (float)ScreenWidth() / (float)TheGreatMachineImage->Sprite()->width;
                float ScaleY = (float)ScreenHeight() / (float)TheGreatMachineImage->Sprite()->height;
                DrawDecal({0.0f, 0.0f}, TheGreatMachineImage->Decal(), {ScaleX, ScaleY}, olc::Pixel(255, 255, 255, 128));
                SetDrawTarget(GameLayer);
                const char *LevelSelectText = "[WIP] Select Level";
                
                olc::vi2d ScreenCentre = ScreenSize() / 2;
                int TextScale = 4;
                olc::vi2d LineSize =  GetTextSize(LevelSelectText) * TextScale;
                DrawString(ScreenCentre.x - LineSize.x / 2, 1 * LineSize.y, LevelSelectText, olc::WHITE, TextScale);
                
                for (int i = 0; i < Levels.size(); ++i) {
                    char * text;
                    olc::Pixel TextColor;
                    if (Levels[i]->LevelCompleted) {
                        text = Levels[i]->LevelCompleteName;
                        TextColor = olc::GREEN;
                    } else {
                        text = Levels[i]->LevelName;
                        TextColor = olc::WHITE;
                    }
                    olc::vi2d textSize =  GetTextSize(text) * TextScale;
                    
                    // TODO(jack): Postion based on how many levels there are
                    olc::vi2d textPos = {ScreenCentre.x - textSize.x / 2,  (1 + i) * (ScreenHeight() / (1 + (int)Levels.size()))};
                    olc::vi2d MousePos = GetMousePos();
                    if (MousePos.x >= textPos.x && MousePos.x <= textPos.x + textSize.x &&
                        MousePos.y >= textPos.y && MousePos.y <= textPos.y + textSize.y)
                    {
                        TextColor = olc::BLUE;
                        if (GetMouse(0).bPressed) {
                            GameState = GAME_STATE_EDITOR;
                            ActiveLevel = Levels[i];
                        }
                    }
                    
                    DrawString(textPos, text, TextColor, TextScale);
                }
            } break;
            
            case GAME_STATE_EDITOR: {
                ActiveLevel->OnUserUpdate(fElapsedTime);
                if(GetKey(olc::ESCAPE).bPressed) {
                    GameState = GAME_STATE_LEVEL_SELECT;
                }
            } break;
        }
        return true;
    }
    
    void DrawUI()
    {
        BeginImGuiFrame();
        
        if (GameState == GAME_STATE_EDITOR) {
            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("Main")) {
                    if (ImGui::MenuItem("Level Select", "Esc")) {
                        GameState = GAME_STATE_LEVEL_SELECT;
                    }
                    
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }
            ActiveLevel->DrawUI();
        }
        EndImGuiFrame();
    }
    
    void BeginImGuiFrame() {
        //These 3 lines are mandatory per-frame initialization
        ImGui_ImplOpenGL2_NewFrame();
        pge_imgui.ImGui_ImplPGE_NewFrame();
        ImGui::NewFrame();
    }
    
    void EndImGuiFrame() {
        //This finishes the Dear ImGui and renders it to the screen
        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    }
    
    olc::Renderable* TheGreatMachineImage;
    olc::Sprite* TheGreatMachineBG;
    
    int GameLayer;
    int BGLayer;
    enum {
        GAME_STATE_TITLE,
        GAME_STATE_LEVEL_SELECT,
        GAME_STATE_EDITOR
    } GameState = GAME_STATE_TITLE;
    
    std::vector<Level *> Levels;
    Level *ActiveLevel;
    CircuitEditor *editor;
    olc::imgui::PGE_ImGUI pge_imgui;
};

int main()
{
    TheGreatMachine demo;
    if (demo.Construct(1280, 720, 1, 1))
        demo.Start();
    
    return 0;
}
