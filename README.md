# Cinema Console App (Java)

**Cinema Console App** is a simple command-line application developed in Java for managing cinema operations. The app enables users to browse movies, view showtimes, and handle ticket bookings through a straightforward text-based interface.

### Key Features:

* Display a list of movies with descriptions and showtimes
* Book and cancel tickets via console commands
* Manage seat availability and booking records
* User-friendly text-based menu navigation
* Data stored using file system or simple in-memory structures

### Technologies Used:

* Java SE
* Console-based user interface
* File handling for data persistence

---
# Interface Description
## Cinema console application
A console cinema management application that allows users to register, log in and select roles, as well as sort and search users, order tickets and manage the user database.
### Authorization
When entering the program, the user is asked whether he has been registered before.

![Registration](docs/Registration.png)

If the user claims that he has been registered, a situation arises.

![Registration](docs/Login.png)

If the login is correct, a situation arises in accordance with the role the user plays in the system.

![Main_menu](docs/Main_menu.png)

If the user does not claim that he was registered, but wants to, then a situation arises.

![New_user](docs/New_user.png)

After self-registration, the user will be assigned the role of spectator and the spectator menu will appear.

![Main_menu_user](docs/Main_menu_user.png)

### Sorting
If sorting is required, then a situation arises as in the picture.

![Result_sort](docs/Result_sort_id.png)

The result of sorting by name in the picture.

![Result_sort](docs/Result_sort.png)

### Search result
Search by name.

![Result_find](docs/Result_find_name.png)
### Booking tickets
An example of ordering a ticket when choosing a seat in the hall.

![Booking_tickets](docs/Booking_tickets.png)


