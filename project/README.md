Elevator Project
================

Summary
-------

This is a project in the course TTK4145 for controlling `n` elevators working in parallell across `n` floors written in [ToBeDecided]. 


Changelog
---------

Initial commit
 - Cloned from github.com/TTK4145/project
 - Updated io.c and io.h in drivers/ for simulator support, old files renamed to io_old.
 - Moved D library file `libphobos2.a` to drivers/ for simulator testing
 - Updated Makefile with required libraries for simulator
 - Compiled all sources
 - Added rough .gitignore

Project specifications:
==============

Summary
-------
Create software for controlling `n` elevators working in parallel across `m` floors.


Main requirements
-----------------
Be reasonable: There may be semantic hoops that you can jump through to create something that is "technically correct". But do not hesitate to contact us if you feel that something is ambiguous or missing from these requirements.

No orders are lost
 - Once the light on an external button (calling an elevator to that floor; top 6 buttons on the control panel) is turned on, an elevator should arrive at that floor
 - Similarly for an internal button (telling the elevator what floor you want to exit at; front 4 buttons on the control panel), but only the elevator at that specific workspace should take the order
 - This means handling both losing network connection, losing power (both to the elevator and the machine that controls it), and software that crashes
   - For internal orders, handling loss of power/software crash implies that the orders are executed once service is restored
   - The time used to detect these failures should be reasonable, ie. on the order of magnitude of seconds (not minutes)
 - If the elevator is disconnected from the network, it should still serve whatever orders are currently "in the system" (ie. whatever lights are showing)
   - It should also keep serving internal orders, so that people can exit the elevator even if it is disconnected

Multiple elevators should be more efficient than one
 - The orders should be distributed across the elevators in a reasonable way
   - Ex: If all three elevators are idle and two of them are at the bottom floor, then a new order at the top floor should be handled by the closest elevator (ie. neither of the two at the bottom).
 - You are free to choose and design your own "cost function" of some sort: Minimal movement, minimal waiting time, etc.
 - The project is not about creating the "best" or "optimal" distribution of orders. It only has to be clear that the elevators are cooperating and communicating.
 
An individual elevator should behave sensibly and efficiently
 - No stopping at every floor "just to be safe"
 - The external "call upward" and "call downward" buttons should behave differently
   - Ex: If the elevator is moving from floor 1 up to floor 4 and there is a downward order at floor 3, then the elevator should not stop on its way upward, but should return back to floor 3 on its way down
 
The lights should function as expected
 - The lights on the external buttons should show the same thing on all `n` workspaces
 - The internal lights should not be shared between elevators
 - The "door open" lamp should be used as a substitute for an actual door, and as such should not be switched on while the elevator is moving
   - The duration for keeping the door open should be in the 1-5 second range

 
Start with `1 <= n <= 3` elevators, and `m == 4` floors. Try to avoid hard-coding these values: You should be able to add a fourth elevator with no extra configuration, or change the number of floors with minimal configuration. You do, however, not need to test for `n > 3` and `m != 4`.

Unspecified behaviour
---------------------
Some things are left intentionally unspecified. Their implementation will not be explicitly tested, and are therefore up to you.

Which orders are cleared when stopping at a floor
 - You can clear only the orders in the direction of travel, or assume that everyone enters/exits the elevator when the door opens
 
How the elevator behaves when it cannot connect to the network during initialization
 - You can either enter a "single-elevator" mode, or refuse to start
 
How the external (call up, call down) buttons work when the elevator is disconnected from the network
 - You can optionally refuse to take these new orders
 

Permitted simplifications and assumptions
-----------------------------------------
Try to create something that works at a base level first, before adding more advanced features. You are of course free to include any or all (or more) of these optional features from the start.

You can make these simplifications and still get full score:
 - At least one elevator is always working normally
 - Stop button & obstruction switch are disabled
   - Their functionality (if/when implemented) is up to you.
 - No multiple simultaneous errors: Only one error happens at a time, but the system must still return to a fail-safe state after this error
 - No network partitioning: Situations where there are multiple sets of two or more elevators with no connection between them can be ignored
   
   

Evaluation
----------

 - Completion  
   This is a test of the complete system, where the student assistants will use a checklist of various scenarios to test that the elevator system works in accordance to the main requirements listed above. If some behaviour/feature is not part of these requirements, it will not be tested. 
   
 - Design  
   This is a 10 minute presentation driven by you, followed by a 5 minute Q&A. The presentation should demonstrate that:
   - The system is robust: It will always return to a fail-safe state where no orders are lost, regardless of any foreseen or unforeseen events
   - You have made conscious and reasoned decisions about network topology, module responsibilities, and other design issues
   - You understand any weaknesses your design has, and how they are (or could be) addressed
   Diagrams and other visual aids are encouraged.
   
 - Code Review  
   This is a 15 minute session where you are in control of the keyboard, and you will be asked to navigate through your implementation. You will be evaluated on:
   - Code quality: Well written, structured, easy to navigate, consistent, etc.
   - If your code is idiomatic wrt. the language (Using the language the way it is supposed to be used)
   - Maintainability: Separated into modules, APIs are complete, easy to swap out pieces of code
   - The ease of adding any missing features (if any)
   - Your ability to navigate and demonstrate understanding of your own code
   
   
   
