# gun_hunt
Gunshot detection homework

A) Download source

    $ git clone https://github.com/surajj1234/gun_hunt.git 

B) Install dependencies

    $ sudo apt-get update
    $ sudo apt-get install netcat
    $ sudo apt-get install libncurses-dev
    $ sudo apt-get install libpthread-stubs0-dev

C) Compile & build

Navigate to the project root directory, then

    $ make clean
    $ make

D) Play the Game

Navigate to the project root directory, then

    $ cd build/
    $ ./gun_hunt

To test network connectivity, open a different terminal and run
    ```$ netcat -ul localhost 4444```
netcat will start listening for incoming UDP packets on port 4444

Enjoy the hunt !
