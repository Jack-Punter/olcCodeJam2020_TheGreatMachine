#ifndef CIRCUIT_EDITOR_H
#define CIRCUIT_EDITOR_H
#include "CircuitNode.h"

struct CircuitEditor {
    void OnUserCreate(olc::PixelGameEngine *_pge)
    {
        pge = _pge;
        LogicComponentRenderable = new olc::Renderable;
        LogicComponentRenderable->Load("..\\misc\\LogicComponents.png");
        
        IoComponentRenderable = new olc::Renderable;
        IoComponentRenderable->Load("..\\misc\\IOComponents.png");
    }
    
    CircuitNode *NewNode = 0;
    void OnUserUpdate(float fElapsedTime)
    {
        for(const auto& t : CircuitTrees) {
            t->Draw();
        }
        
        if (CurrentControlMode == PLACE_COMPONENTS)
        {
            if ((CurrentLogicComponent != LOGIC_NONE) || (CurrentIoComponent != IO_NONE))
            {
                if (!NewNode) {
                    if (CurrentLogicComponent != LOGIC_NONE) {
                        printf("Allocating %s node\n", LogicComponentStrings[(int)CurrentLogicComponent]);
                        NewNode = CreateHeldNode<LogicCircuitNode>(pge, CurrentLogicComponent, LogicComponentSize,
                                                                   LogicComponentRenderable);
                    } else if (CurrentIoComponent != IO_NONE) {
                        printf("Allocating %s node\n", IoComponentStrings[(int)CurrentIoComponent]);
                        NewNode = CreateHeldNode<IoCircuitNode>(pge, CurrentIoComponent, IoComponentSize,
                                                                IoComponentRenderable);
                    }
                } else {
                    NewNode->pos = pge->GetMousePos() - NewNode->SpriteSize;
                    bool IsColliding = false;
                    olc::Pixel Tint = GetHeldTintColor(NewNode, &IsColliding);
                    NewNode->Draw(Tint);
                    
                    if(pge->GetMouse(0).bPressed && !IsColliding) {
                        NewNode->Held = false;
                        CircuitTrees.push_back(NewNode);
                        NewNode = 0;
                        if (!pge->GetKey(olc::CTRL).bHeld) {
                            CurrentIoComponent = IO_NONE;
                            CurrentLogicComponent = LOGIC_NONE;
                        }
                    }
                    if(pge->GetMouse(1).bPressed) {
                        delete NewNode;
                        NewNode = 0;
                        CurrentIoComponent = IO_NONE;
                        CurrentLogicComponent = LOGIC_NONE;
                    }
                }
            }
            else // We aren't placing new node
            {
                if (!FirstClick && pge->GetMouse(0).bPressed) {
                    CircuitNode *node = GetNodeAtLocation(pge->GetMousePos());
                    if (node && !node->IsStatic) {
                        FirstClick = node;
                        FirstClickPos = node->pos;
                        FirstClick->Held = true;
                    }
                }
                else if (FirstClick) {
                    FirstClick->pos = pge->GetMousePos() - FirstClick->SpriteSize;
                    bool IsColliding;
                    olc::Pixel TintCol = GetHeldTintColor(FirstClick, &IsColliding);
                    FirstClick->Draw(TintCol);
                    
                    if (pge->GetMouse(1).bPressed) {
                        FirstClick->pos = FirstClickPos;
                        FirstClick->Held = false;
                        FirstClick = 0;
                    }
                    
                    bool MousePressed = pge->GetMouse(0).bPressed;
                    if (MousePressed && !IsColliding) {
                        FirstClick->Held = false;
                        FirstClick = 0;
                    }
                }
            }
        }
        else if(CurrentControlMode == PLACE_CONNECTIONS)
        {
            if(pge->GetMouse(0).bPressed) {
                CircuitNode *node = GetNodeAtLocation(pge->GetMousePos());
                if (node && node->SpriteIndex != (int)LOGIC_NONE) {
                    if(!FirstClick) {
                        FirstClick = node;
                    } else  {
                        if(node->ConnectChild(FirstClick)){
                            RemoveErase(&CircuitTrees, FirstClick);
                            //Reset the clicks#
                            FirstClick  = 0;
                        }
                    }
                }
            }
            
            if(!FirstClick && pge->GetMouse(1).bPressed) {
                CircuitNode *node = GetNodeAtLocation(pge->GetMousePos());
                if(node) {
                    std::vector<CircuitNode *> children = node->GetChildren();
                    CircuitTrees.reserve(CircuitTrees.size() + children.size());
                    CircuitTrees.insert(std::end(CircuitTrees), std::begin(children), std::end(children));
                    for(auto* c : children) {
                        c->RemoveParent(node);
                        node->RemoveInput(c);
                    }
                    // NOTE(jack): If Ctrl is pressed and the node is not a root, add it as a root
                    // and remove it from its parents;
                    if (std::find(std::begin(CircuitTrees), std::end(CircuitTrees), node) == std::end(CircuitTrees))
                    {
                        node->RemoveFromParents();
                        CircuitTrees.push_back(node);
                    }
                }
            }
            
            if (FirstClick) {
                pge->DrawLine(FirstClick->pos + FirstClick->SpriteSize, pge->GetMousePos());
            }
            if (FirstClick && pge->GetMouse(1).bPressed) {
                FirstClick = 0;
            }
            
        } else if (CurrentControlMode == INTERACT) {
            if(pge->GetMouse(0).bPressed) {
                CircuitNode *node = GetNodeAtLocation(pge->GetMousePos());
                if(node) {
                    node->OnClickEvent();
                }
            }
        } else if (CurrentControlMode == DELETE_COMPONENTS) {
            if (pge->GetMouse(0).bPressed && !pge->GetKey(olc::CTRL).bHeld) {
                CircuitNode *node = GetNodeAtLocation(pge->GetMousePos());
                if(node && !node->IsStatic) {
                    std::vector<CircuitNode *> children = node->GetChildren();
                    CircuitTrees.reserve(CircuitTrees.size() + children.size());
                    CircuitTrees.insert(std::end(CircuitTrees), std::begin(children), std::end(children));
                    
                    for(auto* c : children) {
                        c->RemoveParent(node);
                        node->RemoveInput(c);
                    }
                    RemoveErase(&CircuitTrees, node);
                    delete node;
                }
            }
            if(pge->GetMouse(0).bPressed && pge->GetKey(olc::CTRL).bHeld) {
                CircuitNode *node = GetNodeAtLocation(pge->GetMousePos());
                if(node && !node->IsStatic) {
                    std::vector<CircuitNode*> StaticChildren = node->RemoveStaticChildrenRecursive();
                    CircuitTrees.reserve(CircuitTrees.size() + StaticChildren.size());
                    CircuitTrees.insert(std::end(CircuitTrees), std::begin(StaticChildren), std::end(StaticChildren));
                    delete node;
                    RemoveErase(&CircuitTrees, node);
                }
            }
            if (pge->GetMouse(1).bPressed) {
                CircuitNode *node = GetNodeAtLocation(pge->GetMousePos());
                if(node) {
                    std::vector<CircuitNode *> children = node->GetChildren();
                    CircuitTrees.reserve(CircuitTrees.size() + children.size());
                    CircuitTrees.insert(std::end(CircuitTrees), std::begin(children), std::end(children));
                    for(auto* c : children) {
                        c->RemoveParent(node);
                        node->RemoveInput(c);
                    }
                    // NOTE(jack): If Ctrl is pressed and the node is not a root, add it as a root
                    // and remove it from its parents;
                    if (std::find(std::begin(CircuitTrees), std::end(CircuitTrees), node) == std::end(CircuitTrees))
                    {
                        node->RemoveFromParents();
                        CircuitTrees.push_back(node);
                    }
                }
            }
        }
    }
    