Language resources
------------------
We encourage submissions to this list! Tutorials, libraries, articles, blog posts, talks, videos...

 - [Python](http://python.org/)
   - [Official tutorial (2.7)](http://docs.python.org/2.7/tutorial/)
   - [Python for Programmers](https://wiki.python.org/moin/BeginnersGuide/Programmers) (Several websites/books/tutorials)
   - [Advanced Python Programming](http://www.slideshare.net/vishnukraj/advanced-python-programming)
   - [Socket Programming HOWTO](http://docs.python.org/2/howto/sockets.html)
 - C
   - [Amended C99 standard](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf) (pdf)
   - [GNU C library](http://www.gnu.org/software/libc/manual/html_node/)
   - [POSIX '97 standard headers index](http://pubs.opengroup.org/onlinepubs/7990989775/headix.html)
   - [POSIX.1-2008 standard](http://pubs.opengroup.org/onlinepubs/9699919799/) (Unnavigable terrible website)
   - [Beej's network tutorial](http://beej.us/guide/bgnet/)
   - [Deep C](http://www.slideshare.net/olvemaudal/deep-c)
 - [Go](http://golang.org/)
   - [Official tour](http://tour.golang.org/)
   - [Go by Example](https://gobyexample.com/)
   - [Learning Go](http://www.miek.nl/projects/learninggo/)
   - From [the wiki](http://code.google.com/p/go-wiki/): [Articles](https://code.google.com/p/go-wiki/wiki/Articles), [Talks](https://code.google.com/p/go-wiki/wiki/GoTalks)
   - [Advanced Go Concurrency Patterns](https://www.youtube.com/watch?v=QDDwwePbDtw) (video): transforming problems into the for-select-loop form
 - [D](http://dlang.org/)
   - [The book](http://www.amazon.com/exec/obidos/ASIN/0321635361/) by Andrei Alexandrescu ([Chapter 1](http://www.informit.com/articles/article.aspx?p=1381876), [Chapter 13](http://www.informit.com/articles/article.aspx?p=1609144))
   - [Programming in D](http://ddili.org/ders/d.en/)
   - [Pragmatic D Tutorial](http://qznc.github.io/d-tut/)
   - [DConf talks](http://www.youtube.com/channel/UCzYzlIaxNosNLAueoQaQYXw/videos)
   - [Vibe.d](http://vibed.org/)
 - [Erlang](http://www.erlang.org/)
   - [Learn you some Erlang for great good!](http://learnyousomeerlang.com/content)
   - [Erlang: The Movie](http://www.youtube.com/watch?v=uKfKtXYLG78), [Erlang: The Movie II: The sequel](http://www.youtube.com/watch?v=rRbY3TMUcgQ)
 - [Rust](http://www.rust-lang.org/)
 - Java
   - [The Java Tutorials](http://docs.oracle.com/javase/tutorial/index.html)
   - [Java 8 API spec](http://docs.oracle.com/javase/8/docs/api/)
 - [Scala](http://scala-lang.org/)
   - [Learn](http://scala-lang.org/documentation/)

<!-- -->
 
 - Design and code quality
   - [The State of Sock Tubes](http://james-iry.blogspot.no/2009/04/state-of-sock-tubes.html): How "state" is pervasive even in message-passing- and functional languages
   - [Defactoring](http://raganwald.com/2013/10/08/defactoring.html): Removing flexibility to better express intent
   - [The Future of Programming](http://vimeo.com/71278954) (video): A presentation on what programming may look like 40 years from now... as if it was presented 40 years ago.
   - [Railway Oriented Programming](http://www.slideshare.net/ScottWlaschin/railway-oriented-programming): A functional approach to error handling
   - [Practical Unit Testing](https://www.youtube.com/watch?v=i_oA5ZWLhQc) (video): "Readable, Maintainable, and Trustworthy"
   - [Core Principles and Practices for Creating Lightweight Design](https://www.youtube.com/watch?v=3G-LO9T3D1M&t=4h31m25s) (video)
   

   
Other
-----
You are encouraged to exchange ideas with your fellow students. Or, to put it another way: You are required to exchange ideas with your fellow students.

You are allowed to copy the network module from the internet/fellow students, but you must make sure that you understand how it works.

**Be gentle with the hardware.** The buttons aren't designed for bashing. Sliding the "elevator" up and down is fine. Make sure no cables are dangling behind and under the workspace.



Drivers
-------
Are found in the directory called [driver](driver)



Objectives
-------

To be able to evaluate your own progress you should set some objectives/milestones. Below we have suggested some objectives, but because of the large amount of freedom we are giving you, these will not necessarily apply well to your project. The important thing is that you make a plan and set some explicit goals, and make sure you don't fall behind.

You should present your goals and progress to the student assistants each week, and they will offer their experiences and insights to help guide you down the right path.

#####Week 8: Run a single elevator
This could mean implementing the simple "elevator algorithm", or any other way of dispatching orders to a single elevator.

#####Week 9: Network and communication
Establish a network across multiple computers who are able to communicate with each other, using your chosen topology.

#####Week 10: Cost function
Create the decision/dispatch system for your elevators. Remember that you might be able to create a temporary version of this system (depending on how it's supposed to work) by creating a "bad" version first (like assigning a random elevator to a new order).

Contact
-------
Any questions or suggestions to this text/the project in general?

Send an email, a message on It's Learning, open an issue, or create a pull request.







