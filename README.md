# Daily Health Tracker

First C++ Project, learning C++ self taught and in Uni. This is a desktop health tracking application built from scratch using **C++** and **Qt6**.

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Qt6](https://img.shields.io/badge/Qt-6.x-green.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20%2F%20MSYS2-lightgrey.svg)

---

## Features

* **Animated Progress Rings:** Custom-rendered circular progress bars using `QPainter` backed by a 60 FPS smooth easing loop.
* **Live Percentage Indicators:** Dynamically updates and displays the exact completion percentage right in the center of each ring.
* **Modern Dark Aesthetics:** Designed with a clean, borderless dark-mode palette and fully customized via Qt Style Sheets (`style.qss`).
* **Flexible Custom Logging:** Type in any custom amount of calories or water on the fly to instantly update your daily goals.
* **Persistent Setup:** Lock in custom calorie and hydration targets right upon launching.

---

## Project Structure

```text
├── main.cpp         # Main application window, UI layout, custom animated widgets, and event logic
├── tracker.h        # Header definitions for goals and backend tracking functions
├── tracker.cpp      # Backend logic and goal management
└── style.qss        # Custom Qt stylesheet for styling widgets and controls