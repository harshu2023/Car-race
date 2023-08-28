#include <iostream>
#include <string>
#include <cmath>
#include "olcConsoleGameEngine.h"
#include <vector>
using namespace std;

class CarRacing : public olcConsoleGameEngine
{
public:
    CarRacing() {
        m_sAppName = L"Formula OLC";
    }
private:
    float dist = 0.0;
    float curvature = 0.0;
    float trackcurvature = 0.0;
    float trackdist = 0.0;
    float carPos = 0.0;
    float carDist = 0.0;
    float carCurvature = 0.0;
    float carSpeed = 0.0;
    vector<pair<float, float>> vecTrack;
    list<float> Laptime;
    float Currentlaptime;


protected:

    virtual bool OnUserCreate() {
        vecTrack.push_back(make_pair(0.0, 100.0));
        vecTrack.push_back(make_pair(1.0, 200.0));
        vecTrack.push_back(make_pair(0.0, 400.0));
        vecTrack.push_back(make_pair(-1.0, 120.0));
        vecTrack.push_back(make_pair(0.0, 500.0));
        vecTrack.push_back(make_pair(1.0, 400.0));
        vecTrack.push_back(make_pair(0.0, 200.0));
        vecTrack.push_back(make_pair(1.0, 700.0));
        vecTrack.push_back(make_pair(0.0, 300.0));


        for (auto t : vecTrack)
            trackdist += t.second;


        Laptime = { 0, 0, 0, 0, 0 };
        Currentlaptime = 0.0;

        return true;
    }

