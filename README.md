# doing
doing is a lightweight monitoring tool that captures your activities on a desktop computer, either it's watching a particular MV on youtube.com or writing code in Eclipse. The activities are either sent via websocket to analysis server(checkout https://github.com/2erowf/doing_as) Or simply ouputed to std out.  
# how it works
Doing currently only support windows at the moment. It relies on UIAutomation framework to extract value of certain UI control of the foreground process and intelligently identify different activities based on URL or window title of the foreground window and others. The acitivities are detected, recorded, put on a queue to be sent out and eventually sent out. Every activity has a timestamp, duration, and other identity related fields, such as url, process name etc. 
# usage

doing.exe [*-console* | *-remote WEBSOCKET_SERVER*]

*-console*, activities are formatted and output to standard output

*-remote WEBSOCKET_SERVER*, activities are formatted and sent to WEBSOCKET_SERVER via websocket

# TODO
support url extraction from Edge, Firefox, IE on windows

support Mac OS
support GTK+ based linux desktop OSes
