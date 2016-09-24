This is a simple skeleton parser for PovoCop Job Description Language, used solely for a comparison with Elixir-based implementation. 
You have to install civetweb first. 
The http server starts at localhost:8080 - at this address, after running a server, you can add .zip archives with main.pjdl inside.
This simple parser supports only two statements:

job
   short "description"
   
Because Elixir implementation proved to be only slightly slower, but was faster to create and had less lines of code -- this civetweb
based implementation is abandoned. It is uploaded here only because it will be referenced in our papers sent to scientific
conferences (for comparison purposes only, in spirit of open science).

In case of any problems with compiling or running this example, contact me at (adanilecki -at- cs put poznan pl)
A.D.Danilecki
