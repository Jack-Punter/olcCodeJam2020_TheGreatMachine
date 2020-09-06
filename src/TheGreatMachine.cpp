#include "olcPixelGameEngine.h"
#include "pge/imgui_impl_pge.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl2.h"
#define Assert(x) {if(!(x)) *(int*)nullptr = 0;}
#define ArrayCount(x) (sizeof(x)/sizeof(x[0]))

template<typename Container, typename T>
inline void RemoveErase(Container* container, T value) {
    auto NewEnd = std::remove(std::begin(*container), std::end(*container), value);
    container->erase(NewEnd, std::end(*container));
}

#include "generated/generated.h"
#include "CircuitEditor.h"
#include "CircuitNode.h"
#include "Level.h"

// ================ SimpleLevels ================
#include "generated/AndLevel.h"
#include "generated/OrLevel.h"
#include "generated/XorLevel.h"
#include "generated/NandLevel.h"
#include "generated/NorLevel.h"
#include "generated/XnorLevel.h"
#include "generated/NotLevel.h"

// ================ MediumLevels ================
#include "generated/FullAdderLevel.h"
#include "generated/MultiPlexerLevel.h"
#include "generated/Medium3Level.h"
#include "generated/Medium4Level.h"
#include "generated/Medium5Level.h"

// ================= HardLevels =================
#include "generated/Hard1Level.h"
#include "generated/Hard2Level.h"
#include "generated/Hard3Level.h"


#include <algorithm>

