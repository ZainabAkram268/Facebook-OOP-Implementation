# Facebook OOP Console Application

A console-based C++ implementation that models core Facebook functionalities using Object-Oriented Programming principles. The application demonstrates social networking features including user profiles, posts, comments, likes, friendships, pages, and memories.

## Features

- **User Management** – Create and manage user profiles
- **Pages** – Create and follow pages
- **Posts** – Create posts with text and activities (feeling, thinking, making, celebrating)
- **Comments** – Add and view comments on posts
- **Likes** – Like/unlike posts with like tracking
- **Friendships** – Add friends and view friend lists
- **Timeline** – View chronological posts
- **Home Feed** – See posts from friends and liked pages
- **Memories** – Share and view past posts with anniversary reminders
- **Data Persistence** – Load all data from text files (Users, Pages, Posts, Comments, Activities, Links)

## OOP Concepts Used

| Concept | Implementation |
|---------|----------------|
| Encapsulation | Private members with public getters/setters |
| Inheritance | `User` and `Page` inherit from `Object`; `Memory` inherits from `Post` |
| Polymorphism | Virtual functions like `PrintAccName()`, `ListView()` |
| Abstraction | `PostContent` abstract class with `Activity` implementation |
| Composition | Posts contain comments and liked-by lists |

## Tech Stack

- **Language:** C++
- **Storage:** Text files
- **Concepts:** OOP, File I/O, Dynamic Memory Management

## Setup & Run

1. **Compile:**
   ```bash
   g++ project.cpp -o facebook
