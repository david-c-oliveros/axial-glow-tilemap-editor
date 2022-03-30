#include <iostream>
#include <vector>
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#include "World.h"
#include "Button.h"
#include "MathUtils.h"


enum AppState
{
    MENU,
    EDIT
};


class TileMapEditor : public olc::PixelGameEngine
{
    public:
        TileMapEditor()
        {
            sAppName = "TileMapEditor";
        }

    private:
        AppState iCurAppState;
        TileType iCurColorIndex;
        std::vector<color_t> vColors;
        olc::vf2d vCursorCoords;

        /***********************************/
        /*              Menu               */
        /***********************************/
        Button cOpenButton = Button();

        /***********************************/
        /*              Edit               */
        /***********************************/

        int iCursorSize = 1;
        const int iMaxCursorSize = 8;

        olc::TileTransformedView tv;
        World cWorld = World();
        int iGameTick;
        Button cSaveButton = Button();
        Button cResetButton = Button();
        Button cIncreaseSizeButton = Button();
        Button cDecreaseSizeButton = Button();
        std::vector<Button> vTileTypes;
        bool bOnUI = false;

    public:
        bool OnUserCreate() override
        {
            /***********************************/
            /*              Menu               */
            /***********************************/
            cOpenButton = Button(olc::vf2d((ScreenWidth() / 2) - 20.0f, (ScreenHeight() / 2) - 8.0f),
                                 olc::vf2d(80.0f, 16.0f), "Open Map");

            /***********************************/
            /*              Edit               */
            /***********************************/
            iCurAppState = EDIT;
            iCurColorIndex = WHITE;
            vColors = {
                { olc::WHITE, "White" },
                { olc::RED,   "Red"   },
                { olc::BLUE,  "Blue"  }
            };
            tv = olc::TileTransformedView({ ScreenWidth(), ScreenHeight() }, { 32, 32 });
            cWorld.GenerateWorld();
            iGameTick = 0;
            cIncreaseSizeButton = Button( olc::vf2d(ScreenWidth() - 64.0f, 128.0f), olc::vf2d(16.0, 16.0f), "+" );
            cDecreaseSizeButton = Button( olc::vf2d(ScreenWidth() - 32.0f, 128.0f), olc::vf2d(16.0, 16.0f), "-" );
            olc::vf2d p = { (ScreenWidth() / 2) - 84, ScreenHeight() - 64 };

            cSaveButton = Button(p, olc::vf2d(80.0f, 16.0f), "Save Map");
            p.x += 88.0f;
            cResetButton = Button(p, olc::vf2d(80.0f, 16.0f), "Reset Map");
            p = olc::vf2d(10.0f, 15.0f);
            vTileTypes.push_back(Button(p, olc::vf2d(80.0f, 16.0f), "Type 1"));
            p.y += 20.0f;
            vTileTypes.push_back(Button(p, olc::vf2d(80.0f, 16.0f), "Type 2"));
            p.y += 20.0f;
            vTileTypes.push_back(Button(p, olc::vf2d(80.0f, 16.0f), "Type 3"));
            p.y += 40.0f;

            return true;
        }


        bool OnUserUpdate(float fElapsedTime) override
        {
            if (iCurAppState == EDIT)
            {
                if (GetKey(olc::Key::TAB).bReleased) iCurAppState = MENU;
            }
            else if (iCurAppState == MENU)
            {
                if (GetKey(olc::Key::TAB).bReleased) iCurAppState = EDIT;
            }

            switch(iCurAppState)
            {
                case(EDIT):
                    HandleEditInput();
                    RenderEdit();
                    cSaveButton.Update();
                    cResetButton.Update();
                    cIncreaseSizeButton.Update();
                    cDecreaseSizeButton.Update();
                    for (int i = 0; i < vTileTypes.size(); i++)
                    {
                        vTileTypes[i].Update();
                    }
                    break;

                case(MENU):
                    HandleMenuInput();
                    RenderMenu();
                    cOpenButton.Update();
                    break;

                default:
                    break;
            }
            iGameTick++;

            return true;
        }