class TheGreatMachine : public olc::PixelGameEngine
{
    using olc::PixelGameEngine::FillRect;
    public:
	TheGreatMachine() {
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
        TheGreatMachineImage->Load("../misc/TheGreatMachine.png");
        
        BGLayer = CreateLayer();
        EnableLayer(BGLayer, true);
        SetDrawTarget(BGLayer);
        Clear(olc::Pixel(0x4c, 0x4c, 0x4c));
        
        // ================ SimpleLevels ================
        Levels.push_back(new AndLevel);
        Levels.back()->OnUserCreate(this);
        
        Levels.push_back(new OrLevel);
        Levels.back()->OnUserCreate(this);
        
        Levels.push_back(new XorLevel);
        Levels.back()->OnUserCreate(this);
        
        Levels.push_back(new NandLevel);
        Levels.back()->OnUserCreate(this);
        
        Levels.push_back(new NorLevel);
        Levels.back()->OnUserCreate(this);
        
        Levels.push_back(new XnorLevel);
        Levels.back()->OnUserCreate(this);
        
        Levels.push_back(new NotLevel);
        Levels.back()->OnUserCreate(this);
        
        // ================ MediumLevels ================
        Levels.push_back(new FullAdderLevel);
        Levels.back()->OnUserCreate(this);
        
        Levels.push_back(new MultiPlexerLevel);
        Levels.back()->OnUserCreate(this);
        
        Levels.push_back(new Medium3Level);
        Levels.back()->OnUserCreate(this);
        
        Levels.push_back(new Medium4Level);
        Levels.back()->OnUserCreate(this);
        
        Levels.push_back(new Medium5Level);
        Levels.back()->OnUserCreate(this);
        
        
        // ================= HardLevels =================
        Levels.push_back(new Hard1Level);
        Levels.back()->OnUserCreate(this);
        
        Levels.push_back(new Hard2Level);
        Levels.back()->OnUserCreate(this);
        
        Levels.push_back(new Hard3Level);
        Levels.back()->OnUserCreate(this);
        
        
        Levels.push_back(new SandboxLevel);
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
                DrawDecal({0.0f, 0.0f}, TheGreatMachineImage->Decal(), {ScaleX, ScaleY}, olc::Pixel(255, 255, 255, 32));
                SetDrawTarget(GameLayer);
                const char *LevelSelectText = "Select Level";
                
                olc::vi2d ScreenCentre = ScreenSize() / 2;
                int TextScale = 4;
                olc::vi2d LineSize =  GetTextSize(LevelSelectText) * TextScale;
                DrawString(ScreenCentre.x - LineSize.x / 2, 1 * LineSize.y / 2, LevelSelectText, olc::WHITE, TextScale);
                
                if(GetKey(olc::ESCAPE).bPressed) {
                    ExitAttempt = true;
                }
                
                for (int i = 0; i < Levels.size(); ++i) {
                    int LevelTextScale = 3;
                    
                    char *RawText;
                    olc::Pixel TextColor;
                    if (Levels[i]->LevelCompleted) {
                        RawText= Levels[i]->LevelCompleteName;
                        TextColor = olc::GREEN;
                    } else {
                        RawText= Levels[i]->LevelName;
                        TextColor = olc::WHITE;
                    }
                    std::vector<std::string> lines;
                    std::vector<olc::vi2d> LineSizes;
                    
                    int start = 0;
                    for(int i = 0; i < strlen(RawText) + 1; ++i) {
                        if (RawText[i] == '\n' || RawText[i] == '\0') {
                            lines.emplace_back(&RawText[start], &RawText[i]);
                            LineSizes.push_back(GetTextSize(lines.back()) * LevelTextScale);
                            start = i + 1;
                        }
                    }
                    
                    int textMaxXSize = std::max_element(std::begin(LineSizes), std::end(LineSizes),
                                                        [] (olc::vi2d size1, olc::vi2d size2) {
                                                            return size1.x < size2.x;
                                                        })->x;
                    olc::vi2d textPos = {
                        (1 + i / 7) * (ScreenWidth() / 4),
                        (1 + i % 7) * (ScreenHeight() / (1 + 7))
                    };
                    
                    olc::vi2d textTLeft = {
                        textPos.x - textMaxXSize / 2,
                        textPos.y - (int)(LineSizes.size() * LineSizes[0].y) / 2
                    };
                    
                    olc::vi2d textBRight = {
                        textPos.x + textMaxXSize / 2,
                        textPos.y + (int)(LineSizes.size() * LineSizes[0].y) / 2
                    };
                    
                    olc::vi2d MousePos = GetMousePos();
                    if (MousePos.x >= textTLeft.x && MousePos.x <= textBRight.x &&
                        MousePos.y >= textTLeft.y && MousePos.y <= textBRight.y &&
                        !ExitAttempt)
                    {
                        TextColor = olc::BLUE;
                        if (GetMouse(0).bPressed) {
                            GameState = GAME_STATE_EDITOR;
                            ActiveLevel = Levels[i];
                        }
                    }
                    int yPos = textTLeft.y;
                    for(int l = 0; l < lines.size(); ++l)
                    {
                        DrawString({textPos.x - LineSizes[l].x / 2, yPos + l * LineSizes[l].y},
                                   lines[l], TextColor, LevelTextScale);
                    }
                }
                
                if (ExitAttempt) {
                    int TextScale = 3;
                    const char *QueryText = "Are you sure you want to exit?";
                    const char *YesButton = "Yes";
                    const char *NoButton = "No";
                    olc::vi2d QueryTextSize = GetTextSize(QueryText) * TextScale;
                    olc::vi2d buttonSize;
                    
                    if(strlen(YesButton) > strlen(NoButton)) {
                        buttonSize = GetTextSize(YesButton) * TextScale;
                    } else {
                        buttonSize = GetTextSize(NoButton) * TextScale;
                    }
                    olc::vi2d yesTextSize = GetTextSize(YesButton) * TextScale;
                    olc::vi2d noTextSize = GetTextSize(NoButton) * TextScale;
                    
                    olc::vi2d PopUpSize = QueryTextSize + olc::vi2d{0, QueryTextSize.y * 3};
                    olc::vi2d PopUpPos = ScreenSize() / 2 - PopUpSize / 2;
                    
                    olc::vi2d yesButtonPos = PopUpPos + PopUpSize - olc::vi2d{2 * yesTextSize.x, 2 * QueryTextSize.y};
                    olc::vi2d noButtonPos = PopUpPos + olc::vi2d{2 * noTextSize.x , 2 * QueryTextSize.y};
                    
                    olc::Pixel yesButtonCol = olc::WHITE;
                    if(GetMouseX() >=yesButtonPos.x && GetMouseX() <= yesButtonPos.x + yesTextSize.x &&
                       GetMouseY() >=yesButtonPos.y && GetMouseY() <= yesButtonPos.y + yesTextSize.y)
                    {
                        if(GetMouse(0).bPressed) {
                            exit(0);
                        }
                        yesButtonCol = olc::BLUE;
                    }
                    
                    olc::Pixel noButtonCol = olc::WHITE;
                    if(GetMouseX() >= noButtonPos.x && GetMouseX() <= noButtonPos.x + noTextSize.x &&
                       GetMouseY() >= noButtonPos.y && GetMouseY() <= noButtonPos.y + noTextSize.y)
                    {
                        noButtonCol = olc::BLUE;
                        if(GetMouse(0).bPressed) {
                            ExitAttempt = false;
                        }
                    }
                    
                    FillRect(PopUpPos, PopUpSize, olc::VERY_DARK_GREY);
                    DrawString(PopUpPos, QueryText, olc::WHITE, TextScale);
                    DrawString(yesButtonPos, YesButton, yesButtonCol, TextScale);
                    DrawString(noButtonPos, NoButton, noButtonCol, TextScale);
                }
                
            } break;
            
