#pragma once
#include "rendering/MainScreenAssets.h"
#include "raylib.h"
#include <string_view>
namespace dungeon
{
    class Game;
    
    class Renderer
    {
    public:
        Renderer() = default;
        
        void draw(const Game& game) const;
        
        bool loadMainScreenAssets();
        
        void toggleGameInstructions() noexcept;
    
    private: //--------------------------------------------------------------------------------Private Starts HERE!
        // ---------------------------------------------------------------------
        // Main Screen design resolution
        // ---------------------------------------------------------------------
        static constexpr int ScreenWidth = 1280;
        static constexpr int ScreenHeight = 720;
        
        // ---------------------------------------------------------------------
        // Main Screen layout
        //
        // Design coordinates use a BOTTOM-LEFT origin.
        //
        // Raylib itself uses a TOP-LEFT origin.
        //
        // (real grapth planing uses Bottom-Left is Origin, and I'm an engeineer, so convert it!)
        //
        // The renderer converts between the two where necessary.
        // ---------------------------------------------------------------------
        
        //TitleArt
        static constexpr int TitleArtX = 0;
        static constexpr int TitleArtY = 670;
        static constexpr int TitleArtWidth = 1050;
        static constexpr int TitleArtHeight = 50;
        
        //Stats Title
        static constexpr int StatsTitleX = 1050;
        static constexpr int StatsTitleY = 670;
        static constexpr int StatsTitleWidth = 230;
        static constexpr int StatsTitleHeight = 50;
        
        //Map Renderer
        static constexpr int MapRenderX = 0;
        static constexpr int MapRenderY = 200;
        static constexpr int MapRenderWidth = 470;
        static constexpr int MapRenderHeight = 470;
        
        //POV Render
        static constexpr int PovRenderX = 470;
        static constexpr int PovRenderY = 200;
        static constexpr int PovRenderWidth = 580;
        static constexpr int PovRenderHeight = 470;
        
        //Stats Window
        static constexpr int StatsWindowX = 1050;
        static constexpr int StatsWindowY = 310;
        static constexpr int StatsWindowWidth = 230;
        static constexpr int StatsWindowHeight = 360;
        
        //Gear Title
        static constexpr int GearTitleX = 1050;
        static constexpr int GearTitleY = 260;
        static constexpr int GearTitleWidth = 230;
        static constexpr int GearTitleHeight = 50;
        
        //Gear Window
        static constexpr int GearWindowX = 1050;
        static constexpr int GearWindowY = 0;
        static constexpr int GearWindowWidth = 230;
        static constexpr int GearWindowHeight = 260;
        
        //Action Bar
        static constexpr int ActionBarX = 0;
        static constexpr int ActionBarY = 0;
        static constexpr int ActionBarWidth = 1050;
        static constexpr int ActionBarHeight = 200;
        
        // ---------------------------------------------------------------------
        // Map
        // ---------------------------------------------------------------------
        
        // The Map Render contains a 27 px frame on every side.
        static constexpr int MapFrameThickness = 27;
        
        static constexpr int VisibleMapWidth =
            MapRenderWidth - (MapFrameThickness * 2);
        
        static constexpr int VisibleMapHeight =
            MapRenderHeight - (MapFrameThickness * 2);
        
        // Temporary placeholder map tile size.
        // The final map tiles will be PNGs and use the 416x416 visual area.
        static constexpr int TemporaryMapTileSize = 16;
        
        // ---------------------------------------------------------------------
        // Statistics PNG slots
        // ---------------------------------------------------------------------
        
        static constexpr int StatIconSize = 32;
        static constexpr int StatNumberSize = 32;
        
        static constexpr int StatIconX = 1120;
        static constexpr int StatNumberX = 1178;
        
        static constexpr int StatAtkY = 615;
        
        static constexpr int StatStrY = 568;
        
        static constexpr int StatPrecY = 521;
        
        static constexpr int StatDmgY = 474;
        
        static constexpr int StatDefY = 427;
        
        static constexpr int StatToughY = 380;
        
        static constexpr int StatHpY = 333;
        
        // ---------------------------------------------------------------------
        // Enemy Statistics PNG slots
        //
        // Same geometry as Player Stats, but relative to the Enemy Stats
        // Window at X = 0, Y = 260.
        // ---------------------------------------------------------------------
        
        static constexpr int EnemyStatIconX = 70;
        static constexpr int EnemyStatNumberX = 128;
        
        static constexpr int EnemyStatAtkY = 565;
        static constexpr int EnemyStatStrY = 518;
        static constexpr int EnemyStatPrecY = 471;
        static constexpr int EnemyStatDmgY = 424;
        static constexpr int EnemyStatDefY = 377;
        static constexpr int EnemyStatToughY = 330;
        static constexpr int EnemyStatHpY = 283;
        
        // ---------------------------------------------------------------------
        // Gear PNG slots
        // ---------------------------------------------------------------------
        
        static constexpr int GearIconSize = 48;
        
        // Potion rows
        static constexpr int PotionIconX = 1100;
        static constexpr int PotionNumberX = 1182;
        
        static constexpr int HealthPotionY = 197;
        static constexpr int RagePotionY = 139;
        
        // Weapon / Armor
        static constexpr int WeaponIconX = 1095;
        static constexpr int ArmorIconX = 1187;
        static constexpr int WeaponArmorY = 73;
        
        // Accessories
        static constexpr int Tier1AccessoryX = 1071;
        static constexpr int Tier2AccessoryX = 1141;
        static constexpr int Tier3AccessoryX = 1211;
        static constexpr int AccessoriesY = 15;
        
        // ---------------------------------------------------------------------
        // Coordinate conversion
        // ---------------------------------------------------------------------
        
        static int toRaylibY(
            int bottomLeftY,
            int height) noexcept;
        
        static Rectangle toRaylibRectangle(
            int x,
            int y,
            int width,
            int height) noexcept;
        
        // ---------------------------------------------------------------------
        // Travarsal/ Combat Screen rendering
        // ---------------------------------------------------------------------
        
        void drawMainScreenLayout() const;
        void drawCombatScreenLayout(const Game& game) const;
        
        void drawMap(const Game& game) const;
        void drawStatSlots() const;
        void drawGearSlots() const;
        void drawEnemyStatSlots(const Game& game) const;
        
        void drawTitleAssets() const;
        void drawStatAssets(const Game& game) const;
        void drawGearAssets(const Game& game) const;
        
        void drawActionBar() const;
        void drawCombatActionBar() const;
        void drawInventoryActionBar() const;
        
        void drawLootTable(const Game& game) const;
        void drawDiceRoll(const Game& game) const;
        void drawCombatInfo(const Game& game) const;
        void drawFallbackDie(int value, int x, int y, int size) const;
        
        bool m_gameInstructionsOpen{ false };
        
        void drawTexture(
            const Texture2D& texture,
            int x,
            int y,
            int width,
            int height) const;
        
        void drawFallbackText(
            std::string_view text,
            int x,
            int y,
            int width,
            int height,
            int fontSize) const;
        
        void drawWrappedFallbackText(
            std::string_view text,
            int x,
            int y,
            int width,
            int height,
            int fontSize,
            int lineSpacing) const;
 
        MainScreenAssets m_mainScreenAssets;
    };
}