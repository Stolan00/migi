### DISCLAIMER: Migi is still in active development, and has not yet been released. These docs are currently incomplete, and may not accurately describe the state of the application at any given time.

# Migi
Migi is a desktop application for managing an Anilist library. It is developed with the Qt framework, and is primarily written in C++, while the UI is written in QML. 

Please note, this is a personal project made with the goal of learning about C++ application development within a framework. I cannot guarantee support. For a more feature-complete application serving a similar purpose, see [Taiga](https://taiga.moe/).

# SQLite
Migi uses a SQLite database to store and keep track of your library locally. This database will be created for you the first time you authenticate your account, and updated as needed. While Migi does rely on this database, it is designed to be portable and user-friendly, and it is possible to make use of it elsewhere. It follows Third Normal Form (3NF), and can easily be used in another client or even serve as an offline backup.

To access it, navigate to Migi's [AppData](https://doc.qt.io/qt-6/qstandardpaths.html#AppDataLocation) folder, then go to `/Migi/Migi/Data/db/migi_database.sqlite3`. From there you can directly copy the file and use it as needed.

Various export options are also planned, including markdown and CSV.
