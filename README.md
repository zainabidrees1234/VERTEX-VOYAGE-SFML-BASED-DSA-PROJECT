# VertexVoyage: Pakistan Shortest Path Visualizer 🚗📍

An interactive C++ application that visualizes the **shortest path** between Pakistani cities on a real map using **Dijkstra's Algorithm**. Built with **SFML**, this tool lets you input source, destination, and multiple stops, and visualizes the most efficient route.  

🧭 Features:
- Visual map of Pakistan 🇵🇰
- 📌 City selection with intelligent input matching
- 🛣️ Dijkstra’s algorithm for shortest path
- ➕ Supports multiple stops (custom or optimized order)
- 🖼️ Real-time path drawing using SFML
- 🧮 Adjacency List and Matrix support

---

## ⚙️ SFML Setup (Windows & Visual Studio)

1. 🔽 **Download SFML**  
   Get it from: https://www.sfml-dev.org/download/sfml/2.5.1/

2. 🗂️ **Extract SFML**  
   Extract it somewhere like: `C:\SFML`

3. 🛠️ **Configure Visual Studio Project**
   - Right-click your project → **Properties**
   - Under **C/C++ → General → Additional Include Directories**, add:  
     `C:\SFML\include`
   - Under **Linker → General → Additional Library Directories**, add:  
     `C:\SFML\lib`
   - Under **Linker → Input → Additional Dependencies**, add:
     ```
     sfml-graphics-d.lib  
     sfml-window-d.lib  
     sfml-system-d.lib
     ```
     (Use non-`-d` versions for Release mode)

4. 📦 **Copy DLLs**  
   Copy `sfml-graphics-2.dll`, `sfml-window-2.dll`, `sfml-system-2.dll` from `C:\SFML\bin` to your project’s `Debug` or `Release` folder.

---

## ▶️ How to Run

1. 🖼️ Make sure `map.jpg` is present in your project directory.
2. 🏁 Build and run the project in Visual Studio.
3. 🧠 Input source and destination cities.
4. ➕ Optionally add intermediate stops.
5. 🎯 Watch the shortest path get visualized!

---

## 📂 Project Structure

📁 Project/
├── Output-Image.jpg # Background image of the Pakistan map
├── main.cpp # GUI + user input handling + visual drawing
├── graph.cpp # Graph implementation with Dijkstra's algorithm
├── graph.h # Graph class declaration
├── Secret.txt # Weighted adjacency list of the graph (edges: distances between the cities)
├── README.md # Project description and setup guide

---

## 🧑‍💻 Author  
**Umm-e-Habiba Imran**  
📍 Bahria University, Islamabad  
📧 habibaimrannn@gmail.com  
🔗 [GitHub](https://github.com/habiba-imran) | [LinkedIn](https://www.linkedin.com/in/habiba-imran-118624258/)

---

⭐ *If you like this project, give it a star!*