        void RenderEdit()
        {
            Clear(olc::VERY_DARK_BLUE);
            std::string sSelectedTileType = vColors[iCurColorIndex].sName;
            olc::vf2d vStrPos = { 10.0f, 85.0f };
            DrawStringDecal(vStrPos, vColors[iCurColorIndex].sName, vColors[iCurColorIndex].pColor);
            cIncreaseSizeButton.DrawSelf(this);
            cDecreaseSizeButton.DrawSelf(this);
            cWorld.DrawMap(&tv, vCursorCoords, iCursorSize);
            cSaveButton.DrawSelf(this);
            cResetButton.DrawSelf(this);
            for (int i = 0; i < vTileTypes.size(); i++)
            {
                vTileTypes[i].DrawSelf(this);
            }
        }


        void RenderMenu()
        {
            Clear(olc::VERY_DARK_BLUE);
            cOpenButton.DrawSelf(this);
        }


        void HandleMenuInput()
        {
            olc::vf2d vCursorScreenCoords = GetMousePos();
            if (cOpenButton.ButtonHover(vCursorScreenCoords))
            {
                bOnUI = true;
                if (GetMouse(0).bPressed)
                {
                    cOpenButton.Pressed();
                    cWorld.LoadMapFromFile();
                }
            }
        }


        void HandleEditInput()
        {
            if (GetMouse(2).bPressed) tv.StartPan(GetMousePos());
            if (GetMouse(2).bHeld) tv.UpdatePan(GetMousePos());
            if (GetMouse(2).bReleased) tv.EndPan(GetMousePos());
            if (GetMouseWheel() > 0) tv.ZoomAtScreenPos(2.0f, GetMousePos());
            if (GetMouseWheel() < 0) tv.ZoomAtScreenPos(0.5f, GetMousePos());

            olc::vf2d vCursorScreenCoords = GetMousePos();
            vCursorCoords = tv.ScreenToWorld(vCursorScreenCoords);
            bOnUI = false;
            for (int i = 0; i < vTileTypes.size(); i++)
            {
                if (vTileTypes[i].ButtonHover(vCursorScreenCoords))
                {
                    bOnUI = true;
                    if (GetMouse(0).bPressed)
                    {
                        vTileTypes[i].Pressed();
                        iCurColorIndex = TileType(i);
                        std::cout << "Selected tile " << i + 1 << std::endl;
                    }
                }
            }

            if (cSaveButton.ButtonHover(vCursorScreenCoords))
            {
                bOnUI = true;
                if (GetMouse(0).bPressed)
                {
                    // Save to file
                    std::cout << "Saving map to file" << std::endl;
                    cSaveButton.Pressed();
                    cWorld.SaveMapToFile();
                }
            }

            if (cResetButton.ButtonHover(vCursorScreenCoords))
            {
                bOnUI = true;
                if (GetMouse(0).bPressed)
                {
                    std::cout << "Reseting map" << std::endl;
                    cResetButton.Pressed();
                    cWorld.ResetMap();
                }
            }

            if (cIncreaseSizeButton.ButtonHover(vCursorScreenCoords))
            {
                bOnUI = true;
                if (GetMouse(0).bPressed)
                {
                    if (iCursorSize == 1)
                        iCursorSize = 2;
                    else
                        iCursorSize += 2;

                    if (iCursorSize > iMaxCursorSize)
                        iCursorSize = iMaxCursorSize;
                    cIncreaseSizeButton.Pressed();
                }
            }

            if (cDecreaseSizeButton.ButtonHover(vCursorScreenCoords))
            {
                bOnUI = true;
                if (GetMouse(0).bPressed)
                {
                    iCursorSize -= 2;
                    if (iCursorSize < 2)
                        iCursorSize = 1;
                    cDecreaseSizeButton.Pressed();
                }
            }

            if (!bOnUI)
            {
                if (GetMouse(0).bHeld)
                {
                    cWorld.AddSolidTile(vCursorCoords, iCurColorIndex, iCursorSize);
                }

                if (GetMouse(1).bHeld)
                {
                    cWorld.RemoveSolidTile(tv.ScreenToWorld(vCursorScreenCoords), iCursorSize);
                }
            }
        }
};



int main()
{
    TileMapEditor tme;
    if (tme.Construct(720, 480, 2, 2, false, true))
        tme.Start();

    return 0;
}