    virtual bool OnUserUpdate(float Elapsedtime) {
        int carDirection = 0;
        if (m_keys[VK_UP].bHeld)
            carSpeed += 2.0 * Elapsedtime;

        else
            carSpeed -= 1.0 * Elapsedtime;


        if (m_keys[VK_LEFT].bHeld)
        {
            carCurvature -= 0.6 * Elapsedtime * (1.0 - carSpeed / 2.0);
            carDirection = -1;
        }
        if (m_keys[VK_RIGHT].bHeld)
        {
            carCurvature += 0.6 * Elapsedtime * (1.0 - carSpeed / 2.0);
            carDirection = +1;
        }
        if (abs(carCurvature - trackcurvature) >= 0.8) {
            carSpeed -= 4.0 * Elapsedtime;
        }
        if (carSpeed > 1.0) { carSpeed = 1.0; }
        if (carSpeed < 0.0) { carSpeed = 0.0; }

        dist += (70.0 * carSpeed) * Elapsedtime;
        float offset = 0;
        int tracksec = 0;

        Currentlaptime += Elapsedtime;

        if (dist >= trackdist) {
            dist -= trackdist;
            Laptime.push_front(Currentlaptime);
            Laptime.pop_back();
            Currentlaptime = 0.0;

        }
        while (tracksec < vecTrack.size() && offset <= dist) {
            offset += vecTrack[tracksec].second;
            tracksec++;
        }
        float targetcurvature = vecTrack[tracksec - 1].first;

        float trackcurvediff = (targetcurvature - carCurvature) * Elapsedtime * carSpeed;
        carCurvature += trackcurvediff;
        trackcurvature = carCurvature * carSpeed * Elapsedtime;


        for (int y = 0;y < ScreenHeight() / 2;y++) {
            for (int x = 0;x < ScreenWidth();x++) {
                Draw(x, y, y < ScreenHeight() / 4 ? PIXEL_HALF : PIXEL_SOLID, FG_DARK_BLUE);
            }
        }
        for (int x = 0;x < ScreenWidth();x++) {
            int hill =(int)abs(sin(x * 0.01 + trackcurvature)*16.0);
            for (int y = (ScreenHeight() / 2) - hill;y < ScreenHeight() / 2;y++) {
                Draw(x, y, PIXEL_SOLID, FG_DARK_YELLOW);
            }
        }

        for (int y = 0;y < ScreenHeight() / 2.0;y++)
            for (int x = 0;x < ScreenWidth();x++) {

                float perspective = (float)y / (ScreenHeight() / 2.0);
                float roadwidth = 0.1 + 0.8 * perspective;
                float clipwidth = 0.15 * roadwidth;

                roadwidth *= 0.5;

                float middlepoint = 0.5 + curvature * pow((1.0 - perspective), 3);

                int leftgrass = (middlepoint - roadwidth - clipwidth) * ScreenWidth();
                int leftclip = (middlepoint - roadwidth) * ScreenWidth();
                int rightclip = (middlepoint + roadwidth) * ScreenWidth();
                int rightgrass = (middlepoint + roadwidth + clipwidth) * ScreenWidth();

                int row = y + ScreenHeight() / 2;

                int grasscolour = sin(20.0 * pow((1 - perspective), 3) + dist * 0.1) > 0.0 ? FG_GREEN : FG_DARK_GREEN;

                int clipcolour = sin(80.0 * pow((1 - perspective), 3) + dist) > 0.0 ? FG_GREEN : FG_DARK_GREEN;

                int roadcolour = (tracksec - 1) == 0 ? FG_WHITE : FG_GREY;

                if (x >= 0 && x < leftgrass) {
                    Draw(x, row, PIXEL_SOLID, grasscolour);
                }
                if (x >= leftgrass && x < leftclip) {
                    Draw(x, row, PIXEL_SOLID, clipcolour);
                }
                if (x >= leftclip && x < rightclip) {
                    Draw(x, row, PIXEL_SOLID, roadcolour);
                }
                if (x >= rightclip && x < rightgrass) {
                    Draw(x, row, PIXEL_SOLID, clipcolour);
                }
                if (x >= rightgrass && x < ScreenWidth()) {
                    Draw(x, row, PIXEL_SOLID, grasscolour);
                }
            }

                carPos = carCurvature - trackcurvature;
                int carpos = (ScreenWidth() / 2) + ((int)(ScreenWidth() * carPos) / 2) - 7;

                switch (carDirection) {
                case 0:
                    DrawStringAlpha(carPos, 80, L"  ||####||   ");
                    DrawStringAlpha(carPos, 80, L"     ##      ");
                    DrawStringAlpha(carPos, 80, L"    ####     ");
                    DrawStringAlpha(carPos, 80, L"  ||####||   ");
                    DrawStringAlpha(carPos, 80, L" ||######||  ");
                    DrawStringAlpha(carPos, 80, L"  ||####||   ");
                    break;


                case 1:
                    DrawStringAlpha(carPos, 80, L"    //####// ");
                    DrawStringAlpha(carPos, 80, L"      ##     ");
                    DrawStringAlpha(carPos, 80, L"    ####     ");
                    DrawStringAlpha(carPos, 80, L"  //####//   ");
                    DrawStringAlpha(carPos, 80, L" //######//  ");
                    DrawStringAlpha(carPos, 80, L" //####//    ");
                    break;

                case -1:
                    DrawStringAlpha(carPos, 80, L" \\####\\    ");
                    DrawStringAlpha(carPos, 80, L"    ##       ");
                    DrawStringAlpha(carPos, 80, L"    ####     ");
                    DrawStringAlpha(carPos, 80, L"   \\####\\  ");
                    DrawStringAlpha(carPos, 80, L"   \\######\\");
                    DrawStringAlpha(carPos, 80, L"    \\####\\ ");
                    break;
                }

                DrawString(0, 0, L"Distance: " + to_wstring(dist));
                DrawString(0, 1, L"Target Curvature:" + to_wstring(curvature));
                DrawString(0, 2, L"Car Curvature:" + to_wstring(carCurvature));
                DrawString(0, 3, L"Track Curvature:" + to_wstring(trackcurvature));
                DrawString(0, 4, L"Car Speed:" + to_wstring(carSpeed));

                auto displaytime = [](float t) {

                    int minutes = t / 60.0;
                    int seconds = t - (minutes * 60.0);
                    int milliseconds = (t - (float)seconds) * 1000.0;

                    return to_wstring(minutes) + L"." + to_wstring(seconds) + L":" + to_wstring(milliseconds);

                };

                DrawString(10, 8, displaytime(Currentlaptime));

                int j = 10;

                for (auto l : Laptime) {
                    DrawString(10, j, displaytime(l));
                    j++;
                }

                return true;
    }


};

int main()
{
    CarRacing game;
    game.ConstructConsole(160, 100, 8, 8);
    game.Start();


}
