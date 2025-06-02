#include <iostream>
#include <sstream>
#include <unordered_map>
#include <fstream>
#include <cctype>
#include <cmath>
#include <algorithm>

#include "inputbox.h"
#include "graph.h"

using namespace std;

const int INF = 1000000;

string cityNames[] = {
    "gilgit", "mingora", "mardan", "peshawar", "muzzafarabad", "bannu", "deraismailkhan", "parchinar",
    "rawalpindi", "sialkot", "sargodha", "faislabad", "lahore", "multan","bahawalpur", "rahimyarkhan",
    "sadiqabad", "zhob", "chamman", "quetta", "deramuradjamali", "jacobabad", "sukkur", "larkana",
    "nawabshah", "mirpurkhas", "hyderabad", "karachi", "turbat", "gwadar", "islamabad"
};

string toLower(const string& str) {
    string res = str;
    transform(res.begin(), res.end(), res.begin(), ::tolower);
    return res;
}

unordered_map<string, int> cityNameToIndex;

void initializeCityMap() {
    int cityCount = static_cast<int>(sizeof(cityNames) / sizeof(cityNames[0]));
    for (int i = 0; i < cityCount; ++i) {
        cityNameToIndex[toLower(cityNames[i])] = i;
    }
}

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first == string::npos) ? "" : str.substr(first, last - first + 1);
}

int getExactCityIndex(const string& input) {
    string key = toLower(trim(input));
    auto it = cityNameToIndex.find(key);
    return (it != cityNameToIndex.end()) ? it->second : -1;
}

void loadGraph(Graph& g, const string& filename) {
    ifstream file(filename);
    int totalVertices, u, v, w;
    if (!(file >> totalVertices)) {
        cerr << "Error reading total vertices from file\n";
        exit(1);
    }
    string line;
    getline(file, line); // consume newline after totalVertices

    while (getline(file, line)) {
        istringstream iss(line);
        if (!(iss >> u)) continue;
        while (iss >> v) {
            if (!(iss >> w)) break;
            g.addEdge(u, v, w);
        }
    }
    file.close();
}

pair<vector<int>, int> computeBestPath(Graph& graph, int src, int dest, const vector<int>& stops, bool useCustomStopOrder) {
    vector<int> bestPath;
    int bestDistance = INF;

    if (useCustomStopOrder) {
        vector<int> waypoints = { src };
        waypoints.insert(waypoints.end(), stops.begin(), stops.end());
        waypoints.push_back(dest);

        vector<int> fullPath;
        int totalDistance = 0;
        bool valid = true;

        for (size_t i = 0; i < waypoints.size() - 1; ++i) {
            int dist = 0;
            vector<int> seg = graph.dijkstra(waypoints[i], waypoints[i + 1], dist);
            if (seg.empty()) {
                cout << "No path between " << cityNames[waypoints[i]] << " and " << cityNames[waypoints[i + 1]] << endl;
                valid = false;
                break;
            }
            if (!fullPath.empty()) seg.erase(seg.begin());
            fullPath.insert(fullPath.end(), seg.begin(), seg.end());
            totalDistance += dist;
        }
        if (valid) {
            bestDistance = totalDistance;
            bestPath = fullPath;
        }
    }
    else {
        vector<int> perm = stops;
        sort(perm.begin(), perm.end());
        do {
            vector<int> tempPath;
            int tempDistance = 0;
            vector<int> route = { src };
            route.insert(route.end(), perm.begin(), perm.end());
            route.push_back(dest);

            bool valid = true;
            for (size_t i = 0; i < route.size() - 1; ++i) {
                int dist = 0;
                vector<int> seg = graph.dijkstra(route[i], route[i + 1], dist);
                if (seg.empty()) {
                    cout << "No path between " << cityNames[route[i]] << " and " << cityNames[route[i + 1]] << endl;
                    valid = false;
                    break;
                }
                if (!tempPath.empty()) seg.erase(seg.begin());
                tempPath.insert(tempPath.end(), seg.begin(), seg.end());
                tempDistance += dist;
            }
            if (valid && tempDistance < bestDistance) {
                bestDistance = tempDistance;
                bestPath = tempPath;
            }
        } while (next_permutation(perm.begin(), perm.end()));
    }

    return { bestPath, bestDistance };
}

