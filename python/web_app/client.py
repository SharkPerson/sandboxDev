import socket

HOST="192.168.0.100"
PORT=16001
DST_HOST="127.0.0.1"
DST_PORT=8000

MESSAGE="HELLO"
my_buf = bytes(MESSAGE, 'utf-8')

with socket.socket() as sock:
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    #sock.bind((DST_HOST,PORT))
    sock.connect((DST_HOST,DST_PORT))
    sock.send(my_buf)
