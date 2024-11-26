
# Students Affair Management System

**Author:** Manely Ghasemnia Hamedani  
**Course:** Fundamentals of Computer Programming  

## Overview

This project is a **Students Affair Management System** that helps manage various student services like meal reservations, account charging, meal reporting, and more. The system is designed to handle both student and admin functionalities. The student can reserve meals, check their financial status, vote on polls, and manage their meal plans. Admins, on the other hand, have the ability to approve or remove students, charge student accounts, and manage food and meal plans, along with generating reports.

### Features

The system has two types of users: **Students** and **Admins**, each with specific features.

## Student Features

The following functions are available for students:

1. **Reserve a Meal**:
   - Reserve a meal at a specific self (dining hall) on a particular date.

2. **Take Food**:
   - Mark that the student has taken the meal for the day.

3. **Charge Account**:
   - Add a specific amount to the student’s account.

4. **Send Charge**:
   - Send a charge to another user’s account.

5. **Cancel Reservation**:
   - Cancel a previously made reservation for a meal.

6. **Daily Reserve**:
   - Make a daily reservation for a specific food item.

7. **Define Agent**:
   - Define a meal agent for a given date and meal.

8. **Change Self**:
   - Change the self (dining hall) for a meal reservation.

9. **Check News**:
   - Retrieve the latest news updates from the system.

10. **Vote in Poll**:
    - Vote in an ongoing poll.

11. **Get Reserved Meal Plan**:
    - Get the meal plan for the week, including reservations.

12. **Get Financial Report**:
    - View the financial report of the student’s account.

13. **Get Taken Meal Report**:
    - Get a report of the meals the student has taken.

## Admin Features

Admins have the ability to manage students and the system. The following functions are available for admins:

1. **Approve Student**:
   - Approve a student’s registration or status.

2. **Change Student Password**:
   - Change the password for a student account.

3. **Remove Student**:
   - Remove a student from the system.

4. **Deactivate Student**:
   - Deactivate a student account.

5. **Define Self**:
   - Define a new self (dining hall) with meal time and other details.

6. **Define Food**:
   - Define a new food item in the system.

7. **Define Meal Plan**:
   - Set a meal plan for a self (dining hall) on a given date.

8. **Charge Student Account**:
   - Charge a student’s account with a specific amount.

9. **Add News**:
   - Add news for students to view.

10. **Add Poll**:
    - Create a poll for students to vote on.

11. **Statistics**:
    - Generate statistics about meals taken on a specific date.

12. **Get Student Report**:
    - Retrieve a report for a specific student within a date range.

13. **Get System Report**:
    - Retrieve a system-wide report within a date range.
  
## Compilation Instructions

1. **Prerequisites**:
   - A C compiler (e.g., GCC).
   - A terminal or command-line interface.

2. **Compiling**:
   To compile the project, use the following command in the project directory:

   ```bash
   gcc -o student_affair_system main.c
   ```

3. **Running**:
   To run the compiled program, use:

   ```bash
   ./student_affair_system
   ```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

- Special thanks to the course instructors and peers for feedback and support during the development of this project.
