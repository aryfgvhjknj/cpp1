# Student Management System

A console-based C++ application for managing student records with persistent file storage.

## Features

- Add a student with a unique ID
- Display all saved students
- Update an existing student
- Delete a student
- Automatically save records in `students.txt`
- Validate numeric input and required text fields

## Build and run

From this folder, run:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o student_management.exe
.\student_management.exe
```

The program creates `students.txt` automatically after the first successful add, update, or delete operation. Each record is stored as:

```text
id|name|age|course|email
```

Avoid using the `|` character in text fields because it separates stored values.
