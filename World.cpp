#include "World.h"

World::World()
{
    sGreeting = "Welcome to the world";
}

World::~World() {}


void World::GenerateWorld()
{
    StaticGenMap();
}


void World::StaticGenMap()
{
    m_pMapSprite = std::make_unique<olc::Renderable>();
    m_pMapSprite->Load("./res/sprites/tilemap.png");
    sMapString =
        "################################################################################################################################"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "#..............................................................................................................................#"
        "################################################################################################################################";

    vSize = { 128, 24 };

    for (int i = 0; i < vSize.y; i++)
    {
        vMap.push_back(std::vector<char>());
        for (int j = 0; j < vSize.x; j++)
        {
            vMap[i].push_back(sMapString[i * vSize.x + j]);
        }
    }
}


void World::PrintWorld()
{
    // 2D vector
    for (int i = 0; i < vMap.size(); i++)
    {
        for (int j = 0; j < vMap[i].size(); j++)
        {
            std::cout << vMap[i][j];
        }
        std::cout << std::endl;
    }
}


// TODO - Make better (right now just loops through string and finds the first emtpy cell)
olc::vf2d World::FindSpawnableCell()
{
    // String
    for (int i = 0; i < vSize.y; i++)
    {
        for (int j = 0; j < vSize.x; j++)
        {
            if (sMapString[i * vSize.x + j] == '.')
            {
                return { j + 0.5f, i + 0.5f };
            }
        }
    }
}


olc::vi2d World::GetSize()
{
    return vSize;
}


void World::DrawMap(olc::TileTransformedView* tv, olc::vf2d vCursorCoords)
{
    olc::vi2d vCursorTile = vCursorCoords;
    olc::vi2d vTL = tv->GetTopLeftTile().max({ 0, 0 });
    olc::vi2d vBR = tv->GetBottomRightTile().min(vSize);
    olc::vi2d vTile;
    olc::Pixel cOutlineColor = olc::BLACK;
    int iTransparency = 255;
    //tv->DrawRect(vCursorTile, { 1.0f, 1.0f }, olc::Pixel(255, 255, 255, 200));
    for (vTile.y = vTL.y; vTile.y < vBR.y; vTile.y++)
    {
        for (vTile.x = vTL.x; vTile.x < vBR.x; vTile.x++)
        {
            if (vTile.x == vCursorTile.x && vTile.y == vCursorTile.y)
                iTransparency = 200;
            else
                iTransparency = 255;
            switch(vMap[vTile.y][vTile.x])
            //switch(sMapString[vTile.y * vSize.x + vTile.x])
            {
                case('#'):
                    tv->FillRect(vTile, { 1.0f, 1.0f }, olc::Pixel(255, 255, 255, iTransparency));
                    tv->DrawRect(vTile, { 1.0f, 1.0f }, cOutlineColor);
                    break;

                case('+'):
                    tv->FillRect(vTile, { 1.0f, 1.0f }, olc::Pixel(255, 0, 0, iTransparency));
                    tv->DrawRect(vTile, { 1.0f, 1.0f }, cOutlineColor);
                    break;

                case('='):
                    tv->FillRect(vTile, { 1.0f, 1.0f }, olc::Pixel(0, 0, 255, iTransparency));
                    tv->DrawRect(vTile, { 1.0f, 1.0f }, cOutlineColor);
                    break;
                //tv->DrawPartialDecal(vTile, m_pMapSprite->Decal(), { 0.0f, 0.0f }, { 16.0f, 16.0f }, { 2.1f, 2.1f });
            }
        }
    }
    tv->DrawRect(vCursorTile, { 1.0f, 1.0f }, olc::DARK_GREY);
}


olc::vf2d World::FindRandomOpenSpot()
{
    auto ti = std::chrono::system_clock::now().time_since_epoch().count();
    srand(ti);
    olc::vf2d randCoord = { (int)rand() % vSize.x, (int)rand() % vSize.y };
    int index = randCoord.y * vSize.x + randCoord.x;
    while (index < sMapString.length() && sMapString[index] != '.')
    {
        randCoord = { rand() % vSize.x, rand() % vSize.y };
        index = randCoord.y * vSize.x + randCoord.x;
    }

    return randCoord;
}


void World::AddSolidTile(olc::vi2d index, TileType tTileType)
{
    char cTile;
    switch(tTileType)
    {
        case(0):
            cTile = '#';
            break;
        case(1):
            cTile = '+';
            break;
        case(2):
            cTile = '=';
            break;
    }
    if (index.x >= vSize.x)
    {
        int iDeltaSize = index.x - vSize.x;
        vSize.x += iDeltaSize;
        // 2D vector
        for (int j = 0; j < iDeltaSize; j++)
        {
            vMap[0].push_back('#');
        }
        for (int i = 1; i < vMap.size() - 1; i++)
        {
            vMap[i][vSize.x - iDeltaSize - 1] = '.';
            for (int j = 0; j < iDeltaSize; j++)
            {
                vMap[i].push_back('.');
            }
            vMap[i][vSize.x - 1] = '#';
        }
        for (int j = 0; j < iDeltaSize; j++)
        {
            vMap[vMap.size() - 1].push_back('#');
        }
    }
    else
    {
        if (index.y >= 0 && index.y < vSize.y && vMap[index.y][index.x] == '.')
            vMap[index.y][index.x] = cTile;
    }
}


void World::RemoveSolidTile(olc::vi2d index)
{
    int i = index.y * vSize.x + index.x;
    if (index.y >= 0 && index.y < vSize.y && index.x < vSize.x && vMap[index.y][index.x] != '.')
        vMap[index.y][index.x] = '.';
}


void World::SaveMapToFile()
{
    std::ofstream fout;
    int i = 0;
    std::string filename = "";
    while(true)
    {
        filename = "maps/world_map_" + std::to_string(i) + ".txt";
        if (!std::filesystem::exists(filename))
        {
            fout.open(filename);
            break;
        }
        i++;
    }

    for (int i = 0; i < vSize.y; i++)
    {
        for (int j = 0; j < vSize.x; j++)
        {
            fout << sMapString[i * vSize.x + j];
        }
        fout << std::endl;
    }
    fout.close();
}


void World::ResetMap()
{
    vSize = { 128, 24 };
    for (int i = 0; i < vSize.y; i++)
    {
        std::cout << vMap[i].size() << std::endl;
        vMap[i].resize(vSize.x);
        for (int j = 0; j < vSize.x; j++)
        {
            vMap[i][j] = sMapString[i * vSize.x + j];
        }
    }
}
