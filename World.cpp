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
    sMap =
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
        "################################################################################################################################";

    vSize = { 128, 16 };
}


void World::PrintWorld()
{
    for (int i = 0; i < vSize.y; i++)
    {
        for (int j = 0; j < vSize.x; j++)
        {
            std::cout << sMap[i * vSize.x + j];
        }
        std::cout << std::endl;
    }
}


// TODO - Make better (right now just loops through string and finds the first emtpy cell)
olc::vf2d World::FindSpawnableCell()
{
    for (int i = 0; i < vSize.y; i++)
    {
        for (int j = 0; j < vSize.x; j++)
        {
            if (sMap[i * vSize.x + j] == '.')
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


void World::DrawMap(olc::TileTransformedView* tv)
{
    olc::vi2d vTL = tv->GetTopLeftTile().max({ 0, 0 });
    olc::vi2d vBR = tv->GetBottomRightTile().min(vSize);
    olc::vi2d vTile;
    for (vTile.y = vTL.y; vTile.y < vBR.y; vTile.y++)
    {
        for (vTile.x = vTL.x; vTile.x < vBR.x; vTile.x++)
        {
            if (sMap[vTile.y * vSize.x + vTile.x] == '#')
            {
                tv->FillRect(vTile, { 1.0f, 1.0f }, olc::WHITE);
                tv->DrawRect(vTile, { 1.0f, 1.0f }, olc::BLACK);
                //tv->DrawPartialDecal(vTile, m_pMapSprite->Decal(), { 0.0f, 0.0f }, { 16.0f, 16.0f }, { 2.1f, 2.1f });
            }
        }
    }
}


olc::vf2d World::FindRandomOpenSpot()
{
    auto ti = std::chrono::system_clock::now().time_since_epoch().count();
    srand(ti);
    olc::vf2d randCoord = { (int)rand() % vSize.x, (int)rand() % vSize.y };
    int index = randCoord.y * vSize.x + randCoord.x;
    while (index < sMap.length() && sMap[index] == '#')
    {
        randCoord = { rand() % vSize.x, rand() % vSize.y };
        index = randCoord.y * vSize.x + randCoord.x;
    }

    return randCoord;
}


void World::AddSolidTile(olc::vi2d index)
{
    int i = index.y * vSize.x + index.x;
    if (i < sMap.length() && sMap[i] == '.')
        sMap[i] = '#';
}


void World::RemoveSolidTile(olc::vi2d index)
{
    int i = index.y * vSize.x + index.x;
    if (i < sMap.length() && sMap[i] == '#')
        sMap[i] = '.';
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
            fout << sMap[i * vSize.x + j];
        }
        fout << std::endl;
    }
    fout.close();
}
