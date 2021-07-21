IPC_TYPE = "SOCKET"
if IPC_TYPE == "SOCKET":
    import socket

    HOST = '127.0.0.1'
    PORT = 9000
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        while True:
            state = s.send(b"Hello server from client!")
            if not state:
                print("Connection closed.")
                break
elif IPC_TYPE == "PIPE":
    import subprocess

    with subprocess.Popen("./server", stdin=subprocess.PIPE, text=True) as p:
        while True:
            state = p.stdin.write("Hello server from client!")
            if not state:
                print("Connection closed.")
                break
