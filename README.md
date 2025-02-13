# ❄️ Penguin Battle

**Penguin Battle** is a multiplayer 2D action game inspired by Battle City but with a **winter-themed twist**! Instead of tanks, players control **penguins**, using **snowball launchers** to eliminate opponents and conquer the battlefield. The game follows a **client-server architecture** using the **HTTP protocol** with the **CROW** library and supports **multiplayer battles, destructible environments, and weapon upgrades**.

## 🐧 Game Concept

- Up to **4 penguins** battle in a **snow-covered arena**.
- Players can move in **four directions** and shoot **snowballs** to eliminate enemies.
- The **game board** consists of:
  - ❄️ **Free spaces** where penguins can move.
  - 🧱 **Breakable ice walls** that can be destroyed.
  - 🏔️ **Indestructible ice blocks** that block movement and snowballs.
- Penguins use **snowball launchers**:
  - If a snowball **hits an opponent**, they respawn at their starting position.
  - After **3 eliminations**, a player is removed from the game.
  - **The last penguin standing wins!**
- Players earn **100 points per elimination** and an extra **200 points for winning a match**.

## 🔥 Advanced Mechanics

- **Weapon Upgrades:**  
  - At **500 points**, players can **reduce the cooldown between snowball shots** (up to 4 times).  
  - At **10 score points**, snowball **speed doubles** (one-time upgrade).  
- **Colliding snowballs cancel each other out.**  
- **Hidden ice bombs:** Breaking certain walls may trigger an explosion that eliminates everything in a **10m² radius**.  
- **Friendly Mode (2v2)** – Two-player teams battle against each other, but **no points are awarded**.

## 🛠️ Features

- **Multiplayer snowball battles** (2-4 players per match).  
- **Client-server communication** via **HTTP + CROW framework**.  
- **User authentication** (Login/Register with unique usernames).  
- **SQLite database** for storing user stats and match data.  
- **Matchmaking system** (players wait max 30 seconds before a game starts).  
- **Destructible environments** (ice walls, hidden bombs).  
- **GUI (In Development)** – A graphical interface is planned for future updates.

## 📂 Technologies Used

- **C++** (game logic, server, and client)
- **CROW framework** (for HTTP-based server-client communication)
- **SQLite ORM** (database for storing player data)
- **Qt (Planned for GUI)**

## 👨‍💻 Contributors

The development team behind **Penguin Battle**:

- [sabinaurse](https://github.com/sabinaurse)
- [adinaion](https://github.com/adinaion)
- [Didi](https://github.com/ifrimdiana)
- [Lorincz-Gabriella](https://github.com/Lorincz-Gabriella)

---
📌 *Developed by the Penguin Battle Team* 🐧❄️