int main() {
    initializeCityMap();
    const int totalCities = static_cast<int>(sizeof(cityNames) / sizeof(cityNames[0]));
    Graph graph(totalCities);
    loadGraph(graph, "Secret.txt");

    const int windowWidth = 980;
    const int windowHeight = 980;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "VertexVoyage");

    bool showError = false;

    sf::Texture mapTexture;
    if (!mapTexture.loadFromFile("C:\\Users\\user\\Desktop\\Project3\\Project3\\Pakistan-map.jpg")) {
        std::cerr << "Failed to load map image!" << std::endl;
        return -1;
    }

    sf::Sprite mapSprite(mapTexture);
    sf::Vector2u imageSize = mapTexture.getSize();
    float scaleX = static_cast<float>(windowWidth) / imageSize.x;
    float scaleY = static_cast<float>(windowHeight) / imageSize.y;
    mapSprite.setScale(scaleX, scaleY);

    sf::RectangleShape overlay(sf::Vector2f(windowWidth, windowHeight));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/segoeui.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return -1;
    }

    bool showUI = true;
    bool useCustomStopOrder = true;

    // Radio buttons
    sf::RectangleShape radioUserOrder(sf::Vector2f(20, 20));
    radioUserOrder.setFillColor(sf::Color::Blue);
    radioUserOrder.setPosition(50, 140);

    sf::RectangleShape radioBestOrder(sf::Vector2f(20, 20));
    radioBestOrder.setFillColor(sf::Color::White);
    radioBestOrder.setPosition(50, 180);

    sf::Text radioUserText("Use My Stop Order", font, 18);
    radioUserText.setPosition(80, 135);
    radioUserText.setFillColor(sf::Color::Black);

    sf::Text radioBestText("Shortest Path (Best Stop Order)", font, 18);
    radioBestText.setPosition(80, 175);
    radioBestText.setFillColor(sf::Color::Black);

    int overlayAlpha = 150;
    bool fadingOutOverlay = false;

    bool useMatrix = false;

    sf::RectangleShape sidebar(sf::Vector2f(380, windowHeight));
    sidebar.setFillColor(sf::Color(0, 0, 0, 120));
    sidebar.setPosition(30, 0);

    sf::Text sourceCityText("Source City:", font, 28);
    sourceCityText.setFillColor(sf::Color::White);
    sourceCityText.setPosition(50, 50);

    sf::Text destCityText("Destination City:", font, 28);
    destCityText.setFillColor(sf::Color::White);
    destCityText.setPosition(50, 140);

    sf::Text stopCityText("Add Stop City:", font, 28);
    stopCityText.setFillColor(sf::Color::White);
    stopCityText.setPosition(50, 230);

    sf::Text errorText;
    errorText.setFont(font);
    errorText.setCharacterSize(18);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(50, 495);

    sf::RectangleShape addStopButton(sf::Vector2f(40, 40));
    addStopButton.setPosition(360, 270);
    addStopButton.setFillColor(sf::Color(100, 180, 100));

    sf::Text plusText("+", font, 28);
    plusText.setFillColor(sf::Color::White);
    plusText.setPosition(370, 270);

    sf::Text stopHintText("Click '+' to add more than 1 stops", font, 16);
    stopHintText.setFillColor(sf::Color(200, 200, 200));
    stopHintText.setPosition(50, 315);

    InputBox sourceInput(50, 90, 300, 40, font);
    InputBox destInput(50, 180, 300, 40, font);
    InputBox stopInput(50, 270, 300, 40, font);

    sf::RectangleShape findButton(sf::Vector2f(280, 50));
    findButton.setPosition(50, 360);
    findButton.setFillColor(sf::Color(70, 130, 180));

    sf::Text findButtonText("Find Shortest Path", font, 25);
    findButtonText.setFillColor(sf::Color::White);
    findButtonText.setPosition(70, 370);

    sf::RectangleShape modeButton(sf::Vector2f(300, 40));
    modeButton.setPosition(50, 430);
    modeButton.setFillColor(sf::Color(60, 60, 120));

    sf::Text modeButtonText("Using: Adjacency List", font, 20);
    modeButtonText.setFillColor(sf::Color::White);
    modeButtonText.setPosition(60, 435);

    sf::Text modeHintText("Click the button to switch to Adjacency Matrix", font, 18);
    modeHintText.setFillColor(sf::Color::White);
    modeHintText.setPosition(50, 475);

    sf::Text distanceText;
    distanceText.setFont(font);
    distanceText.setCharacterSize(30);
    distanceText.setFillColor(sf::Color::Black);
    distanceText.setPosition(50, 70);

    map<int, sf::Vector2f> cityCoordinates = {
        {0, {771, 95}}, {1, {653, 181}}, {2, {680, 206}}, {3, {639, 237}}, {4, {735, 225}},
        {5, {581, 290}}, {6, {593, 371}}, {7, {549, 263}}, {8, {683, 274}}, {9, {784, 326}},
        {10, {689, 352}}, {11, {686, 397}}, {12, {775, 403}}, {13, {630, 497}}, {14, {609, 545}},
        {15, {579, 596}}, {16, {554, 617}}, {17, {512, 414}}, {18, {343, 460}}, {19, {383, 501}},
        {20, {438, 597}}, {21, {449, 622}}, {22, {484, 654}}, {23, {437, 670}}, {24, {461, 769}},
        {25, {486, 802}}, {26, {452, 819}}, {27, {365, 837}}, {28, {163, 751}}, {29, {125, 804}},
        {30, {707, 261}}
    };

    vector<int> finalPath;
    vector<int> stops;
    vector<int> originalStops;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (showUI) {
                sourceInput.handleEvent(event, window);
                destInput.handleEvent(event, window);
                stopInput.handleEvent(event, window);
            }

            // Radio button toggling and recompute path
            if (!showUI && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                bool recompute = false;
                if (radioUserOrder.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    if (!useCustomStopOrder) {
                        useCustomStopOrder = true;
                        radioUserOrder.setFillColor(sf::Color::Blue);
                        radioBestOrder.setFillColor(sf::Color::White);
                        recompute = true;
                    }
                }
                else if (radioBestOrder.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    if (useCustomStopOrder) {
                        useCustomStopOrder = false;
                        radioBestOrder.setFillColor(sf::Color::Blue);
                        radioUserOrder.setFillColor(sf::Color::White);
                        recompute = true;
                    }
                }
                if (recompute) {
                    int src = getExactCityIndex(sourceInput.getText());
                    int dest = getExactCityIndex(destInput.getText());
                    if (src == -1 || dest == -1) {
                        errorText.setString("Invalid source or destination city!");
                        showError = true;
                        break;
                    }
                    pair<vector<int>, int> result = computeBestPath(graph, src, dest, originalStops, useCustomStopOrder);
                    vector<int> path = result.first;
                    int dist = result.second;
                    if (path.empty()) {
                        errorText.setString("No valid path found with selected stops.");
                        showError = true;
                    }
                    else {
                        showError = false;
                        finalPath = path;
                        distanceText.setString("Total Distance: " + to_string(dist) + " km");
                    }
                }
            }

            // Find path button pressed
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left
                && showUI && findButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {

                int src = getExactCityIndex(sourceInput.getText());
                int dest = getExactCityIndex(destInput.getText());

                if (src == -1 || dest == -1) {
                    errorText.setString("Invalid source or destination city!");
                    showError = true;
                    break;
                }

                originalStops = stops;  // preserve stops

                pair<vector<int>, int> result = computeBestPath(graph, src, dest, originalStops, useCustomStopOrder);
                vector<int> path = result.first;
                int dist = result.second;

                if (path.empty()) {
                    errorText.setString("No valid path found with selected stops.");
                    showError = true;
                }
                else {
                    showError = false;
                    finalPath = path;
                    distanceText.setString("Total Distance: " + to_string(dist) + " km");
                }

                fadingOutOverlay = true;
                showUI = false;
            }

            // Add stop button logic (unchanged)
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left
                && showUI && addStopButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {

                string stopText = stopInput.getText();
                stringstream ss(stopText);
                string token;
                bool foundInvalidStop = false;
                while (getline(ss, token, ',')) {
                    token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
                    int idx = getExactCityIndex(token);
                    if (idx != -1 && find(stops.begin(), stops.end(), idx) == stops.end()) {
                        stops.push_back(idx);
                        cout << "Added stop: " << token << endl;
                    }
                    else {
                        cout << "Invalid stop: " << token << endl;
                        foundInvalidStop = true;
                    }
                }
                stopInput.clear();
                if (stops.size() >= 8) {
                    errorText.setString("Too many stops may affect performance!");
                    showError = true;
                }
                else if (foundInvalidStop) {
                    errorText.setString("One or more stop cities are invalid!");
                    showError = true;
                }
                else {
                    showError = false;
                }
            }

            // Mode toggle logic (unchanged)
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left
                && showUI && modeButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                useMatrix = !useMatrix;
                graph.setMode(useMatrix);
                modeButtonText.setString(useMatrix ? "Using: Adjacency Matrix" : "Using: Adjacency List");
                modeHintText.setString(useMatrix ? "Click to switch to List" : "Click to switch to Matrix");
            }
        }

        if (fadingOutOverlay) {
            overlayAlpha -= 5;
            if (overlayAlpha <= 0) {
                overlayAlpha = 0;
                fadingOutOverlay = false;
            }
            overlay.setFillColor(sf::Color(0, 0, 0, overlayAlpha));
        }

        window.clear();
        window.draw(mapSprite);

        // Draw path lines
        if (finalPath.size() >= 2) {
            for (size_t i = 0; i < finalPath.size() - 1; ++i) {
                sf::Vector2f p1 = cityCoordinates[finalPath[i]];
                sf::Vector2f p2 = cityCoordinates[finalPath[i + 1]];
                sf::Vector2f direction = p2 - p1;
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159265f;
                sf::RectangleShape thickLine(sf::Vector2f(length, 4));
                thickLine.setPosition(p1);
                thickLine.setRotation(angle);
                thickLine.setFillColor(sf::Color::Red);
                window.draw(thickLine);
            }
        }

        if (showUI || overlayAlpha > 0) {
            overlay.setFillColor(sf::Color(0, 0, 0, overlayAlpha));
            window.draw(overlay);
            window.draw(sidebar);
        }

        if (showUI) {
            window.draw(sourceCityText);
            window.draw(destCityText);
            window.draw(stopCityText);
            window.draw(stopHintText);
            sourceInput.draw(window);
            destInput.draw(window);
            stopInput.draw(window);
            window.draw(findButton);
            window.draw(findButtonText);
            window.draw(addStopButton);
            window.draw(plusText);
            window.draw(modeButton);
            window.draw(modeButtonText);
            window.draw(modeHintText);
        }

        if (!showUI && !finalPath.empty()) {
            window.draw(radioUserOrder);
            window.draw(radioUserText);
            window.draw(radioBestOrder);
            window.draw(radioBestText);
        }

        if (fadingOutOverlay && overlayAlpha > 0) {
            overlayAlpha -= 2;
            if (overlayAlpha <= 0) {
                overlayAlpha = 0;
                fadingOutOverlay = false;
                showUI = false;
            }
        }

        if (!finalPath.empty()) {
            int src = finalPath.front();
            int dest = finalPath.back();

            sf::CircleShape endPointCircle(8);
            endPointCircle.setOrigin(8, 8);
            endPointCircle.setFillColor(sf::Color::Green);
            endPointCircle.setPosition(cityCoordinates[src]);
            window.draw(endPointCircle);

            endPointCircle.setFillColor(sf::Color::Blue);
            endPointCircle.setPosition(cityCoordinates[dest]);
            window.draw(endPointCircle);

            sf::CircleShape stopCircle(8);
            stopCircle.setOrigin(8, 8);
            stopCircle.setFillColor(sf::Color(255, 165, 0));
            for (int stopCity : originalStops) {
                stopCircle.setPosition(cityCoordinates[stopCity]);
                window.draw(stopCircle);
            }

            sf::CircleShape intermediateCircle(6);
            intermediateCircle.setOrigin(6, 6);
            intermediateCircle.setFillColor(sf::Color::Cyan);
            for (int node : finalPath) {
                if (node != src && node != dest && std::find(originalStops.begin(), originalStops.end(), node) == originalStops.end()) {
                    intermediateCircle.setPosition(cityCoordinates[node]);
                    window.draw(intermediateCircle);
                }
            }
        }

        if (showError) {
            window.draw(errorText);
        }
        else if (!finalPath.empty()) {
            window.draw(distanceText);
        }

        window.display();
    }

    return 0;
}