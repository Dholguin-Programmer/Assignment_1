// 3rd Party library includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nlohmann/json.hpp>

// C++ standard library includes
#include <stdio.h>
#include <iostream>
#include <thread>
#include <memory>
#include <ctime>
#include <fstream>
#include <stdlib.h>

// System Networking includes
#include <curl/curl.h>

// My library includes
#include "DiegofiedWindow.h"

namespace
{
    // This function, using CURL, downloads a file onto our File System
    bool DownloadFile(const char* url, const char* fileName)
    {
        CURL* curlHandle = nullptr;
        CURLcode curlOpCode;

        curlHandle = curl_easy_init();

        if (curlHandle)
        {
            FILE* fileHandle = fopen(fileName,"wb");
            curl_easy_setopt(curlHandle, CURLOPT_URL, url);
            curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, NULL);
            curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, fileHandle);
            curlOpCode = curl_easy_perform(curlHandle);
            curl_easy_cleanup(curlHandle);
            fclose(fileHandle);
        }

        return curlOpCode == CURLE_OK;
    }
    
    // This function generattes a URL string that is used to download a file reachable from our network
    // TODO: Maybe have some sort of Network Device that handles all of this Network specific functionality
    std::string GenerateUrlString(uint16_t& year, uint8_t& month, uint8_t& day,
                                  std::string protocol, std::string hostName, std::string path)
    {
        std::string curlURL = protocol + "://";
        curlURL += hostName + path;

        // Note that there are a number of static strings we use whenever fetching, 
        // I'd probably need to know more about the 'statsapi' and which each fields means..
        // Once again, maybe a policy of sorts here?

        // Now let's append our query commands
        curlURL += "?";
        
        // Static.. don't really know what this means so leave it be...
        curlURL += "hydrate=game(content(editorial(recap))),decisions";

        curlURL += "&"; // Let's query another thing..

        // Get details regarding latest games 

        curlURL += "date=" + std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);

        curlURL += "&"; // Let's query another thing..

        // Static.. don't really know what this means so leave it be...
        curlURL += "sportId=1";

        // We only care about the month, data, and year fields
        // But to future proff this process, we should have a poilicy
        // of sorts that based upon what we're looking for, we change what we're querying, etc

        return curlURL;
   }

    // If I had more time, I'd try to abstract this into a GameObject class of sorts
    float getObjectsUpperXCoordinateWorldSpace(float maxXCoordinateValueInWorldSpace, 
                                               float objectsScaleX,
                                               float objectsPositionX)
    {
        return objectsPositionX + (objectsScaleX * maxXCoordinateValueInWorldSpace);
    }

    float getObjectsUpperYCoordinateWorldSpace(float maxYCoordinateValueInWorldSpace, 
                                               float objectsScaleY,
                                               float objectsPositionY)
   {
        return objectsPositionY + (objectsScaleY * maxYCoordinateValueInWorldSpace);
   } 
}

