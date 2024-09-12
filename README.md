### Get Out Of My Pi 

#### What ?
A simple c++ program that runs periodically to detect failed SSH attempts and exposing them on telegram with details of each attemp like ip, exact location, used username, etc... as well as banning them after each 3 failed attempts.

#### Why?

Discovering \
Learning \
Fun? 

Why i didn't use existing solutions out there? I TOLD YOU it's FUN to recreate things. \
Also man i am tired of random bots scanning my Pi and brute forcing username + pass to gain access or at least get anything useful over SSH.

#### How?
* Basically it is a C++ program that should run as cron job. (every minute)
* It reads the file in `/var/log/btmp` to see all failed logins.
* We take that input and parse it into what we call inside the program scope: `Attempt`
* Of course we keep the number or the `Cursor` of the last line we  parsed from the previous execution, to not mess it up and read the same input over and over again
* After that, we store the parsed IPs into a map of `<IP, numberOfAttempts>`
* At any given time we read the map and find the IPs that have tried more than 3 times; WE BAN THEM by issuing `iptables` command from `system()` call.
* Ah, and most importantly and why we got this idea: expose these ips to the internet.

#### What are these generated files?
 * `ips` the file that contains the ip store map
* `cursor` the file that contains the last parsed line
* `logs` its only a file that store what's inside /var/log/btmp file
* `secrets` NECESSARY FILE that is used as a .env file and it is read by the `secrets_loader.cpp` file into a map `<line0, line1>, <line2, line3>` ... 

#### Ok. How to try it?
1 - Get the full source code and compile it. \
2 - Create a root-level cron that runs the generated executable file.

#### Authors
* Ahmed Debbech - Original Author