    void DrawUI()
    {
        //Create and react to your UI here
        //ImGui::ShowDemoWindow();
        if (ImGui::Begin("Editor Controls"))
        {
            //ImGui::PushItemWidth(-1);
            if(ImGui::Button("Clear")) {
                ClearEditor();
            }
            
            ImGui::Text("Control Modes:");
            ImGui::PushItemWidth(-1);
            ImGui::ListBox("ControlModes", &(int)CurrentControlMode, ControlModeStrings, ArrayCount(ControlModeStrings), ArrayCount(ControlModeStrings));
            if(CurrentControlMode == PLACE_COMPONENTS)
            {
                ImGui::Text("Logic Components:");
                ImGui::PushItemWidth(-1);
                LogicComponent OldLogicComponent = CurrentLogicComponent;
                ImGui::ListBox("LogicComponents", &(int)CurrentLogicComponent, LogicComponentStrings, ArrayCount(LogicComponentStrings), ArrayCount(LogicComponentStrings));
                if(OldLogicComponent != CurrentLogicComponent) {
                    if (NewNode) {
                        delete NewNode;
                        NewNode = 0;
                    }
                    CurrentIoComponent = IO_NONE;
                }
                
                ImGui::Text("IO Components:");
                ImGui::PushItemWidth(-1);
                IoComponent OldIoComponent = CurrentIoComponent;
                ImGui::ListBox("IoComponents", &(int)CurrentIoComponent, IoComponentStrings, ArrayCount(IoComponentStrings), ArrayCount(IoComponentStrings));
                if(OldIoComponent != CurrentIoComponent) {
                    if (NewNode) {
                        delete NewNode;
                        NewNode = 0;
                    }
                    CurrentLogicComponent = LOGIC_NONE;
                }
            }
            else
            {
                if (NewNode) {
                    delete NewNode;
                    NewNode = 0;
                }
                CurrentIoComponent = IO_NONE;
                CurrentLogicComponent = LOGIC_NONE;
            }
        }
        ImGui::End();
    }
    
