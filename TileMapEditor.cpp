#include <iostream>
#include <vector>
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#include "World.h"
#include "Button.h"
#include "MathUtils.h"


class TileMapEditor : public olc::PixelGameEngine
{
    public:
        TileMapEditor()
        {
            sAppName = "TileMapEditor";
        }

    private:
        olc::TileTransformedView tv;
        World cWorld = World();
        int iGameTick;
        Button cSaveButton = Button();

    public:
        bool OnUserCreate() override
        {
            tv = olc::TileTransformedView({ ScreenWidth(), ScreenHeight() }, { 32, 32 });
            cWorld.GenerateWorld();
            iGameTick = 0;
            olc::vf2d p = { (ScreenWidth() / 2) - 32, ScreenHeight() - 64 };
            cSaveButton = Button(p, { 80, 16 }, "Save Map");

            return true;
        }


        bool OnUserUpdate(float fElapsedTime) override
        {
            HandleInput();
            Render();
            cSaveButton.Update();
            iGameTick++;

            return true;
        }


        void Render()
        {
            Clear(olc::VERY_DARK_BLUE);
            cWorld.DrawMap(&tv);
            cSaveButton.DrawSelf(this);
        }


        void HandleInput()
        {
            if (GetMouse(2).bPressed) tv.StartPan(GetMousePos());
            if (GetMouse(2).bHeld) tv.UpdatePan(GetMousePos());
            if (GetMouse(2).bReleased) tv.EndPan(GetMousePos());
            if (GetMouseWheel() > 0) tv.ZoomAtScreenPos(2.0f, GetMousePos());
            if (GetMouseWheel() < 0) tv.ZoomAtScreenPos(0.5f, GetMousePos());

            if (cSaveButton.ButtonHover(GetMousePos()))
            {
                if (GetMouse(0).bPressed)
                {
                    // Save to file
                    std::cout << "Saving map to file" << std::endl;
                    cSaveButton.Pressed();
                    cWorld.SaveMapToFile();
                }
            }
            else
            {
                if (GetMouse(0).bHeld)
                {
                    cWorld.AddSolidTile(tv.ScreenToWorld(GetMousePos()));
                }

                if (GetMouse(1).bHeld)
                {
                    cWorld.RemoveSolidTile(tv.ScreenToWorld(GetMousePos()));
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
