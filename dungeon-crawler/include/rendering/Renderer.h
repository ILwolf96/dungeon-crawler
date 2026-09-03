#pragma once

#include "raylib.h"

namespace dungeon
{
    class Game;

    class Renderer
    {
    public:
        Renderer() = default;

        void draw(const Game& game) const;

    private:
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

        static constexpr int MapFrameThickness = 27;

        static constexpr int VisibleMapWidth =
            MapRenderWidth - (MapFrameThickness * 2);

        static constexpr int VisibleMapHeight =
            MapRenderHeight - (MapFrameThickness * 2);

        static constexpr int TemporaryMapTileSize = 16;

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
        // Main Screen rendering
        // ---------------------------------------------------------------------

        void drawMainScreenLayout() const;
        void drawMap(const Game& game) const;
    };
}