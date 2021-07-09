#! /bin/sh

echo "WEBSERVER TESTER:"

echo "testing GET / (SHOULD RETURN 200 + index)"

curl http://localhost:$1

sleep 4
clear

echo "testing GET /does_not_exist (SHOULD RETURN 404 FILE NOT FOUND + ERROR PAGE)"

curl http://localhost:$1/does_not_exist

sleep 4
clear

echo "testing POST size 0 (SHOULD RETURN 405 METHOD NOT ALLOWED + ERROR PAGE)"

curl -X POST http://localhost:$1/

sleep 4
clear

echo "testing POST with plain text file (SHOULD RETURN 200 OK + create file in root server dir)"

curl -X POST -H "Content-Type: plain/text" --data "BODY IS HERE write something shorter or longer than body limit" http://localhost:$1/plain_text_in_a_plain_file

sleep 4
clear