    void ClearEditor() {
        for (int i = 0; i < CircuitTrees.size(); ++i) {
            CircuitNode **pnode = &CircuitTrees[i];
            if (!(*pnode)->IsStatic) {
                std::vector<CircuitNode *> tmp = (*pnode)->RemoveStaticChildrenRecursive();
                CircuitTrees.reserve(CircuitTrees.size() + tmp.size());
                CircuitTrees.insert(std::end(CircuitTrees), std::begin(tmp), std::end(tmp));
                
                delete *pnode;
                *pnode = nullptr;
            } else {
                std::vector<CircuitNode *> tmp = (*pnode)->GetChildren();
                for(CircuitNode *t : tmp) {
                    if(!t->IsStatic) {
                        (*pnode)->RemoveInput(t);
                        delete t;
                    }
                }
            }
            RemoveErase(&CircuitTrees, nullptr);
        }
    };
    
    void ClearEditorNonStatic() {
        for(CircuitNode *&node : CircuitTrees) {
            if (!node->IsStatic) {
                delete node;
                node = 0;
            }
        }
        RemoveErase(&CircuitTrees, (CircuitNode *)0);
    };
    
    olc::Pixel GetHeldTintColor(CircuitNode *NewNode, bool* IsColliding)
    {
        *IsColliding = false;
        olc::Pixel Tint = olc::Pixel(0, 255, 0, 64);
        for(auto *Node : CircuitTrees) {
            std::vector nodes = Node->IntersectionsWithRect(NewNode->pos, NewNode->pos + 2 * NewNode->SpriteSize);
            RemoveErase(&nodes, NewNode);
            if (nodes.size() > 0) {
                if(!(nodes.size() == 1 && nodes[0] == NewNode)) {
                    Tint = olc::Pixel(255, 0, 0, 64);
                    *IsColliding = true;
                }
            }
        }
        
        if(pge->GetMouseX() >= SideBarPos.x - NewNode->SpriteSize.x || pge->GetMouseX() < NewNode->SpriteSize.x ||
           pge->GetMouseY() >= pge->ScreenHeight() - NewNode->SpriteSize.y || pge->GetMouseY() < NewNode->SpriteSize.y)
        {
            Tint = olc::Pixel(255, 0, 0, 64);
            *IsColliding = true;
        }
        
        return Tint;
    }
    
    CircuitNode *GetNodeAtLocation(olc::vi2d pos) {
        for(auto *t : CircuitTrees) {
            CircuitNode* node = t->GetNodeAt(pos);
            if (node) return node;
        }
        return 0;
    }
    olc::PixelGameEngine *pge;
    olc::vf2d SideBarSize;
    olc::vf2d SideBarPos;
    bool ShowPartsList = true;
    
    std::vector<CircuitNode *> CircuitTrees;
    olc::vf2d LogicComponentSize{48.0f, 32.0f};
    olc::vf2d IoComponentSize{16.0f, 16.0f};
    
    olc::Renderable *LogicComponentRenderable;
    olc::Renderable *IoComponentRenderable;
    
    CircuitNode *FirstClick = 0;
    olc::vi2d FirstClickPos;
    CircuitNode *SecondClick = 0;
    
    LogicComponent CurrentLogicComponent = LOGIC_NONE;
    IoComponent CurrentIoComponent = IO_NONE;
    ControlMode CurrentControlMode = PLACE_COMPONENTS;
};

#endif //CIRCUIT_EDITOR_H
