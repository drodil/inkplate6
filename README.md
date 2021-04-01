# inkplate6

My Inkplate6 project with small windowing engine and few
widgets. 

## Settings

Modify necessary settings in Settings.cpp file before compiling.

## Google calendar events

To get this to work you must create a Google Apps Script webapp with 
script located in support/Code.gs. Just change the calendar ID with 
the calendar ID of your choice. 

As the Google webapp doesn't return you Content-Length header that 
Arduino HTTPClient requires to work correctly, you must also host 
the support/proxy.php file in some server. Simply change the url in
that file to point to the webapp and use your own server address
for the project. 

## Other widgets

Other widgets created for the project can be found from the widgets
folder. Just include them in your sketch and add them to the screen
in inkplate6.ino. 

## TODO

* Clean up. A lot.
* Get around the PHP proxy hack