int main(int argc, char* argv[]) 
{
    // (1) Create our URL string that we'll be using with CURL to download a file remotely
    time_t curSysTime = time(0);
    tm *curSysTimeStruct = localtime(&curSysTime);
    uint16_t curYear  (curSysTimeStruct->tm_year + 1900);
    uint8_t curMonth (curSysTimeStruct->tm_mon); 
    uint8_t curDay   (curSysTimeStruct->tm_mday);
    std::string urlToParse = GenerateUrlString(curYear,
                                               curMonth,
                                               curDay,
                                               "https",
                                               "statsapi.mlb.com",
                                               "/api/v1/schedule");

    // (2) TODO: Maybe make sure that this string is valid given network configuration possibly here?

    // (3) Using CURL, fetch this file and load it
    std::string downloadFileName = "mlb-stats-" + std::to_string(curYear) + "-" + std::to_string(curMonth) + "-" + std::to_string(curDay); 
    if(DownloadFile(urlToParse.c_str(), downloadFileName.c_str()))
    {   
        /*
            In production, we would want to be doing a number of things concurrently
            although this isn't really needed given the limited scope of the assignment
            this is implemented to showcase that I am aware of the bottlenecks here.
            
            Some things we might want to fix going forward
                1) Do we want some sort of 'Device Manager' to handle the
                execution steps of each individual device on the hardware 

                Ex: SpecificDevice_1
                    Launch Window Device first (hogs all cycles on thread 1)
                    Launch Device_2 second (shares cycles on thread 2)
                    Launch Device_3 third  (shared cycles on thread 2)

                Ex: SpecificDevice_2
                    Launch Window Device first (hogs all cycles on thread 1)
                    Launch Device_2 second (hogs all cycles on thread 2)
                    Launch Device_3 third  (hogs all cycles on thread 3) 
        
            I won't be using concurrency here, but as noted above, in production code, I would
        */        
        std::unique_ptr<Window::Device::Diegofied> diegoWindow (new Window::Device::Diegofied());
        diegoWindow->PrimeDevice();

        /*
            Application Specific Logic - Step 1
            - First extract the JSON fields we care about, as this is what we'll be rendering to the user
        */

        std::ifstream inputFile(downloadFileName.c_str());
        nlohmann::json jsonParser;
        inputFile >> jsonParser;

        /*
            Application Specific Logic - Step 2
            - Let's create our Background for MLB
        */
        diegoWindow->p_renderer->createGeometricObject(Renderer::Types::MlbInfo("NULL", "NULL", "NULL", "NULL"),
                                                       Renderer::Types::GeometricObjects::SQUARE,
                                                       Renderer::Types::RenderingType::STATIC,
                                                       Renderer::Types::Vector3f(0.0f,0.0f,0.0f),
                                                       Renderer::Types::Textures::USING_TEXTURE,
                                                       "images/background.jpg",
                                                       Renderer::Types::Vector3f(0.0f, 0.0f, -50.0f), // position
                                                       Renderer::Types::Vector3f(1.0f, 1.0f, 0.5f), // scale
                                                       "shaders/vertex/3D_color_vertex_texture.vs",
                                                       "shaders/fragment/3D_color_vertex_texture.fs");

        // Let's define our working set as everyhing we really care about...
        // TODO: Add error handlign to this, as this data can come in many different formats based upon the day
        auto workingSetOfGames = jsonParser["dates"].at(0);
        auto totalGamesContainer = workingSetOfGames["games"];
        const unsigned int numGames = workingSetOfGames["totalGames"];

        std::cout << "NumGames: " << numGames << std::endl;
        const float fullWindowX = diegoWindow->m_winWidth;
        const float fullWindowY = diegoWindow->m_winHeight;
        const float floorXValue = 0;
        const float floorYValue = 0;
        
        const float maxXValueWithSpacing = fullWindowX - (fullWindowX * 0.15);
        const float maxXValueOnlySpacing = (fullWindowX * 0.10);

        const float objectXStep = maxXValueWithSpacing / (float)numGames;
        const float spacingXStep = maxXValueOnlySpacing / (float)numGames;
        
        const Renderer::Types::Vector3f scale = Renderer::Types::Vector3f(0.05, 0.05, 0.05f);
        float objectsLowerXCoordinate = spacingXStep;
        for(unsigned int index = 0; index < numGames; index++)
        {
            // Extract Mlb Information for each specific object
            auto gameData = totalGamesContainer.at(index)["teams"];
            Renderer::Types::MlbInfo objInfo = 
                Renderer::Types::MlbInfo(gameData["away"]["team"]["name"], gameData["home"]["team"]["name"],
                                         totalGamesContainer.at(index)["gamesInSeries"].dump(),
                                         totalGamesContainer.at(index)["seriesGameNumber"].dump());

            // Calculates our upper X coordinate value
            const float objectsUpperXCoordinate = getObjectsUpperXCoordinateWorldSpace(fullWindowX, scale.x, objectsLowerXCoordinate);
            
            std::cout << "Object " << index << std::endl;
            std::cout << "Objects Min X Coordinate: " << objectsLowerXCoordinate << std::endl;
            std::cout << "Objects Max X Coordinate: " << objectsUpperXCoordinate << std::endl;

            /* We draw from the lower bound as our orthogonal view objects are drawn rooted drawing wise, from the lower bound (X, Y)
               /\
               ||
               ||
               ||
               ------------------>
               (0,0) 
            */
            Renderer::Types::Vector3f position = Renderer::Types::Vector3f( objectsLowerXCoordinate, fullWindowY / 2.0f, -25);
            diegoWindow->p_renderer->createGeometricObject(objInfo,
                                                           Renderer::Types::GeometricObjects::SQUARE,
                                                           Renderer::Types::RenderingType::DYNAMIC,
                                                           Renderer::Types::Colors::WHITE,
                                                           Renderer::Types::Textures::NO_TEXTURE,
                                                           "IGNORED",
                                                           position,
                                                           scale,
                                                           "shaders/vertex/3D_color_vertex_texture.vs",
                                                           "shaders/fragment/3D_color_vertex_texture.fs");
            
            // This is the delta between the object's upper x position and it's lower x position
            objectsLowerXCoordinate += (objectsUpperXCoordinate - objectsLowerXCoordinate) * 1.5f;

            // Add in our spacing betweeen each of these elements
            objectsLowerXCoordinate += spacingXStep;
        }

        // Blocking call (This starts drawing our Window and in turn our Renderer) i.e begin drawing to screen
        diegoWindow->StartDevice();
    }
    else
    {
        std::cerr << "Failed to download MLB file using URL string: " << urlToParse << std::endl;
    }
    
    return 0;
}