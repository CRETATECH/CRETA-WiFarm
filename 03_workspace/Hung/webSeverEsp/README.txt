Client send message to sever: HTTP POST request;

code:
/*************************/
POST /body HTTP/1.1
Host: 192.168.1.109:80
Content-Type: application/json
Cache-Control: no-cache

// Json 
{"data": "Temperature", "address": "192.168.1.109", "function": "nope"}

/*************************/


note: em không bi?t làm th? nào d? chuy?n do?n code trên thành html nên em s? d?ng tool
là ph?n m?m postman. 
https://www.getpostman.com/apps