            case GAME_STATE_EDITOR: {
                SetDrawTarget(BGLayer);
                float ScaleX = (float)(ScreenWidth() - 300) / (float)TheGreatMachineImage->Sprite()->width;
                float ScaleY = (float)ScreenHeight() / (float)TheGreatMachineImage->Sprite()->height;
                DrawDecal({0.0f, 0.0f}, TheGreatMachineImage->Decal(), {ScaleX, ScaleY}, olc::Pixel(255, 255, 255, 32));
                SetDrawTarget(GameLayer);
                ActiveLevel->OnUserUpdate(fElapsedTime);
                if(GetKey(olc::ESCAPE).bPressed) {
                    GameState = GAME_STATE_LEVEL_SELECT;
                }
            } break;
        }
        return true;
    }
    
    bool ExitAttempt = false;
    
    void DrawUI()
    {
        BeginImGuiFrame();
        if (GameState == GAME_STATE_EDITOR)
        {
            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("Main")) {
                    if (ImGui::MenuItem("Level Select", "Esc")) {
                        GameState = GAME_STATE_LEVEL_SELECT;
                    }
                    
                    ImGui::MenuItem("Controls", 0, &ShowControls);
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }
            
            if (ShowControls) {
                if(ImGui::Begin("Controls", &ShowControls, ImGuiWindowFlags_NoDocking)) {
                    ImGui::TextWrapped("Editor Controls Window:\n"
                                       "Use the \"Control Modes\" list to select control mode.\n\n"
                                       "PLACE_COMPONENTS: Select a compnent from one of the components list. Left Click in the world to place. (Hold CTRL to place multiple ,Right click to cancel).\nClick on a node in the world to move it. (Right Click to cancel)\n\n"
                                       "PLACE_CONNECTIONS: Click on a component, then click on the component you want to connect it to. The component which is taking the input must be the second click. Right Click to remove all connections to a component.\n\n"
                                       "INTERACT: Click to toggle the values of the \"IO_BIT\" components.\n\n"
                                       "DELETE_COMPONENTS: Click a component to detete it (Hold CTRL to also delete all of its children). Right Click to delete all connections to a component.\n\n"
                                       "Simulation Window:\n"
                                       "This window tells you whether the level has been completed.\nThe \"Evaluate\" button tests your circuit and will halt on the first failed state.\nThe \"Simulate\" button runs a full input sequence without halting. (This does NOT check if the level is complete! Use \"Evaluate\" for this)");
                }
                ImGui::End();
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
    
    bool ShowControls = false;
    
    std::vector<Level *> Levels;
    Level *ActiveLevel;
    CircuitEditor *editor;
    olc::imgui::PGE_ImGUI pge_imgui;
};

int main(int argc, char *argv[])
{
    bool fullScreen = false;
    printf("argc: %d\n", argc);
    for(int i = 0; i < argc; ++ i) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    
    if(argc == 2) {
        if(strcmp(argv[1], "true") == 0) {
            fullScreen = true;
        }
    }
    TheGreatMachine demo;
    if (demo.Construct(1280, 720, 1, 1, fullScreen))
        demo.Start();
    
    return 0;
}
