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


note: em kh�ng bi?t l�m th? n�o d? chuy?n do?n code tr�n th�nh html n�n em s? d?ng tool
l� ph?n m?m postman. 
https://www.getpostman.com/apps